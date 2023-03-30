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
//#include"File.h"
#include"threadPool.h"

#define SqlIP "192.168.239.129"
class TcpServer
{
public:
    TcpServer(const char* ip, uint32_t port, uint32_t fileport, Log::Logger logger);
    virtual ~TcpServer();

public:
    void tcpStart();
    void dataProcess(int fd, char* data, int size);
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

    //static void recvFile(int fd, HizZ::File* file);

    static void sendFile(int sock_fd, std::string path, int fileSize);

    //static void fileServerStart();
};
#endif // TCPSERVER_H


