#ifndef TOOL_H_INCLUDED
#define TOOL_H_INCLUDED

#include"Log.h"
#include<cstring>
#include<unistd.h>
#include <sys/stat.h>
#include<dirent.h>
#include<fstream>
#include<unistd.h>

static Log::Logger ToolLog("ToolLog");
static void ERROR_CHECK(int ret, std::string str, Log::Logger logger)
{
    if (ret < 0)
    {
        ERROR_LOG(logger, str);
    }
};

static void DEBUG_CHECK(int ret, std::string str, Log::Logger logger)
{
    if (ret < 0)
    {
        DEBUG_LOG(logger, str);
    }
};

static void INFO_CHECK(int ret, std::string str, Log::Logger logger)
{
    if (ret < 0)
    {
        INFO_LOG(logger, str);
    }
};

static void WARN_CHECK(int ret, std::string str, Log::Logger logger)
{
    if (ret < 0)
    {
        WARN_LOG(logger, str);
    }
};

static void FATAL_CHECK(int ret, std::string str, Log::Logger logger)
{
    if (ret < 0)
    {
        FATAL_LOG(logger, str);
    }
};

static void intToBytes(int num, char* arr, int offset, int size)
{
    if (size < 4)
    {
        ERROR_LOG(ToolLog, "An array of length >=4 is required");
        return;
    }
    if ((offset + 3) >= size || offset < 0)
    {
        ERROR_LOG(ToolLog, "Access overflow");
        return;
    }
    arr[offset] = (unsigned char)(0x000000ff & num);
    arr[offset + 1] = (unsigned char)((0x0000ff00 & num) >> 8);
    arr[offset + 2] = (unsigned char)((0x00ff0000 & num) >> 16);
    arr[offset + 3] = (unsigned char)((0xff000000 & num) >> 24);
}

static int bytesToInt(char* bytes, int offset, int size)
{
    if (offset + 3 >= size)
    {
        ERROR_LOG(ToolLog, "Array access could out of bounds");
        return 0;
    }
    int addr = (int)bytes[offset] & 0x000000ff;
    addr |= ((int)bytes[offset + 1] << 8) & 0x0000ff00;
    addr |= ((int)bytes[offset + 2] << 16) & 0x00ff0000;
    addr |= ((int)bytes[offset + 3] << 24) & 0xff000000;
    return addr;
}

static void stringSplit(std::string str, std::string split, std::vector<std::string>& res,int splitNum = -1)
{
    int index = str.find(split, 0);
    if (index == std::string::npos)
    {
        res.push_back(str);
        return;
    }
    int n = 0;
    int num = 0;
    while (index != std::string::npos && num != splitNum-1)
    {
        res.push_back(str.substr(n, index - n));
        num++;
        n = index + 1;
        index = str.find(split, index + 1);
    }
    //if (index != str.size() - 1)
        res.push_back(str.substr(n, str.size() - 1));
}

static bool removeFile(std::string& dirName)
{
    DIR* dir;
    struct dirent* dirinfo;
    struct stat statbuf;
    char filepath[256] = { 0 };
    lstat(dirName.c_str(), &statbuf);


    if (S_ISREG(statbuf.st_mode)) // 是否是文件
    {
        remove(dirName.c_str());
    }
    else if (S_ISDIR(statbuf.st_mode)) // 是否是目录
    {
        if ((dir = opendir(dirName.c_str())) == NULL)
        {
            return false;
        }

        while ((dirinfo = readdir(dir)) != NULL)
        {
            if (strcmp(dirinfo->d_name, ".") == 0 || strcmp(dirinfo->d_name, "..") == 0) // 是否是特殊目录
                continue;

            std::string absPath = dirName + "/" + std::string(dirinfo->d_name);
            removeFile(absPath);
        }
        closedir(dir);

        rmdir(dirName.c_str());
    }
    else
    {
        return false;
    }

    return true;
}

static std::string getParentDir(const std::string path)
{
    std::string pdir = path;
    if (pdir.length() < 1) {
        return "";
    }
    while (pdir.length() > 1 && (pdir[pdir.length() - 1] == '/')) pdir = pdir.substr(0, pdir.length() - 1);

    pdir = pdir.substr(0, pdir.find_last_of('/'));
    return pdir;
}

static int CreateDir(const std::string dir)
{
    int ret = 0;
    if (dir.empty())
        return -1;
    if (!access(dir.c_str(), F_OK))
        return ret;
    std::string pdir = "";
    if ((ret = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)))
    {
        pdir = getParentDir(dir);
        if ((ret = CreateDir(pdir)) == 0)
        {
            ret = CreateDir(dir);
        }
    }
    return ret;
}

static void pos(const std::string& T, const std::string& P, int lineNumber,int deleteLine,int &start_pos,int &end_pos) {
    if (lineNumber == 0) {
        start_pos = 0;
    }
    int count = 0;
    for (int i = 0; i < T.size(); i++) {
        if (T[i] == '\n') {
            count++;
            if (count == lineNumber) {
                start_pos = i+1;
            }
            if (count == lineNumber + deleteLine + 1) {
                end_pos = i;
            }
        }
    }
    if (count < lineNumber + deleteLine + 1) {
        end_pos = T.size() - 1;
    }
}

static bool textChange(std::string path,int started, int charRemoved, std::string text)
{
    std::fstream fs(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!fs.is_open()) {
        return false;
    }
    struct stat stbuf;
    stat(path.c_str(), &stbuf);

    int length = stbuf.st_size;

    char* buffer = new char[length + 1];
    fs.seekg(0, std::ios::beg);
    fs.read(buffer, length);
    fs.close();
    buffer[length] = '\0';
    std::string buf(buffer);

    std::string substr1 = buf.substr(0, started);

    std::string substr2 = "";
    if (started+charRemoved<buf.size())
    {
        substr2 = buf.substr(started + charRemoved);
    }
    buf = substr1 + text +substr2;
    delete[]buffer;

    std::ofstream ofs(path);
    if (!ofs.is_open())
        return false;
    ofs.write(buf.c_str(), buf.size());
    ofs.close();
    return true;
}

static std::string readLine(int fd)
{
    std::string ret;
    char p;
    while ((read(fd,&p,1))!=NULL)
    {
        if (p == '\n' || p == EOF)
        {
            break;
        }
        ret += p;
    }

    return ret;
}

#endif // TOOL_H_INCLUDED
