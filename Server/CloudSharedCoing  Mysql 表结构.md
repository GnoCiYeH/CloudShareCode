# CloudSharedCoding : Mysql 表结构

项目地址：<https://github.com/GnoCiYeH/CloudShareCode.git>

## 数据库：CloudSharedCoding

- ### User

|    字段名     | 字段类型及其属性          | 描述               |
| :-----------: | ------------------------- | ------------------ |
|    user_id    | primary key not null      | 主键 非空          |
| user_password | binary (32) not null      | md5加密存储 非空   |
|   user_icon   | varchar(255) default null | 用户头像的存储路径 |

- ### File

|     字段名     | 字段类型及其属性                        | 描述                            |
| :------------: | --------------------------------------- | ------------------------------- |
|    file_id     | int primary key auto_increment not null | 文件的唯一标识符,主键 非空 自增 |
|   file_name    | varchar(32) not null                    | 文件名                          |
|   file_user    | varchar(32) not null                    | 文件所属用户                    |
|   file_path    | varchar(255) not null                   | 文件服务器路径                  |
|  file_project  | int not null                            | 所属项目id                      |
| file_privilege | tinyint not null default 0              | 权限等级                        |

- ### Project

|  字段名   | 字段类型及其属性                        | 描述                                                         |
| :-------: | --------------------------------------- | ------------------------------------------------------------ |
|  pro_id   | int primary key auto_increment not null | 项目唯一标识符，主键 非空 自增，可用于查询拥有该项目权限的所有用户 |
| pro_owner | varchar(32)                             | 项目所有者id                                                 |
| pro_name  | varchar(32) not null                    | 项目名                                                       |
| pro_uuid  | varchar(36) not null                    | 加入别人的项目时所需uuid                                     |

- ### Privilege  权限表为一个三元组，记录项目的权限列表及用户对于该项目的权限级别

|      字段名      | 字段类型及其属性           | 描述                   |
| :--------------: | -------------------------- | ---------------------- |
|     _user_id     | varchar(32) not null       | 拥有该项目权限的用户id |
|     _pro_id      | int not null               | 项目表中pro_id外键     |
| _privilege_level | tinyint not null default 0 | 权限等级               |

---

## 权限级别

| 权限等级 | 用户权限                                                     | 文件权限 |
| :------: | ------------------------------------------------------------ | -------- |
|    0     | root权限，可对该项目/文件进行所有操作，对项目uuid可见（uuid为拉取其他人进入项目的唯一标识）。 | root     |
|    1     | 可对除root规定的Protected级别文件及root文件级别外执行所有操作，对项目uuid可见。 | protect  |
|    2     | 对Protect文件有可读权限，root文件可见不可读，对权限等级>=2的所有文件拥有所有权限。 | privite  |
|    3     | 对Protect文件无可读权限，root文件不可见，对权限等级>=3的所有文件拥有所有权限。 | default  |
|    4     | 对项目所有文件仅可读，root（管理员及项目所有者）规定的Protected级别文件及root级别文件不可见。 | public   |







