#ifndef PACKAGE_H
#define PACKAGE_H


class Package
{
public:
    enum PackageType {
        //请求登录，应携带用户id及密码数据 使用\n分割
        LOGIN = 0,

        //注册信号
        REGISTER,

        /*
         * 在codeEdit->document 发出 void contentsChange(int position, int charsRemoved, int charsAdded) 信号时应及时向服务器发送该类型数据
         * 应携带改变的文本数据所在的文件id、用户id、文本改变的位置、改变的文本数据（该文本数据可能含有\n，所以不能使用\n分割）
         * 可使用4字节携带文件id，4字节携带用户id的size+用户id,4字节携带文本数据size+文本数据的格式
        */
        TEXT_CHANGE,

        //从服务器获取项目，携带用户id及所需项目id
        GET_PROJECT,

        //从服务器获取当前用户参与的所有项目基本信息 无需携带其他数据
        INIT_PROJS,

        //提交保存信号，携带文件id及提交者信息（用户名）
        SUBMIT,

        //上传项目保存至服务器端，携带文件名，文件大小，用户id，权限，后续应开辟线程发送项目文件
        POST_PROJECT,

        //创建云项目
        NEW_PROJECT,

        //删除项目
        DEL_PROJECT,

        //获取文件
        GET_FILE,

        //新建文件
        NEW_FILE
    };
    enum ReturnType {
        SERVER_ALLOW = 0,
        SERVER_ERROR,
        USER_PROJS,
        PROJ_FILE_INFO,
        NEW_PROJ_INFO,
        FILE,
        NEW_FILE_INFO
    };
    Package(const char* data, int type, int size);
    ~Package();
public:
    char* getPdata() { return p_data; }
    int getSize() { return p_size; }
private:
    char* p_data;
    int p_type;
    int p_size;
};

#endif // PACKAGE_H


