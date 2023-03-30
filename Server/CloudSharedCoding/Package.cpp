#include "Package.h"
#include"Tool.h"

Package::Package(const char* data, int type, int size)
{
    p_type = type;
    p_size = size + 8;
    char buf[size + 8];

    char temp1[4];
    intToBytes(type, temp1, 0, sizeof(temp1));
    char temp2[4];
    intToBytes(size, temp2, 0, sizeof(temp2));

    memcpy(buf, temp1, 4);
    memcpy(buf + 4, temp2, 4);
    if (size > 0)
        memcpy(buf + 8, data, size);

    p_data = buf;
}

Package::~Package()
{

}
