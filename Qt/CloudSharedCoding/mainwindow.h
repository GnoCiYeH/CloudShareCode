#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include"logindialog.h"
#include"projectform.h"
#include<QVector>
#include"InfoType.h"
#include<QHash>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static QTcpSocket* socket;

    static QVector<Project>* userProjs;

    static QString userId;


public slots:
    void dataProgress();

    void close();
    void openCloudProj();
    void openProj(int);

signals:
    void loginAllowed();
    void recvError();
    void projInited();

private:
    Ui::MainWindow *ui;

    QString userName;

    bool isLogin = false;

    QTcpSocket* editSocket;
    QString serverIP = "192.168.239.129";
    quint16 mainPort = 9098;

    //子窗口
    LoginDialog* loginDialog;
    ProjectForm *projectForm;

    //文件容器
    QHash<int,QVector<FileInfo>> pro_fileMap;

private:
    void Login();
};

#endif // MAINWINDOW_H
