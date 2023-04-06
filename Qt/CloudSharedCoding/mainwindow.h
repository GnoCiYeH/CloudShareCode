#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include"logindialog.h"
#include"projectform.h"
#include<QVector>
#include"InfoType.h"
#include<QHash>
#include<QStatusBar>
#include<QTimer>
#include<QLabel>
#include<QDateTime>

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
    void newProFile();
    void deleteProFile();

    static bool loginState(){
        return isLogin;
    }

signals:
    void loginAllowed();
    void projInited();

private slots:
    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;

    QString userName;

    static bool isLogin;

    QTcpSocket* editSocket;
    QString serverIP = "192.168.239.129";
    quint16 mainPort = 9098;

    //子窗口
    LoginDialog* loginDialog;
    ProjectForm* projectForm;

    //文件容器
    QHash<int,QVector<std::shared_ptr<FileInfo>>> pro_fileMap;
    QHash<int,CodeEdit*> fileWidgets;
    QHash<int,std::shared_ptr<Directory>> mainDirMap;

    //项目树状列表菜单
    QAction* submitProject;
    QAction* closeProject;
    QAction* newFile;
    QAction* deleteFile;
    QAction* openFile;
    QAction* attribute;
    QAction* rename;

    QStatusBar* status_bar=new QStatusBar();
    QLabel* label1=new QLabel("就绪",this);
    QLabel* label2=new QLabel;
    QTimer *timer=new QTimer;
private:
    void Login();
    void addFileWidget(std::shared_ptr<FileInfo> file);
};

#endif // MAINWINDOW_H
