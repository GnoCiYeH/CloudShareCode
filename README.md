# CloudShareCode
## 多人在线实时代码编辑器

项目地址：[CloudShareCode](https://github.com/GnoCiYeH/CloudShareCode.git)

---

## 项目简介

​	基于c++的多人在线实时c++IDE，使用TCP/IP协议进行C/S交互。

​	多人共同编辑部分请看演示视频。

​	在实现基础的c++ide功能的同时，添加了项目权限管理的功能，具有相应权限的用户才能对相应的文件进行更改或者查看。管理员能够随时调整项目成员的权限。
![1682700820031](https://github.com/GnoCiYeH/CloudShareCode/blob/main/README_IMAGE/1682700820031.png)

​	

​	我们的团队在没有任何IDE开发经验的情况下，一个月搭建出了目前的demo，项目里面也还有很多bug，但是团队里的每个人都得到了相应的锻炼，也积累了多人合作开发的经验。在合作开发的过程中遇到过很多没遇到过的问题，也出现了很多代码冲突，在开发过程中也遇到很多问题，为一个bug埋头苦干一整天都是常有的事，但是当最后这个demo能够正常的运作并且能够实现我们最初想的功能的时候，我们每个人心中都是有莫大的成就感的。

​	在项目中还会有很多问题，今后会逐渐完善修改。

---

## 客户端

- 使用Qt框架搭建

- 离线可用作文本编辑器（代码编辑）

  ![1682699392803](https://github.com/GnoCiYeH/CloudShareCode/blob/main/README_IMAGE/1682699392803.png)

- 使用本地编译器以及本地gdb进行代码编译调试运行

  - 运行

  ![1682699459241](https://github.com/GnoCiYeH/CloudShareCode/blob/main/README_IMAGE/1682699459241.png)

  - 调试

  ![1682700231062](https://github.com/GnoCiYeH/CloudShareCode/blob/main/README_IMAGE/1682700231062.png)

- 支持云端代码编译运行调试

  - 运行

  ![1682700519765](https://github.com/GnoCiYeH/CloudShareCode/blob/main/README_IMAGE/1682700519765.png)

  - 调试

  ![1682700436465](https://github.com/GnoCiYeH/CloudShareCode/blob/main/README_IMAGE/1682700436465.png)

- 登录可参与多人编辑项目（云端存储，以后可支持上传以及下载）

  - 通过项目独一无二的uuid分享项目和加入项目
  - 项目将会存储在云端，后会实现项目下拉与上传的功能

- 代码编辑器主题风格自定义

  我们为我们的软件添置了许多风格（网上寻找的别人分享开源的css文件）可让用户自定义。

  - Light

  ![1682699227777](https://github.com/GnoCiYeH/CloudShareCode/blob/main/README_IMAGE/1682699227777.png)

  - Dark

  ![1682699338572](https://github.com/GnoCiYeH/CloudShareCode/blob/main/README_IMAGE/1682699338572.png)

  - 以及诸多其他风格。

  

  用户可自定义自己喜欢的代码风格

  ![1682700990720](https://github.com/GnoCiYeH/CloudShareCode/blob/main/README_IMAGE/1682700990720.png)

- 未来可拓展
  - 成为功能更加完美的IDE
  - 插件系统
  - 项目传输
  
  ---

## 服务器

- Linux搭建服务器
- 日志系统记录服务器行为

![1682701238819](https://github.com/GnoCiYeH/CloudShareCode/blob/main/README_IMAGE/1682701238819.png)

- 使用epoll io多路复用技术提高并发效率和性能（未系统测试性能）
- 使用线程池技术提高服务器性能
- 连接Mysql数据库进行数据存储  [数据库结构](https://github.com/GnoCiYeH/CloudShareCode/blob/main/Server/CloudSharedCoing%20%20Mysql%20%E8%A1%A8%E7%BB%93%E6%9E%84.md)
- 未来可拓展
  - 分布式文件存储
  - 文件加密
