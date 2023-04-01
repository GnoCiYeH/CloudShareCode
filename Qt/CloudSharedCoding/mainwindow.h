#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include"logindialog.h"
#include"projectform.h"
#include<QVector>

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

    struct Project
    {
        int pro_id;
        QString pro_name;
        QString pro_owner;
        Project(int id,QString name,QString owner) {
            pro_id=id;
            pro_name=name;
            pro_owner=owner;
        }
    };

    static QVector<Project>* userProjs;

    static QString userId;

public slots:
    void dataProgress();

    void close();
    void openCloudProj();

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

private:
    void Login();
};

Q_DECLARE_METATYPE(MainWindow::Project);

#endif // MAINWINDOW_H
