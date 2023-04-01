#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include"logindialog.h"
#include"projectform.h"

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

public slots:
    void dataProgress();

    void close();
    void openCloudProj();

signals:
    void loginAllowed();

private:
    Ui::MainWindow *ui;

    QString userId;
    QString userName;

    bool isLogin = false;

    QTcpSocket* editSocket;
    QString serverIP = "192.168.239.129";
    quint16 mainPort = 9098;

    //子窗口
    LoginDialog* loginDialog;
    ProjectForm *projectForm;

private:
    void Login();
};

#endif // MAINWINDOW_H
