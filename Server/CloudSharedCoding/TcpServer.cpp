#include "TcpServer.h"
#include <fstream>
#include <sys/timeb.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include<fstream>
#include<fcntl.h>

#define MAXCONNECTION 64
#define EVENT_SIZE 20
#define FILEPATH "/home/heyicong/C++/HizZNetDisk/UserFiles/"
SqlTool* TcpServer::sql = new SqlTool(SqlIP, "root", "191230", "CloudSharedCoding");
std::unordered_map<int, std::string>* TcpServer::userMap = new std::unordered_map<int, std::string>();
std::unordered_map<int, std::vector<int>>* TcpServer::file_map = new std::unordered_map<int, std::vector<int>>();
std::unordered_map<int, std::vector<int>>* TcpServer::project_map = new std::unordered_map<int, std::vector<int>>();
Log::Logger TcpServer::m_logger;
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
    delete file_map;
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
                    userMap->erase(sock_fd);
                }
                else
                {
                    int type = bytesToInt(buf, 0, sizeof(buf));
                    int packageSize = bytesToInt(buf, 4, sizeof(buf));

                    char* data = new char[packageSize + 1];
                    data[packageSize] = '\0';
                    read(sock_fd, data, packageSize);
                    switch (type)
                    {
                    case Package::PackageType::LOGIN:
                    {
                        pool->submit(login, sock_fd,data);
                        break;
                    }
                    case Package::PackageType::INIT_PROJS:
                    {
                        pool->submit(initUserProjects, sock_fd);
                        break;
                    }
                    case Package::PackageType::GET_PROJECT:
                    {
                        pool->submit(sendProjectInfo, sock_fd, data);
                        break;
                    }
                    case Package::PackageType::NEW_PROJECT:
                    {
                        pool->submit(newProject, sock_fd, data);
                        break;
                    }
                    case Package::PackageType::DEL_PROJECT:
                    {
                        pool->submit(delProject, sock_fd, data);
                        break;
                    }
                    case Package::PackageType::GET_FILE:
                    {
                        pool->submit(sendFile, sock_fd, data);
                        break;
                    }
                    case Package::PackageType::NEW_FILE:
                    {
                        pool->submit(newFile, sock_fd, data);
                        break;
                    }
                    case Package::PackageType::DEL_FILE:
                    {
                        pool->submit(delFile, sock_fd, data);
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

void TcpServer::login(int sock_fd, char* data)
{
    std::string temp(data);
    stringList list;
    stringSplit(temp, "\t", list);
    std::string UserId = list[0];
    if (userMap->find(sock_fd) != userMap->end())
    {
        std::string str = "The user is logged in";
        Package pck(str.c_str(), Package::ReturnType::SERVER_ERROR, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
    }
    else
    {
        auto result = sql->exeSql("select user_id from User where user_id = \"" + UserId + "\" and user_password = md5(\"" + list[1] + "\");");
        sqlResultRows vec = sql->getRows(result);
        if (vec.empty())
        {
            std::string error = "The account number or password is incorrect";
            Package pck(error.c_str(), Package::ReturnType::SERVER_ERROR, error.size());
            write(sock_fd, pck.getPdata(), pck.getSize());
        }
        else
        {
            Package pck("", Package::ReturnType::SERVER_ALLOW, 0);
            write(sock_fd, pck.getPdata(), pck.getSize());
            userMap->insert(std::pair<int, std::string>(sock_fd, UserId));
        }
    }

    delete data;
}

void TcpServer::sendProjectInfo(int sock_fd, char* buf)
{
    std::string projId = std::string(buf);

    int pid = std::stoi(projId);
    if (project_map->find(pid) != project_map->end())
    {
        project_map->find(pid)->second.push_back(sock_fd);
    }
    else
    {
        std::vector<int> vec;
        vec.push_back(sock_fd);
        std::pair<int, std::vector<int>> p(pid, vec);
        project_map->insert(p);
    }

    auto projInfoRes = sql->exeSql("select pro_id from Project where pro_id = " + projId + ";");
    auto projInfo = sql->getRows(projInfoRes);
    auto projFileRes = sql->exeSql("select * from File where file_project = " + projId + ";");
    auto rows = sql->getRows(projFileRes);
    int filedNum = mysql_num_fields(projFileRes);
    std::string data = std::string(projInfo[0][0]) + "\n";
    for (auto row : rows)
    {
        for (int i = 0; i < filedNum; i++)
        {
            data += std::string(row[i]) + "\t";
        }
        data += "\n";
    }
    Package pck(data.c_str(), Package::ReturnType::PROJ_FILE_INFO, data.size());
    write(sock_fd, pck.getPdata(), pck.getSize());

    mysql_free_result(projFileRes);
    delete buf;
}

void TcpServer::initUserProjects(int sock_fd)
{
    std::string data = "";

    std::string userId = userMap->find(sock_fd)->second;
    auto userProjRes = sql->exeSql("select Project.pro_id,Project.pro_name,Project.pro_owner,Privilege._privilege_level from Project inner join Privilege\
                                     where (Privilege._user_id = \""+userId+"\" and Privilege._pro_id = Project.pro_id);");
    sqlResultRows rows = sql->getRows(userProjRes);
    for (auto i : rows)
    {
        data += std::string(i[0]) + "\t" + std::string(i[1]) + "\t" + std::string(i[2]) + "\t" + std::string(i[3]) + "\n";
    }
    Package pck(data.c_str(), Package::ReturnType::USER_PROJS,data.size());
    write(sock_fd, pck.getPdata(), pck.getSize());

    mysql_free_result(userProjRes);
}

void TcpServer::newProject(int sock_fd, char* data)
{
    std::string proName(data);
    std::string userpath = "./" + userMap->find(sock_fd)->second;
    std::string path = userpath + "/" + proName;
    std::string userId = userMap->find(sock_fd)->second;
    if (!opendir(userpath.c_str()))
    {
        mkdir(userpath.c_str(),0755);
    }
    mkdir(path.c_str(),0755);

    sql->exeSql("insert into Project (pro_name,pro_owner) value (\"" + proName + "\",\"" + userId + "\")");
    auto res = sql->exeSql("select pro_id,pro_name,pro_owner from Project where pro_name =  \"" + proName + "\";");
    sqlResultRows rows = sql->getRows(res);
    sql->exeSql("insert into Privilege (_user_id,_pro_id) values(\"" + userId + "\"," + std::string(rows[0][0]) + ");");
    std::string str = std::string(rows[0][0]) + "\t" + std::string(rows[0][1]) + "\t" + std::string(rows[0][2]);
    Package pck(str.c_str(), Package::ReturnType::NEW_PROJ_INFO, str.size());
    write(sock_fd, pck.getPdata(), pck.getSize());

    mysql_free_result(res);
    delete data;
}

void TcpServer::delProject(int sock_fd, char* data)
{
    std::string userId = userMap->find(sock_fd)->second;
    std::string buf(data);
    stringList list;
    stringSplit(buf, "\t", list);
    std::string projPath = "./" + userId + "/" + list[1]+"/";

    sql->exeSql("delete from Project where pro_id = " + list[0] + ";");
    sql->exeSql("delete from File where file_project = " + list[0] + ";");

    if (!removeFile(projPath))
    {
        ERROR_LOG(m_logger, "rm_dir ERROR!");
    }

    delete data;
}

void TcpServer::sendFile(int sock_fd, char* data)
{
    std::string temp(data);
    auto res = sql->exeSql("select file_path from File where file_id = " + temp + ";");
    auto row = sql->getRows(res);

    struct stat st;
    stat(row[0][0], &st);
    int fid = std::stoi(temp);

    std::ifstream ifs(row[0][0], std::ios::in | std::ios::binary);
    if (!ifs.is_open())
    {
        ERROR_LOG(m_logger, "ifstream open file error");
    }
    else
    {
        char buffer[1024];
        intToBytes(fid, buffer, 0,sizeof(buffer));

        size_t sended = 0;

        while (sended<st.st_size)
        {
            memset(buffer+4, 0, sizeof(buffer)-4);
            int ret = ifs.readsome(buffer+4, sizeof(buffer)-4);
            sended += ret;
            Package pck(buffer, Package::ReturnType::FILE, ret+4);
            write(sock_fd, pck.getPdata(), pck.getSize());
        }
    }


    if (file_map->find(fid) != file_map->end())
    {
        file_map->find(fid)->second.push_back(sock_fd);
    }
    else
    {
        std::vector<int> vec;
        vec.push_back(sock_fd);
        std::pair<int, std::vector<int>> p(fid, vec);
        file_map->insert(p);
    }

    mysql_free_result(res);
    delete data;
}

void TcpServer::newFile(int sock_fd, char* data)
{
    std::string temp(data);
    stringList list;
    stringSplit(temp, "\t", list);

    std::string path = list[1].substr(0, list[1].find_last_of("/"));

    CreateDir(path);

    int fd = open(list[1].c_str(), O_RDWR | O_CREAT, 0755);

    if (fd != -1) {
        // �ر��ļ�������
        close(fd);
    }

    std::string userId = userMap->find(sock_fd)->second;

    sql->exeSql("insert into File (file_name,file_user,file_path,file_project,file_privilege) \
        values(\"" + list[0] + "\",\"" + userId + "\",\"" + list[1] + "\"," + list[2] + "," + list[3] + ");");

    auto res = sql->exeSql("select * from File where file_path = \"" + list[1] + "\";");
    auto row = sql->getRows(res);

    int num = mysql_num_fields(res);

    std::string buf = "";

    for (int i = 0; i < num; i++)
    {
        buf += std::string(row[0][i]) + "\t";
    }

    Package pck(buf.c_str(), Package::ReturnType::NEW_FILE_INFO,buf.size());
    write(sock_fd, pck.getPdata(), pck.getSize());

    mysql_free_result(res);
    delete data;
}

void TcpServer::delFile(int sock_fd, char* data)
{

    std::string buf(data);
    stringList list;
    stringSplit(buf, "\t", list);
    std::string path = list[1];
    removeFile(path);
    std::string pdir = getParentDir(path);

    int fid = std::stoi(list[0]);
    int pid = std::stoi(list[2]);
    if (file_map->find(fid) != file_map->end())
    {
        file_map->erase(fid);

        //��򿪸���Ŀ�Ĳ����߷����ļ������Ϣ
        std::string buf = list[2] + "\t" + list[0];
        Package pck(buf.c_str(), Package::ReturnType::PROJECT_FILE_DELETE, list[0].size());
        for (auto i : project_map->find(pid)->second)
        {
            write(i, pck.getPdata(), pck.getSize());
        }
    }

    DIR* pDir;
    struct dirent* ptr;

    pDir = opendir(pdir.c_str());

    if((ptr = readdir(pDir))!=0)
        closedir(pDir);
    else
    {
        while ((ptr = readdir(pDir)) == 0)
        {
            pdir = getParentDir(pdir);
            closedir(pDir);
            pDir = opendir(pdir.c_str());
        }
        closedir(pDir);
        removeFile(pdir);
    }

    sql->exeSql("delete from File where file_id = " + list[0] + ";");

    delete data;
}
