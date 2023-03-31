# CloudShareCode
## 多人在线实时代码编辑器

项目地址：[CloudShareCode](https://github.com/GnoCiYeH/CloudShareCode.git)

---

## 项目简介

​	基于c++的多人在线实时代码编辑器吗，使用TCP/IP协议进行C/S交互

- 客户端
  - 使用Qt框架搭建
  - 离线可用作文本编辑器（代码编辑）
  - 登录可参与多人编辑项目（云端存储，可支持上传以及下载）
  - 未来可能的拓展
    - 项目的云端编译运行
    - 项目的本地编译运行
    - 成为功能完善的IDE

- 服务器
  - Linux搭建服务器
  - 使用epoll io多路复用
  - 连接Mysql数据库进行数据存储  [数据库结构](https://github.com/GnoCiYeH/CloudShareCode/blob/main/Server/CloudSharedCoing%20%20Mysql%20%E8%A1%A8%E7%BB%93%E6%9E%84.md)
  - 未来可能的拓展
    - 分布式文件存储
    - 文件加密
    - 云端编译实时返回客户端
