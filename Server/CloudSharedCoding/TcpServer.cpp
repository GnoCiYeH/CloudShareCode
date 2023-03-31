#include "TcpServer.h"
#include <fstream>
#include <sys/timeb.h>
#include <errno.h>

#define MAXCONNECTION 64
#define EVENT_SIZE 20
#define FILEPATH "/home/heyicong/C++/HizZNetDisk/UserFiles/"
SqlTool* TcpServer::sql = new SqlTool(SqlIP, "root", "191230", "CloudSharedCoding");
std::unordered_map<int, std::string>* TcpServer::userMap = new std::unordered_map<int, std::string>();
TcpServer::TcpServer(const char* ip, uint32_t port, Log::Logger& logger)
{
    m_logger = logger;
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = inet_addr(ip);
    m_server_addr.sin_port = htons(port);

    pool = new ThreadPool(1024);
    pool->init();
}

TcpServer::~TcpServer()
{
    close(m_sock_fd);
    close(m_epfd);
    delete sql;
    delete pool;
    delete userMap;
}

void TcpServer::tcpStart()
{
    INFO_LOG(m_logger, "Server start!");
    int ret;
    m_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    FATAL_CHECK(m_sock_fd, "Create socket error", m_logger);
    int reuse = 1;
    if (setsockopt(m_sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopet error\n");
    }
    ret = bind(m_sock_fd, (sockaddr*)&m_server_addr, sizeof(m_server_addr));
    FATAL_CHECK(ret, "TcpServer bind ERROR!", m_logger);
    ret = listen(m_sock_fd, MAXCONNECTION);
    FATAL_CHECK(ret, "TcpServer listen ERROR", m_logger);
    m_epfd = epoll_create(MAXCONNECTION);

    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = m_sock_fd;

    epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_sock_fd, &ev);
    epoll_event events[EVENT_SIZE];

    while (true)
    {
        int eNum = epoll_wait(m_epfd, events, EVENT_SIZE, -1);
        FATAL_CHECK(eNum, "epoll_wait error", m_logger);
        char buf[8];

        for (int i = 0; i < eNum; i++)
        {
            if (events[i].data.fd == m_sock_fd)
            {
                ret = accept(m_sock_fd, NULL, NULL);
                ERROR_CHECK(ret, "TcpServer accept error", m_logger);
                ev.data.fd = ret;
                ev.events = EPOLLIN;
                epoll_ctl(m_epfd, EPOLL_CTL_ADD, ret, &ev);
                INFO_LOG(m_logger, "connect");
            }
            else if (events[i].events & EPOLLIN)
            {
                memset(buf, 0, sizeof(buf));
                int sock_fd = events[i].data.fd;
                if ((ret = read(sock_fd, buf, sizeof(buf))) == 0)
                {
                    INFO_LOG(m_logger, std::string("client_fd:") + (m_logger, std::to_string(sock_fd)) + " disconnected");
                    epoll_ctl(m_epfd, EPOLL_CTL_DEL, sock_fd, NULL);
                }
                else
                {
                    int type = bytesToInt(buf, 0, sizeof(buf));
                    int packageSize = bytesToInt(buf, 4, sizeof(buf));

                    switch (type)
                    {
                    case Package::PackageType::LOGIN:
                    {
                        pool->submit(login, sock_fd, packageSize);
                        break;
                    }
                    case Package::PackageType::INIT_PROJS:
                    {
                        pool->submit(initUserProjects, sock_fd);
                        break;
                    }
                    case Package::PackageType::GET_PROJECT:
                    {
                        pool->submit(sendProjectInfo, sock_fd,packageSize);
                        break;
                    }
                    default:
                        INFO_LOG(m_logger, "UNKNOW PACKAGETYPE");
                        break;
                    }
                }
            }
        }
    }
}

void TcpServer::login(int sock_fd, int packageSize)
{
    char data[packageSize + 1];
    data[packageSize] = '\0';
    read(sock_fd, data, packageSize);
    std::string temp(data);
    stringList list;
    stringSplit(temp, "\t", list);
    std::string UserId = list[0];
    if (userMap->find(sock_fd) != userMap->end())
    {
        std::string str = "The user is logged in";
        Package pck(str.c_str(), Package::ReturnType::ERROR, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
    }
    else
    {
        auto result = sql->exeSql("select user_id from User where user_id = \"" + UserId + "\" and user_password = md5(\"" + list[1] + "\");");
        sqlResultRows vec = sql->getRows(result);
        if (vec.empty())
        {
            std::string error = "The account number or password is incorrect";
            Package pck(error.c_str(), Package::ReturnType::ERROR, error.size());
            write(sock_fd, pck.getPdata(), pck.getSize());
        }
        else
        {
            Package pck("", Package::ReturnType::ALLOW, 0);
            write(sock_fd, pck.getPdata(), pck.getSize());
            userMap->insert(std::pair<int, std::string>(sock_fd, UserId));
        }
    }
}

void TcpServer::sendProjectInfo(int sock_fd,int packageSize)
{
    char buf[packageSize+1];
    buf[packageSize] = '\0';
    read(sock_fd, buf, packageSize);
    std::string projId = std::string(buf);
    auto projRes = sql->exeSql("select * from Project where pro_id = " + projId);
    auto rows = sql->getRows(projRes);
    std::string data = std::string(rows[0][0]) + "\t" + std::string(rows[0][1]) + "\t" + std::string(rows[0][2]);
    Package pck(data.c_str(), Package::ReturnType::PROJ_INFO, data.size());
    write(sock_fd, pck.getPdata(), pck.getSize());
    mysql_free_result(projRes);
}

void TcpServer::initUserProjects(int sock_fd)
{
    std::string data = "";

    std::string userId = userMap->find(sock_fd)->second;
    auto userProjRes = sql->exeSql("select Project.pro_id,Project.pro_name,Project.pro_owner from Project inner join Privilege\
                                     where (Privilege._user_id = \""+userId+"\" and Privilege._pro_id = Project.pro_id);");
    sqlResultRows rows = sql->getRows(userProjRes);
    for (auto i : rows)
    {
        data += std::string(i[0]) + "\t" + std::string(i[1]) + "\t" + std::string(i[2]) + "\n";
    }
    Package pck(data.c_str(), Package::ReturnType::USER_PROJS,data.size());
    write(sock_fd, pck.getPdata(), pck.getSize());
    mysql_free_result(userProjRes);
}
