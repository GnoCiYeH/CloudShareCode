#ifndef TCPSERVER_H
#define TCPSERVER_H

#include"Log.h"
#include"Tool.h"
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>
#include"SqlTool.h"
#include"Package.h"
#include<unordered_map>
#include"ThreadPool.h"
#include<vector>
#include<map>

#define SqlIP "192.168.239.129"
class TcpServer
{
public:
    TcpServer(const char* ip, uint32_t port, Log::Logger& logger);
    virtual ~TcpServer();
    typedef std::vector<std::string> stringList;
    typedef std::vector<MYSQL_ROW> sqlResultRows;
public:
    void tcpStart();
    std::string SqlIp;
    static SqlTool* sql;

private:
    int m_sock_fd;
    struct sockaddr_in m_server_addr;
    static Log::Logger m_logger;
    const char* m_log_filePath;
    int m_epfd;

    ThreadPool* pool;

    static void newProject(int sock_fd,char* data);

    static void initUserProjects(int sock_fd);

    static void sendProjectInfo(int sock_fd, char* data);

    static void delProject(int sock_fd,char* data);

    static void sendFile(int sock_fd, char* data);

    static void login(int sock_fd, char* data);

    static void newFile(int sock_fd, char* data);

    static void delFile(int sock_fd, char* data);

    static void joinProject(int sock_fd, char* data);

    static void sendTextChange(int sock_fd, char* data);

    static void privilegeQuery(int sock_fd, char* data);

    static void privilegeUpdate(int sock_fd, char* data);

    struct UserInfo
    {
        std::string userId;
        std::vector<int> openedFiles;
        std::vector<int> openedProjects;
    };

    static std::unordered_map<int, UserInfo>* userMap;

    static std::multimap<int, int>* file_map;

    static std::multimap<int, int>* project_map;

};
#endif // TCPSERVER_H


