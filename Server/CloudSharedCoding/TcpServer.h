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
    //int m_file_sock_fd;
    struct sockaddr_in m_server_addr;
    //struct sockaddr_in m_file_server_addr;
    Log::Logger m_logger;
    const char* m_log_filePath;
    int m_epfd;
    //int m_file_epid;

    ThreadPool* pool;

    static void sendDir(int sock_fd, std::string* UserId);

    static void sendFile(int sock_fd, std::string path, int fileSize);

    static void login(int sock_fd, int packageSize);

    static std::unordered_map<int, std::string>* userMap;
};
#endif // TCPSERVER_H


