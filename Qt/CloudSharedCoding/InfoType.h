#include<QString>
#include<QVariant>

#ifndef INFOTYPE_H
#define INFOTYPE_H

struct Project
{
    int pro_id;
    QString pro_name;
    QString pro_owner;
    Project(){}
    Project(int id,QString name,QString owner) {
        pro_id=id;
        pro_name=name;
        pro_owner=owner;
    }
};

struct FileInfo
{
    int file_id;
    QString file_name;
    QString file_user;
    QString file_path;
    int file_project;
    short file_privilege;
    bool file_isProtect;
    FileInfo() {}
};
Q_DECLARE_METATYPE(Project);
Q_DECLARE_METATYPE(FileInfo);

#endif // INFOTYPE_H
