#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include"logindialog.h"
#include"projectform.h"
#include<QVector>
#include"InfoType.h"
#include<QHash>

#include"ui_mainwindow.h"

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

    static QHash<int,Project>* userProjs;

    static QString userId;


public slots:
    void dataProgress();

    void close();
    void openCloudProj();
    void openProj(int);
    void newLocalProj();
    void projectItemPressedSlot(QTreeWidgetItem*,int);

    void openProjFile();

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
    QHash<int,QWidget*> fileWidgets;

    //项目树状列表菜单
    QAction* submitProject;
    QAction* closeProject;
    QAction* newFile;
    QAction* newFileFolder;
    QAction* deleteFile;
    QAction* openFile;
    QAction* attribute;
    QAction* rename;

private:
    void Login();
    void addFileWidget(FileInfo& file);
};

#endif // MAINWINDOW_H
