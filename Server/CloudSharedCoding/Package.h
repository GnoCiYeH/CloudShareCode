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

        //�ύ�����źţ�Я���ļ�id���ύ����Ϣ���û�����
        SUBMIT,

        //�ϴ���Ŀ�������������ˣ�Я���ļ������ļ���С���û�id��Ȩ�ޣ�����Ӧ�����̷߳�����Ŀ�ļ�
        POST_PROJECT
    };
    enum ReturnType {
        ALLOW = 0,
        ERROR
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

