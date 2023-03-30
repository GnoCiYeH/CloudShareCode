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

//void TcpServer::recvFile(int fd, HizZ::File* file)
//{
//    int recvSize = 0;
//    int fileSize = file->getSize();
//    std::ofstream ofs;
//    ofs.open(file->getPath(), std::ios::out | std::ios::app | std::ios::binary);
//    if (!ofs.is_open())
//    {
//        perror("open file error");
//        return;
//    }
//    while (file->getOffset() != fileSize)
//    {
//        char data[4096];
//        memset(data, 0, sizeof(data));
//        recvSize = read(fd, data, sizeof(data));
//        if (recvSize == 0)
//        {
//            file->submitFile();
//            ofs.close();
//            delete file;
//            return;
//        }
//        ofs.write(data, recvSize);
//        file->addOffset(recvSize);
//    }
//
//    file->submitFile();
//
//    std::string filedata = "";
//    filedata += file->getName() + "\t";
//    MYSQL_RES* result = sql->exeSql("select f_id from File where f_path = \"" + file->getPath() + "\";");
//    std::string fid(sql->getRows(result)[0][0]);
//    filedata += fid + "\t";
//    filedata += std::to_string(file->getParentID()) + "\t";
//    filedata += file->getUploadTime() + "\t";
//    filedata += std::to_string(file->getSize());
//
//    Package pck(filedata.c_str(), Package::COMPLETE, filedata.size());
//    write(fd, pck.getPdata(), pck.getSize());
//    ofs.close();
//    delete file;
//}

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
                    dataProcess(sock_fd, buf, sizeof(buf));
                }
            }
        }
    }
}

void TcpServer::dataProcess(int sock_fd, char* buf, int size)
{
    int type = bytesToInt(buf, 0, sizeof(buf));
    int packageSize = bytesToInt(buf, 4, sizeof(buf));

    /*switch (type)
    {
    case Package::Type:::
        break;
    default:
        INFO_LOG(m_logger, "UNKNOW PACKAGETYPE");
        break;
    }*/
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
