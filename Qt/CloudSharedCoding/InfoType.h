#include<QString>
#include<QVariant>
#include<QTreeWidgetItem>
#include<memory>

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
    bool is_open = false;
    FileInfo() {}
};

class MyTreeItem : public QTreeWidgetItem
{
public:
    enum Type{
        DIR,
        FILE,
        PROJECT
    };
    explicit MyTreeItem(Type type){this->type=type;}
    Type getType(){return this->type;}
private:
    Type type;
    QTreeWidgetItem item;
};

struct Directory
{
    QString dir_name;
    QString dir_path;
    MyTreeItem* dir_item;
    QHash<QString,std::shared_ptr<Directory>> sub_dirs = QHash<QString,std::shared_ptr<Directory>>(); //子目录容器
    Directory(QString str,QString path,MyTreeItem* item){dir_name = str;dir_item = item;dir_path = path;}
};

Q_DECLARE_METATYPE(Directory);
Q_DECLARE_METATYPE(Project);
Q_DECLARE_METATYPE(FileInfo);

#endif // INFOTYPE_H
