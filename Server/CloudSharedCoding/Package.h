#ifndef PACKAGE_H
#define PACKAGE_H


class Package
{
public:
    enum PackageType {
        //�����¼��ӦЯ���û�id���������� ʹ��\n�ָ�
        LOGIN = 0,

        //ע���ź�
        REGISTER,

        /*
         * ��codeEdit->document ���� void contentsChange(int position, int charsRemoved, int charsAdded) �ź�ʱӦ��ʱ����������͸���������
         * ӦЯ���ı���ı��������ڵ��ļ�id���û�id���ı��ı��λ�á��ı���ı����ݣ����ı����ݿ��ܺ���\n�����Բ���ʹ��\n�ָ
         * ��ʹ��4�ֽ�Я���ļ�id��4�ֽ�Я���û�id��size+�û�id,4�ֽ�Я���ı�����size+�ı����ݵĸ�ʽ
        */
        TEXT_CHANGE,

        //�ӷ�������ȡ��Ŀ��Я���û�id��������Ŀid
        GET_PROJECT,

        //�ӷ�������ȡ��ǰ�û������������Ŀ������Ϣ ����Я����������
        INIT_PROJS,

        //�ύ�����źţ�Я���ļ�id���ύ����Ϣ���û�����
        SUBMIT,

        //�ϴ���Ŀ�������������ˣ�Я���ļ������ļ���С���û�id��Ȩ�ޣ�����Ӧ�����̷߳�����Ŀ�ļ�
        POST_PROJECT,

        //��������Ŀ
        NEW_PROJECT,

        //ɾ����Ŀ
        DEL_PROJECT,

        //��ȡ�ļ�
        GET_FILE,

        //�½��ļ�
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


