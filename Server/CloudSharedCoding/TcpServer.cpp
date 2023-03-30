#include "TcpServer.h"
#include <fstream>
#include <sys/timeb.h>
#include <errno.h>

#define MAXCONNECTION 64
#define EVENT_SIZE 20
#define FILEPATH "/home/heyicong/C++/HizZNetDisk/UserFiles/"
SqlTool* TcpServer::sql = new SqlTool(SqlIP, "root", "191230", "CloudSharedCoding");
TcpServer::TcpServer(const char* ip, uint32_t port, uint32_t fileport, Log::Logger logger)
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
}

void TcpServer::tcpStart()
{
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
    if (userMap.find(sock_fd) != userMap.end())
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
            std::string info = ""; //此处应为返回客户端的用户初始化信息
            Package pck(info.c_str(), Package::ReturnType::ALLOW, info.size());
            write(sock_fd, pck.getPdata(), pck.getSize());
        }
    }
}

void TcpServer::sendFile(int sock_fd, std::string path, int fileSize)
{
    int sendSize = 0;
    int ret = 0;
    char buf[1024];
    std::ifstream ifs;
    ifs.open(path, std::ios::in | std::ios::binary);
    if (!ifs.is_open())
    {
        std::cout << "send file open error";
    }
    while (sendSize < fileSize)
    {
        ret = ifs.readsome(buf, sizeof(buf));
        sendSize += ret;
        write(sock_fd, buf, ret);
    }
    ifs.close();
}

void TcpServer::sendDir(int sock_fd, std::string* UserId)
{
    std::string data = "";
    MYSQL_RES* file = sql->exeSql("select f_name,f_id,f_parent,f_uploadtime,f_size from File where u_id = \"" + *UserId + "\";");
    int field_num = mysql_num_fields(file);
    std::vector<MYSQL_ROW> file_rows = sql->getRows(file);
    for (auto item : file_rows)
    {
        for (int i = 0; i < field_num; i++)
        {
            if (item[i] != nullptr)
            {
                std::string temp(item[i]);
                data = data.append(temp);
            }
            else
            {
                std::string temp = "null";
                data = data.append(temp);
            }
            data += "\t";
        }
        data.append("\n");
    }
    mysql_free_result(file);
    MYSQL_RES* dir = sql->exeSql("select name,dirId,parent,isLeaf from Directory where userId = \"" + *UserId + "\";");
    field_num = mysql_num_fields(dir);
    std::vector<MYSQL_ROW> dir_rows = sql->getRows(dir);
    for (auto item : dir_rows)
    {
        for (int i = 0; i < field_num; i++)
        {
            if (item[i] != nullptr)
            {
                std::string temp(item[i]);
                data = data.append(temp);
            }
            else
            {
                std::string temp = "null";
                data = data.append(temp);
            }
            data += "\t";
        }
        data.append("\n");
    }
    mysql_free_result(dir);
    Package pck(data.c_str(), Package::UPDATEFILE, data.size());
    write(sock_fd, pck.getPdata(), pck.getSize());
}
