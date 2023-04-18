#ifndef PACKAGE_H
#define PACKAGE_H


class Package
{
public:
    enum class PackageType {
        //�����¼��ӦЯ���û�id���������� ʹ��\n�ָ�
        LOGIN = 0,

        //ע���ź�
        REGISTER = 1,

        /*
         * ��codeEdit->document ���� void contentsChange(int position, int charsRemoved, int charsAdded) �ź�ʱӦ��ʱ����������͸���������
         * ӦЯ���ı���ı��������ڵ��ļ�id���û�id���ı��ı��λ�á��ı���ı����ݣ����ı����ݿ��ܺ���\n�����Բ���ʹ��\n�ָ
         * ��ʹ��4�ֽ�Я���ļ�id��4�ֽ�Я���û�id��size+�û�id,4�ֽ�Я���ı�����size+�ı����ݵĸ�ʽ
        */
        TEXT_CHANGE = 2,

        //�ӷ�������ȡ��Ŀ��Я���û�id��������Ŀid
        GET_PROJECT = 3,

        //�ӷ�������ȡ��ǰ�û������������Ŀ������Ϣ ����Я����������
        INIT_PROJS = 4,

        //�ύ�����źţ�Я���ļ�id���ύ����Ϣ���û�����
        SUBMIT = 5,

        //�ϴ���Ŀ�������������ˣ�Я���ļ������ļ���С���û�id��Ȩ�ޣ�����Ӧ�����̷߳�����Ŀ�ļ�
        POST_PROJECT = 6,

        //��������Ŀ
        NEW_PROJECT = 7,

        //ɾ����Ŀ
        DEL_PROJECT = 8,

        //��ȡ�ļ�
        GET_FILE = 9,

        //�½��ļ�
        NEW_FILE = 10,

        //ɾ���ļ�
        DEL_FILE = 11,

        //������Ŀ
        JOIN_PROJECT = 12,

        //������
        HEART_PCK = 13,

        //��ĿȨ�޲�ѯ
        PRIVILEGE_QUERY = 14,

        //��ĿȨ�޹���
        PRIVILEGE_UPDATE = 15,

        //����������Ŀ
        RUN_PROJECT = 16
    };
    enum class ReturnType {
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
        PRIVILEGE_INFO = 10,
        SERVER_OK = 11,
        BUILD_INFO = 12,
        RUN_INFO = 13
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


