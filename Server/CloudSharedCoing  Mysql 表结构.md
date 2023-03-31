# CloudSharedCoding : Mysql 表结构

项目地址：<https://github.com/GnoCiYeH/CloudShareCode.git>

## 数据库：CloudSharedCoding

### User

- user_id  varchar(32)  primary key not null //主键 非空
- user_password   binary (32) not null //md5加密存储 非空
- user_icon  varchar(255) default null //用户头像的存储路径

### File

- file_id int primary key auto_increment not null //文件的唯一标识符,主键 非空 自增
- file_name varchar(32) not null //文件名
- file_user varchar(32) not null //文件所属用户
- file_path varchar(255) not null //文件路径
- file_project int not null //所属项目id
- file_privilege tinyint not null default 0  //权限等级 分为0 1 2 3 4 越小权限越高

### Project

- pro_id int primary key auto_increment not null //项目唯一标识符，主键 非空 自增，可用于查询拥有该项目权限的所有用户
- pro_owner varchar(32) //项目所有者id
- pro_name varchar(32) not null //项目名

### Privilege  权限表为一个二元组，记录项目的权限列表

- _user_id varchar(32) not null 
- _pro_id int not null //项目表中pro_id外键
- _privilege_level tinyint not null default 0 //权限等级 分为0 1 2 3 4 越小权限越高



