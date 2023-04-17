#include<QString>
#include<QVariant>
#include<QTreeWidgetItem>
#include<memory>

#ifndef INFOTYPE_H
#define INFOTYPE_H

class MyTreeItem;
struct Project
{
    int pro_id;
    QString pro_name;
    QString pro_owner;
    QString pro_uuid = "";
    short pro_privilege_level = 4;
    Project(){}
    Project(int id,QString name,QString owner,short level,QString uuid) {
        pro_id=id;
        pro_name=name;
        pro_owner=owner;
        if(level>=0&&level<=4)
        {
            pro_privilege_level=level;
        }
        if(pro_privilege_level<=1)
        {
            pro_uuid = uuid;
        }
    }
};

struct FileInfo
{
    int file_id;
    QString file_name;
    QString file_user;
    QString file_path;
    MyTreeItem* file_item;
    int file_project;
    short file_privilege;
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
    int pro_id = -1; //-1代表本地项目或非法项目
    MyTreeItem* dir_item;
    QHash<QString,std::shared_ptr<Directory>> sub_dirs = QHash<QString,std::shared_ptr<Directory>>(); //子目录容器
    Directory(int id,QString str,QString path,MyTreeItem* item){pro_id = id;dir_name = str;dir_item = item;dir_path = path;}
};

enum HightlightType{
    KEYWORD,
    CLASS,
    SIGNLE_LINE_COMMENT,
    MULITLINE_COMMENT,
    QUOTATION,
    FUNCTION,
    HEADER,
    STDIO,
    BRANCH
};

Q_DECLARE_METATYPE(Directory);
Q_DECLARE_METATYPE(Project);
Q_DECLARE_METATYPE(FileInfo);

#endif // INFOTYPE_H
