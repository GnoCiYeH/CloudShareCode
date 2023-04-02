#ifndef TOOL_H_INCLUDED
#define TOOL_H_INCLUDED

#include"Log.h"
#include<cstring>
#include<unistd.h>
#include <sys/stat.h>
#include<dirent.h>

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
    int addr = (int)bytes[offset];
    addr |= ((int)bytes[offset + 1] << 8);
    addr |= ((int)bytes[offset + 2] << 16);
    addr |= ((int)bytes[offset + 3] << 24);
    return addr;
}

static void stringSplit(std::string str, std::string split, std::vector<std::string>& res)
{
    int index = str.find(split, 0);
    if (index == std::string::npos)
    {
        res.push_back(str);
        return;
    }
    int n = 0;
    while (index != std::string::npos)
    {
        res.push_back(str.substr(n, index - n));
        n = index + 1;
        index = str.find(split, index + 1);
    }
    if (index != str.size() - 1)
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

#endif // TOOL_H_INCLUDED
