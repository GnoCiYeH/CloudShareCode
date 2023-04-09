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
#include"switchingencodingmode.h"

#include "codeedit.h"
//#include"ui_switchingencodingmode.h"
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
    void newCloudProj();

    static bool loginState(){
        return isLogin;
    }

signals:
    void loginAllowed();
    void projInited();
    void returnHeart();

private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void selectencodingMode();

private:
    Ui::MainWindow *ui;

    QString userName;

    static bool isLogin;

    QTcpSocket* editSocket;
    QString serverIP = "192.168.239.129";
    quint16 mainPort = 9098;


    //子窗��?

    //瀛愮獥鍙?

    LoginDialog* loginDialog;
    ProjectForm* projectForm;

    //鏂囦欢瀹瑰櫒
    QHash<int,QVector<std::shared_ptr<FileInfo>>> pro_fileMap;
    QHash<int,CodeEdit*> fileWidgets;
    QHash<int,std::shared_ptr<Directory>> mainDirMap;

    //椤圭洰鏍戠姸鍒楄〃鑿滃崟
    QAction* submitProject;
    QAction* closeProject;
    QAction* newFile;
    QAction* deleteFile;
    QAction* openFile;
    QAction* attribute;
    QAction* rename;


    //心跳检��?

    //蹇冭烦妫€娴?

    QTimer* heartTimer;
    QTimer* detectTimer;
    bool isAlive = true;

    QStatusBar* status_bar=new QStatusBar();
    QLabel* label1=new QLabel("就绪",this);

    QLabel* label2=new QLabel(this);
    QTimer *timer=new QTimer(this);

private:
    void Login();
    void addFileWidget(std::shared_ptr<FileInfo> file);


    SwitchingEncodingMode* encodingType=new SwitchingEncodingMode(this);//编码方式
    QLabel* EncodingTypeLabel=new QLabel(this);//打印编码方式到状态栏


};


#endif // MAINWINDOW_H
