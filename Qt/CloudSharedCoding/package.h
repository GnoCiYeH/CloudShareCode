#ifndef PACKAGE_H
#define PACKAGE_H

#include<QByteArray>
/*
    *该类作为客户端与服务器通信的协议
    * 自带的构造函数将添加8字节的包信息在包头（包类型以及包大小）
    * 可自己定义不同包类型的数据格式，但应告诉服务器端该类型的数据处理格式（包类型必须在包头）
*/
class Package
{
public:
    //不同数据段用特定的分割方式
    enum class PackageType{
        //请求登录，应携带用户id及密码数据 使用\n分割
        LOGIN = 0,

        //注册信号
        REGISTER = 1,

        /*
         * 在codeEdit->document 发出 void contentsChange(int position, int charsRemoved, int charsAdded) 信号时应及时向服务器发送该类型数据
         * 应携带改变的文本数据所在的文件id、用户id、文本改变的位置、改变的文本数据（该文本数据可能含有\n，所以不能使用\n分割）
         * 可使用4字节携带文件id，4字节携带用户id的size+用户id,4字节携带文本数据size+文本数据的格式
        */
        TEXT_CHANGE = 2,

        //从服务器获取项目，携带所需项目id
        GET_PROJECT = 3,

        //从服务器获取当前用户参与的所有项目基本信息 无需携带其他数据
        INIT_PROJS = 4,

        //提交保存信号，携带文件id及提交者信息（用户名）
        SUBMIT = 5,

        //上传项目保存至服务器端，携带文件名，文件大小，用户id，权限，后续应开辟线程发送项目文件
        POST_PROJECT = 6,

        //创建云项目
        NEW_PROJECT = 7,

        //删除项目,携带项目id
        DEL_PROJECT = 8,

        //获取文件
        GET_FILE = 9,

        //新建文件
        NEW_FILE = 10,

        //删除文件
        DEL_FILE = 11,

        //加入项目
        JOIN_PROJECT = 12,

        //心跳包
        HEART_PCK = 13,

        //项目权限查询
        PRIVILEGE_QUERY = 14
    };
    enum class ReturnType{
        SERVER_ALLOW = 0,
        SERVER_ERROR = 1,
        USER_PROJS = 2,
        PROJ_FILE_INFO = 3,
        NEW_PROJ_INFO = 4,
        FILE = 5,
        NEW_FILE_INFO = 6,
        PROJECT_FILE_DELETE = 7,
        TEXT_CHANGE = 8,
        HEART_PCK = 9,
        PRIVILEGE_INFO = 10
    };

    Package(QByteArray data,int type);
    ~Package();
public:
    //获取处理后的包数据
    QByteArray getPdata(){return p_data;}

    //将int类型的值转换为4字节字节数组
    static QByteArray intToByteArr(int num);

    /*
     * 将字节数组转换为int值
     *
     * Parameter arr : 字节数组
     * Parameter offset : 偏移量 若为0则将数组位置0后的四个字节数据转换为int
    */
    static int ByteArrToInt(QByteArray arr,int offset=0);

    //获取处理后的包数据大小
    int getSize(){return p_size;}
private:
    QByteArray p_data;
    int p_size;
};

#endif // PACKAGE_H
