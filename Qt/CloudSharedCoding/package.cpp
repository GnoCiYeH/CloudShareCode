#include "package.h"
#include<QDebug>

Package::Package(QByteArray data,int type)
{
    int dsize = data.size();
    p_size = dsize+8;

    p_data.append(intToByteArr(type));
    p_data.append(intToByteArr(dsize));
    p_data.append(data);
}

QByteArray Package::intToByteArr(int num)
{
    QByteArray arr;
    arr.resize(4);
    arr[0] = (unsigned char)(0x000000ff & num);
    arr[1] = (unsigned char)((0x0000ff00 & num)>>8);
    arr[2] = (unsigned char)((0x00ff0000 & num)>>16);
    arr[3] = (unsigned char)((0xff000000 & num)>>24);
    return arr;
}

int Package::ByteArrToInt(QByteArray bytes,int offset)
{
    int addr = (int)bytes[offset] & 0x000000ff;
    addr |= ((int)bytes[offset + 1] << 8) & 0x0000ff00;
    addr |= ((int)bytes[offset + 2] << 16) & 0x00ff0000;
    addr |= ((int)bytes[offset + 3] << 24) & 0xff000000;
    return addr;
}

Package::~Package()
{

}
