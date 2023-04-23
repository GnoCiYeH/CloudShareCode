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
#include "settingform.h"
#include<QFileDialog>
#include<QFile>
#include<QFileInfo>
#include<map>
#include<algorithm>
#include<QDir>
#include<QMultiHash>
#include"newlocalfile.h"

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

    static QHash<int,QMultiHash<QString,int>*>* debugInfo;

    std::map<QString,CodeEdit*>mp;//存放路径名字和CodeEdit指针的相互映射


public slots:
    void dataProgress();

    void close();
    void openCloudProj();
    void openProj(int);
    void newLocalProj();
    void openLocalProj();
    void projectItemPressedSlot(QTreeWidgetItem*,int);
    void showSetting();

    void openProjFile();
    void newProFile();
    void deleteProFile();
    void newCloudProj();
    void saveLocalProj();

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
    void runProject();
    void stopProject();
    void debugProject();
    void cmdStdin(int,int,int);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:

    enum class ProjectWorkState{
        NONE,
        BUILDING,
        RUNNING,
        DEBUGING,
    } workState = ProjectWorkState::NONE;

    enum class DebugState{
        NONE,
        START,
        WAIT_BREAKPOINT_INFO,
        WAIT_DEBUG_INFO
    } debugState = DebugState::NONE;

    void disposeDebugInfo(QString);

    Ui::MainWindow *ui;

    QString userName;

    int currentProject;

    int runningProject;

    static bool isLogin;

    QString serverIP = "192.168.239.129";
    quint16 mainPort = 9098;


    //子窗��?
    SettingForm* settingWind;
    QDockWidget* buildDock;
    QTextEdit* buildDockwidget;
    QDockWidget* runDock;
    QTextEdit* runDockwidget;
    QToolBar* debugToolBar;

    QToolButton* continueDebugButton;
    QToolButton* stopDebugButton;
    QToolButton* nextDebugButton;
    QToolButton* stepIntoDubugButton;
    QToolButton* stepOutDebugButton;

    QToolButton* runbutton;
    QToolButton* debugbutton;
    QToolButton* stopRun;

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
    bool addFileWidget(std::shared_ptr<FileInfo> file);


    SwitchingEncodingMode* encodingType=new SwitchingEncodingMode(this);//编码方式
    QLabel* EncodingTypeLabel=new QLabel(this);//打印编码方式到状态栏


};


#endif // MAINWINDOW_H
