#include "TcpServer.h"
#include <fstream>
#include <sys/timeb.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include<fstream>
#include<fcntl.h>
#include"uuid/uuid.h"
#include<netinet/tcp.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

#define MAXCONNECTION 64
#define EVENT_SIZE 20
#define FILEPATH "/home/heyicong/C++/HizZNetDisk/UserFiles/"
SqlTool* TcpServer::sql = new SqlTool(SqlIP, "root", "123456", "CloudSharedCoding");
std::unordered_map<int, TcpServer::UserInfo>* TcpServer::userMap = new std::unordered_map<int, TcpServer::UserInfo>();
std::multimap<int, int>* TcpServer::file_map = new std::multimap<int, int>();
std::multimap<int, int>* TcpServer::project_map = new std::multimap<int, int>();
std::unordered_map<std::string, int>* TcpServer::projectInData = new std::unordered_map<std::string, int>();
std::unordered_map<std::string, std::vector<int>>* TcpServer::projectPidMap = new std::unordered_map<std::string, std::vector<int>>();
Log::Logger TcpServer::m_logger;
TcpServer::TcpServer(const char* ip, uint32_t port, Log::Logger& logger)
{
    m_logger = logger;
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = inet_addr(ip);
    m_server_addr.sin_port = htons(port);

    pool = new ThreadPool(64);
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
    if (setsockopt(m_sock_fd, IPPROTO_TCP, TCP_NODELAY, &reuse, sizeof(reuse)) < 0)
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
                    auto itor = userMap->find(sock_fd);
                    if (itor == userMap->end())
                        continue;
                    auto user = itor->second;
                    for (auto it : user.openedFiles)
                    {
                        int num = file_map->count(it);
                        auto fit = file_map->find(it);
                        while (num > 0)
                        {
                            if (fit->second == sock_fd)
                            {
                                file_map->erase(fit);
                                break;
                            }
                            num--;
                            fit++;
                        }
                    }
                    for (auto it : user.openedProjects)
                    {
                        int num = project_map->count(it);
                        auto pit = project_map->find(it);
                        while (num > 0)
                        {
                            if (pit->second == sock_fd)
                            {
                                project_map->erase(pit);
                                break;
                            }
                            num--;
                            pit++;
                        }
                    }

                    if (user.runningPid != -1)
                    {
                        killpg(user.runningPid, 9);
                    }

                    userMap->erase(sock_fd);
                }
                else
                {
                    int type = bytesToInt(buf, 0, sizeof(buf));
                    int packageSize = bytesToInt(buf, 4, sizeof(buf));

                    if (type == 0 && packageSize == 0)
                    {
                        continue;
                    }
                    if (packageSize > 1024 * 1024 || packageSize < 0)
                    {
                        continue;
                    }

                    char* data = new char[packageSize + 1];
                    data[packageSize] = '\0';
                    read(sock_fd, data, packageSize);
                    switch (type)
                    {
                    case (int)Package::PackageType::LOGIN:
                    {
                        pool->submit(login, sock_fd,data);
                        break;
                    }
                    case (int)Package::PackageType::TEXT_CHANGE:
                    {
                        sendTextChange(sock_fd, data);
                        //pool->submit(sendTextChange, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::INIT_PROJS:
                    {
                        pool->submit(initUserProjects, sock_fd);
                        delete[] data;
                        break;
                    }
                    case (int)Package::PackageType::GET_PROJECT:
                    {
                        pool->submit(sendProjectInfo, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::NEW_PROJECT:
                    {
                        pool->submit(newProject, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::DEL_PROJECT:
                    {
                        pool->submit(delProject, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::GET_FILE:
                    {
                        pool->submit(sendFile, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::NEW_FILE:
                    {
                        pool->submit(newFile, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::DEL_FILE:
                    {
                        pool->submit(delFile, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::JOIN_PROJECT:
                    {
                        pool->submit(joinProject, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::HEART_PCK:
                    {
                        Package pck("", (int)Package::ReturnType::HEART_PCK, 0);
                        write(sock_fd, pck.getPdata(), pck.getSize());
                        delete[] data;
                        break;
                    }
                    case (int)Package::PackageType::PRIVILEGE_QUERY:
                    {
                        pool->submit(privilegeQuery, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::PRIVILEGE_UPDATE:
                    {
                        pool->submit(privilegeUpdate, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::RUN_PROJECT:
                    {
                        pool->submit(runProject, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::POST_STDIN:
                    {
                        pool->submit(stdinToProject, sock_fd, data,packageSize);
                        break;
                    }
                    case (int)Package::PackageType::KILL_PROJECT:
                    {
                        pool->submit(killProjectP, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::DUBUG_PROJECT:
                    {
                        pool->submit(debugProject, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::KILL_PROJECT_FORCE:
                    {
                        pool->submit(killProjectForce, sock_fd, data);
                        break;
                    }
                    case (int)Package::PackageType::REGISTER:
                    {
                        pool->submit(userRegister, sock_fd, data);
                        break;
                    }
                    default:
                        INFO_LOG(m_logger, "UNKNOW PACKAGETYPE");
                        delete[] data;
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
    {
        auto result = sql->exeSql("select user_id from User where user_id = \"" + UserId + "\" and user_password = md5(\"" + list[1] + "\");");
        sqlResultRows vec = sql->getRows(result);
        if (vec.empty())
        {
            std::string error = "The account number or password is incorrect";
            Package pck(error.c_str(), (int)Package::ReturnType::SERVER_ERROR, error.size());
            write(sock_fd, pck.getPdata(), pck.getSize());
        }
        else
        {
            bool flag = true;
            for (auto it : *userMap)
            {
                if (it.second.userId == UserId)
                {
                    std::string str = "The user is logged in";
                    Package pck(str.c_str(), (int)Package::ReturnType::SERVER_ERROR, str.size());
                    write(sock_fd, pck.getPdata(), pck.getSize());
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                Package pck("", (int)Package::ReturnType::SERVER_ALLOW, 0);
                write(sock_fd, pck.getPdata(), pck.getSize());
                UserInfo userinfo;
                userinfo.openedFiles = std::vector<int>();
                userinfo.openedProjects = std::vector<int>();
                userinfo.userId = UserId;
                userMap->insert(std::pair<int, TcpServer::UserInfo>(sock_fd, userinfo));
            }
        }
    }

    delete[] data;
}

void TcpServer::sendProjectInfo(int sock_fd, char* buf)
{
    std::string projId = std::string(buf);

    int pid = std::stoi(projId);
    std::pair<int, int> p(pid, sock_fd);
    project_map->insert(p);
    auto userinfo = userMap->find(sock_fd)->second;
    userinfo.openedProjects.push_back(pid);

    auto projInfoRes = sql->exeSql("select pro_id from Project where pro_id = " + projId + ";");
    auto projInfo = sql->getRows(projInfoRes);
    auto priRes = sql->exeSql("select _privilege_level from Privilege where _pro_id = " + projId + " and _user_id = \"" + userinfo.userId + "\";");
    auto prirow = sql->getRows(priRes);
    int privilege = std::stoi(prirow[0][0]);
    MYSQL_RES* projFileRes;
    switch (privilege)
    {
    case 0:
    case 1:
    case 2:
    {
        projFileRes = sql->exeSql("select * from File where file_project = " + projId + ";");
        break;
    }
    case 3:
    {
        projFileRes = sql->exeSql("select * from File where file_project = " + projId + " and file_privilege > 0;");
        break;
    }
    case 4:
    {
        projFileRes = sql->exeSql("select * from File where file_project = " + projId + " and file_privilege > 1;");
        break;
    }
    }
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

    Package pck(data.c_str(), (int)Package::ReturnType::PROJ_FILE_INFO, data.size());
    write(sock_fd, pck.getPdata(), pck.getSize());
    mysql_free_result(priRes);
    mysql_free_result(projFileRes);
    delete buf;
}

void TcpServer::initUserProjects(int sock_fd)
{
    std::string data = "";

    std::string userId = userMap->find(sock_fd)->second.userId;
    auto userProjRes = sql->exeSql("select Project.pro_id,Project.pro_name,Project.pro_owner,Privilege._privilege_level,Project.pro_uuid from Project inner join Privilege\
                                     where (Privilege._user_id = \""+userId+"\" and Privilege._pro_id = Project.pro_id);");
    sqlResultRows rows = sql->getRows(userProjRes);
    for (auto i : rows)
    {
        data += std::string(i[0]) + "\t" + std::string(i[1]) + "\t" + std::string(i[2]) + "\t" + std::string(i[3]) + "\t" + std::string(i[4]) + "\n";
    }
    Package pck(data.c_str(), (int)Package::ReturnType::USER_PROJS,data.size());
    write(sock_fd, pck.getPdata(), pck.getSize());

    mysql_free_result(userProjRes);
}

void TcpServer::newProject(int sock_fd, char* data)
{
    std::string proName(data);
    std::string userpath = "./" + userMap->find(sock_fd)->second.userId;
    std::string path = userpath + "/" + proName;
    std::string userId = userMap->find(sock_fd)->second.userId;
    if (!opendir(userpath.c_str()))
    {
        mkdir(userpath.c_str(),0755);
    }
    mkdir(path.c_str(),0755);

    int fd = open((path + "/CMakeLists.txt").c_str(), O_RDWR | O_CREAT, 0755);

    if (fd != -1) {
        std::fstream fileStream(path + "/CMakeLists.txt", std::ios::out | std::ios::in | std::ios::trunc);
        if (fileStream.is_open())
        {
            fileStream << "cmake_minimum_required(VERSION 3.9)\n";
            fileStream << "project(" << proName << ")\n";
            fileStream << "set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)\n";
            fileStream << "set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)\n";
            fileStream << "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)\n";
            fileStream << "SET(CMAKE_BUILD_TYPE \"Debug\")\n";
            fileStream << "SET(CMAKE_CXX_FLAGS_DEBUG \"$ENV{ CXXFLAGS } -O0 -Wall -g2 -ggdb\")\n";
            fileStream << "SET(CMAKE_CXX_FLAGS_RELEASE \"$ENV{ CXXFLAGS } -O3 -Wall\")\n";
            fileStream << "add_executable(" << proName << " ${SOURCES})\n";
            fileStream.close();
        }
        // 关闭文件描述符
        close(fd);
    }

    uuid_t uuid;
    char uuidStr[36];
    uuid_generate(uuid);
    uuid_unparse(uuid, uuidStr);

    sql->exeSql("insert into Project (pro_name,pro_owner,pro_uuid) value (\"" + proName + "\",\"" + userId + "\",\"" + uuidStr +  "\")");
    auto res = sql->exeSql("select pro_id,pro_name,pro_owner,pro_uuid from Project where pro_name =  \"" + proName + "\";");
    sqlResultRows rows = sql->getRows(res);
    sql->exeSql("insert into Privilege (_user_id,_pro_id,_privilege_level) values(\"" + userId + "\"," + std::string(rows[0][0]) + ",0);");
    std::string str = std::string(rows[0][0]) + "\t" + std::string(rows[0][1]) + "\t" + std::string(rows[0][2]) + "\t" + std::string(rows[0][3]);
    Package pck(str.c_str(), (int)Package::ReturnType::NEW_PROJ_INFO, str.size());
    write(sock_fd, pck.getPdata(), pck.getSize());

    sql->exeSql("insert into File (file_name,file_user,file_path,file_project,file_privilege) \
        values(\"CMakeLists.txt\",\"" + userId + "\",\"" + (path + "/CMakeLists.txt") + "\", " + rows[0][0] + ", 3); ");

    mysql_free_result(res);
    delete[] data;
}

void TcpServer::delProject(int sock_fd, char* data)
{
    std::string userId = userMap->find(sock_fd)->second.userId;
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

    project_map->erase(std::stoi(list[0]));

    delete[] data;
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
            Package pck(buffer, (int)Package::ReturnType::FILE, ret+4);
            write(sock_fd, pck.getPdata(), pck.getSize());
        }

        Package pck(temp.c_str(), (int)Package::ReturnType::FILE_TRANSOVER, temp.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
    }

    int num = file_map->count(fid);
    auto it = file_map->find(fid);
    bool flag = true;
    for (; num > 0; it++, num--)
    {
        int fd = it->second;
        if (fd == sock_fd)
        {
            flag = false;
            break;
        }
    }
    if (flag)
    {
        std::pair<int, int> p(fid, sock_fd);
        file_map->insert(p);
    }
    userMap->find(sock_fd)->second.openedFiles.push_back(fid);

    mysql_free_result(res);
    delete[] data;
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
        // 关闭文件描述符
        close(fd);
    }

    std::string userId = userMap->find(sock_fd)->second.userId;

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

    int pro_id = std::stoi(list[2]);
    int pnum = project_map->count(pro_id);
    auto it = project_map->find(pro_id);
    for (; pnum >0; pnum--,it++)
    {
        int fd = it->second;
        Package pck(buf.c_str(), (int)Package::ReturnType::NEW_FILE_INFO, buf.size());
        write(fd, pck.getPdata(), pck.getSize());
    }

    mysql_free_result(res);
    delete[] data;
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
    }
    //向打开该项目的参与者发送文件变更信息
    std::string buffer = list[2] + "\t" + list[0];
    Package pck(buffer.c_str(), (int)Package::ReturnType::PROJECT_FILE_DELETE, list[0].size());
    int num = project_map->count(pid);
    auto it = project_map->find(pid);
    for (; num > 0; num--, it++)
    {
        int fd = it->second;
        write(fd, pck.getPdata(), pck.getSize());
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

    file_map->erase(fid);

    delete[] data;
}

void TcpServer::joinProject(int sock_fd, char* data)
{
    std::string uuid(data);
    std::string userId = userMap->find(sock_fd)->second.userId;
    auto pro_res = sql->exeSql("select pro_id,pro_name,pro_owner,pro_uuid from Project where pro_uuid = \"" + uuid + "\";");
    auto pro_row = sql->getRows(pro_res);
    auto res = sql->exeSql("select * from Privilege where _user_id = \"" + userId + "\" and _pro_id = " + pro_row[0][0] + ";");
    auto row = sql->getRows(res);
    if (pro_row.size() == 0)
    {
        std::string str = "The project does not exist!";
        Package pck(str.c_str(), (int)Package::ReturnType::SERVER_ERROR, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
    }
    else if (row.size() != 0)
    {
        std::string str = "You have already joined the project, please do not join it repeatedly!";
        Package pck(str.c_str(), (int)Package::ReturnType::SERVER_ERROR, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
    }
    else
    {
        sql->exeSql("insert into Privilege (_user_id,_pro_id,_privilege_level) values (\""+userId+"\","+pro_row[0][0]+",4);");

        std::string data = std::string(pro_row[0][0]) + "\t" + std::string(pro_row[0][1]) + "\t" + std::string(pro_row[0][2]) + "\t" + std::string(pro_row[0][3]);
        Package pck(data.c_str(), (int)Package::ReturnType::NEW_PROJ_INFO, data.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
    }

    mysql_free_result(pro_res);
    mysql_free_result(res);
    delete[] data;
}

void TcpServer::sendTextChange(int sock_fd, char* data)
{
    std::string buf(data);
    stringList list;
    stringSplit(buf, "#", list,8);
    int file_id = std::stoi(list[0]);
    int started = std::stoi(list[2]);
    int charRemoved = std::stoi(list[5]);
    std::string path = list[3];

    if (!textChange(path, started, charRemoved, list[7]))
    {
        ERROR_LOG(m_logger, "edit file error");
    }

    int num = file_map->count(file_id);
    auto it = file_map->find(file_id);

    for (; num>0; it++,num--)
    {
        int fd = it->second;
        if (fd != sock_fd)
        {
            Package pck(data, (int)Package::ReturnType::TEXT_CHANGE, buf.size());
            write(fd, pck.getPdata(), pck.getSize());
        }
    }

    delete[] data;
}

void TcpServer::privilegeQuery(int sock_fd, char* data)
{
    auto res = sql->exeSql("select _user_id,_privilege_level from Privilege where _pro_id = " + std::string(data) + ";");
    auto rows = sql->getRows(res);
    std::string userId = userMap->find(sock_fd)->second.userId;
    std::string buf = std::string(data) + "\n";
    for (auto it : rows)
    {
        if (!(it[0] == userId))
        {
            buf += std::string(it[0]) + "\t" + std::string(it[1]) + "\n";
        }
    }

    Package pck(buf.c_str(), (int)Package::ReturnType::PRIVILEGE_INFO, buf.size());
    write(sock_fd, pck.getPdata(), pck.getSize());

    mysql_free_result(res);
    delete[] data;
}

void TcpServer::privilegeUpdate(int sock_fd, char* data)
{
    stringList list;
    stringSplit(std::string(data), "\n", list);

    for (int i = 1; i < list.size(); i++)
    {
        if (list[i].empty())
            continue;
        stringList info;
        stringSplit(list[i], "\t", info);
        sql->exeSql("update Privilege set _privilege_level = " + info[1] + " where _user_id = \"" + info[0] + "\" and _pro_id = " + list[0] + ";");
    }

    std::string str = "The permission update was successful!";
    Package pck(str.c_str(), (int)Package::ReturnType::SERVER_OK, str.size());
    write(sock_fd, pck.getPdata(), pck.getSize());

    delete[] data;
}

void TcpServer::runProject(int sock_fd, char* data)
{
    std::string proId(data);

    auto res = sql->exeSql("select pro_name,pro_owner from Project where pro_id = " + proId + ";");
    auto rows = sql->getRows(res);
    std::string proPath = "./" + std::string(rows[0][1]) +"/" + std::string(rows[0][0]);
    std::string buildPath = proPath + "/build";

    compileProject(sock_fd, proPath, buildPath);

    Package pck("", (int)Package::ReturnType::BUILD_FINISH, 0);
    write(sock_fd, pck.getPdata(), pck.getSize());
    //读取进程的pipe
    int outfd[2];

    //向进程输入
    int infd[2];

    if (pipe(outfd) == -1)
    {
        std::string str = "Server create pipe error!";
        Package pck(str.c_str(), (int)Package::ReturnType::SERVER_ERROR, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
        ERROR_LOG(m_logger, "pipe error");
        return;
    }
    int subWfd = outfd[1];
    int mainRfd = outfd[0];

    if (pipe(infd) == -1)
    {
        std::string str = "Server create pipe error!";
        Package pck(str.c_str(), (int)Package::ReturnType::SERVER_ERROR, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
        ERROR_LOG(m_logger, "pipe error");
        return;
    }
    int mainWfd = infd[1];
    int subRfd = infd[0];

    char buf[1024];
    memset(buf, 0, sizeof(buf));

    projectInData->erase(proId);
    projectInData->insert(std::pair<std::string, int>(proId, mainWfd));

    int pid = fork();
    if (pid == -1)
    {
        ERROR_LOG(m_logger, "fork error");
    }
    else if (pid == 0) //子进程
    {
        setpgrp();
        //运行用户项目代码

        //重定向输入输出流
        close(STDOUT_FILENO);
        close(STDIN_FILENO);
        int saveOutfd = dup2(subWfd, STDOUT_FILENO);
        int saveInfd = dup2(subRfd, STDIN_FILENO);

        std::string exePath = buildPath + "/bin/" + std::string(rows[0][0]);
        std::string cmdstr = "find "+ buildPath + " -iname " + std::string(rows[0][0]) + " -exec {} \\; ";
        /*if (execlp(exePath.c_str(),NULL) == -1)
        {
            std::string str = "run project error";
            Package pck(str.c_str(), (int)Package::ReturnType::SERVER_ERROR, str.size());
            write(sock_fd, pck.getPdata(), pck.getSize());
        }*/
        int ret = system(cmdstr.c_str());
        std::string str = "Program exited with code "+ std::to_string(ret);
        Package pck(str.c_str(), (int)Package::ReturnType::RUN_FINISH, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());

        projectPidMap->erase(proId); 
        close(mainRfd);
        close(subWfd);
        _Exit(-1);
    }
    else
    {
        userMap->find(sock_fd)->second.runningPid = pid;
        //服务器代码
        std::vector<int> vec = { pid,subWfd,mainRfd };
        projectPidMap->insert(std::pair<std::string, std::vector<int>>(proId, vec));
        int ret = 0;

        while ((ret = read(mainRfd, buf, 1024)) != NULL)
        {
            if (projectPidMap->find(proId) != projectPidMap->end())
            {
                Package pck(buf, (int)Package::ReturnType::RUN_INFO, ret);
                write(sock_fd, pck.getPdata(), pck.getSize());
                memset(buf, 0, sizeof(buf));
            }
            else
                break;
        }

        //waitpid(pid, &ret, 0);
        wait(&ret);
        projectPidMap->erase(proId);
        userMap->find(sock_fd)->second.runningPid = -1;
    }

    close(subRfd);
    close(mainWfd);
    mysql_free_result(res);
    delete[] data;
}

void TcpServer::stdinToProject(int sock_fd, char* data,int size)
{
    std::string proId = std::to_string(bytesToInt(data, 0, 4));
    if (projectInData->find(proId) != projectInData->end())
    {
        int proStdin = projectInData->find(proId)->second;
        write(proStdin, data + 4, size - 4);
    }
    delete data;
}

void TcpServer::compileProject(int sock_fd,std::string proPath, std::string buildPath)
{
    removeFile(buildPath);
    CreateDir(buildPath);

    FILE* fp = NULL;
    char buf[1024];
    memset(buf, 0, sizeof(buf));

    std::string cmakeCmd = "cmake " + proPath + " -B " + buildPath + " 2>&1";
    if ((fp = popen(cmakeCmd.c_str(), "r")) != NULL)
    {
        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
            Package pck(buf, (int)Package::ReturnType::BUILD_INFO, 1024);
            write(sock_fd, pck.getPdata(), pck.getSize());
            memset(buf, 0, sizeof(buf));
        }
        pclose(fp);
    }

    std::string makeCmd = "make -C " + buildPath + " 2>&1";
    if ((fp = popen(makeCmd.c_str(), "r")) != NULL)
    {
        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
            Package pck(buf, (int)Package::ReturnType::BUILD_INFO, 1024);
            write(sock_fd, pck.getPdata(), pck.getSize());
            memset(buf, 0, sizeof(buf));
        }
        pclose(fp);
    }
}

void TcpServer::killProjectP(int sock_fd, char* data)
{
    std::string proId(data);
    if (projectPidMap->find(proId) != projectPidMap->end())
    {
        
        auto res = projectPidMap->find(proId)->second;
        close(projectPidMap->find(proId)->second[1]);
        close(projectPidMap->find(proId)->second[2]);
        if (killpg(projectPidMap->find(proId)->second[0], SIGKILL) == -1)
        {
            ERROR_LOG(m_logger, "kill p error");
        }

        int ret = 0;
        waitpid(projectPidMap->find(proId)->second[0],&ret,0);
        std::string str = "Program exited with code " + std::to_string(ret);
        Package pck(str.c_str(), (int)Package::ReturnType::RUN_FINISH, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());

        projectPidMap->erase(proId);
    }

    delete[] data;
}

void TcpServer::killProjectForce(int sock_fd, char* data)
{
    std::string proId(data);
    if (projectPidMap->find(proId) != projectPidMap->end())
    {
        auto res = projectPidMap->find(proId)->second;
        close(projectPidMap->find(proId)->second[1]);
        close(projectPidMap->find(proId)->second[2]);
        if (killpg(projectPidMap->find(proId)->second[0], SIGKILL) == -1)
        {
            ERROR_LOG(m_logger, "kill p error");
        }

        int ret = 0;
        waitpid(projectPidMap->find(proId)->second[0], &ret, 0);

        projectPidMap->erase(proId);
    }

    delete[] data;
}

void TcpServer::debugProject(int sock_fd, char* data)
{
    std::string proId = std::string(data);

    auto res = sql->exeSql("select pro_name,pro_owner from Project where pro_id = " + proId + ";");
    auto rows = sql->getRows(res);
    std::string proPath = "./" + std::string(rows[0][1]) + "/" + std::string(rows[0][0]);
    std::string buildPath = proPath + "/build";

    compileProject(sock_fd,proPath,buildPath);

    Package pck("", (int)Package::ReturnType::BUILD_FINISH, 0);
    write(sock_fd, pck.getPdata(), pck.getSize());
    //读取进程的pipe
    int outfd[2];

    //向进程输入
    int infd[2];

    if (pipe(outfd) == -1)
    {
        std::string str = "Server create pipe error!";
        Package pck(str.c_str(), (int)Package::ReturnType::SERVER_ERROR, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
        ERROR_LOG(m_logger, "pipe error");
        return;
    }
    int subWfd = outfd[1];
    int mainRfd = outfd[0];

    if (pipe(infd) == -1)
    {
        std::string str = "Server create pipe error!";
        Package pck(str.c_str(), (int)Package::ReturnType::SERVER_ERROR, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
        ERROR_LOG(m_logger, "pipe error");
        return;
    }
    int mainWfd = infd[1];
    int subRfd = infd[0];

    char buf[4096];
    memset(buf, 0, sizeof(buf));

    projectInData->erase(proId);
    projectInData->insert(std::pair<std::string, int>(proId, mainWfd));

    int pid = fork();
    if (pid == -1)
    {
        ERROR_LOG(m_logger, "fork error");
    }
    else if (pid == 0) //子进程
    {
        setpgrp();
        //运行用户项目代码

        //重定向输入输出流
        close(STDOUT_FILENO);
        close(STDIN_FILENO);
        int saveOutfd = dup2(subWfd, STDOUT_FILENO);
        int saveInfd = dup2(subRfd, STDIN_FILENO);

        std::string cmdstr = "find " + buildPath + " -iname " + std::string(rows[0][0]) + " -exec gdb {} -silent \\; ";
        /*if (execlp(exePath.c_str(),NULL) == -1)
        {
            std::string str = "run project error";
            Package pck(str.c_str(), (int)Package::ReturnType::SERVER_ERROR, str.size());
            write(sock_fd, pck.getPdata(), pck.getSize());
        }*/
        int ret = system(cmdstr.c_str());
        std::string str = "debug exited with code " + std::to_string(ret);
        Package pck(str.c_str(), (int)Package::ReturnType::RUN_FINISH, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());

        close(mainRfd);
        close(subWfd);
        _Exit(-1);
    }
    else
    {
        userMap->find(sock_fd)->second.runningPid = pid;
        //服务器代码
        std::vector<int> vec = { pid,subWfd,mainRfd };
        projectPidMap->insert(std::pair<std::string, std::vector<int>>(proId, vec));
        int ret = 0;
        //read(mainRfd, buf, 4096);
        //for (int i = 0; i < bpNum; i++)
        //{
        //    std::string cmd = "b " + list[i+1] + "\n";
        //    write(mainWfd, cmd.c_str(), cmd.size());
        //    ret = read(mainRfd, buf, 4096);
        //    Package pck(buf, (int)Package::ReturnType::BREAK_POINTS_INFO, ret);
        //    write(sock_fd, pck.getPdata(), pck.getSize());
        //    memset(buf, 0, sizeof(buf));
        //}

        while ((ret = read(mainRfd, buf, 4096)) != NULL)
        {
            if (projectPidMap->find(proId) != projectPidMap->end())
            {
                Package pck(buf, (int)Package::ReturnType::DUBUG_INFO, ret);
                write(sock_fd, pck.getPdata(), pck.getSize());
                memset(buf, 0, sizeof(buf));
            }
            else
                break;
        }

        waitpid(pid, &ret, 0);

        std::string str = "Program exited with code " + std::to_string(ret);
        Package pck(str.c_str(), (int)Package::ReturnType::RUN_FINISH, str.size());
        write(sock_fd, pck.getPdata(), pck.getSize());

        if(userMap->find(sock_fd)!=userMap->end())
            userMap->find(sock_fd)->second.runningPid = -1;
        projectPidMap->erase(proId);
    }

    close(subRfd);
    close(mainWfd);

    delete[] data;
}

void TcpServer::userRegister(int sock_fd, char* data)
{
    std::string temp(data);
    stringList list;
    stringSplit(temp, "\t", list);
    auto res = sql->exeSql("select 1 from User where user_id =\"" + list[0] + "\" limit 1;");
    auto rows = sql->getRows(res);
    if (rows.empty())
    {
        sql->exeSql("insert into User (user_id,user_password) values (\"" + list[0] + "\",md5(\"" + list[1] + "\"));");
        Package pck("", (int)Package::ReturnType::REGISTER_OK, 0);
        write(sock_fd, pck.getPdata(), pck.getSize());
    }
    else
    {
        std::string error = "The user ID already exists!";
        Package pck(error.c_str(), (int)Package::ReturnType::REGISTER_ERROR, error.size());
        write(sock_fd, pck.getPdata(), pck.getSize());
    }

    mysql_free_result(res);
    delete[] data;
}