#ifndef PACKAGE_H
#define PACKAGE_H


class Package
{
public:
    enum Type {

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


