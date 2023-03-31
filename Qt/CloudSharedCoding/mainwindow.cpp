#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"myhelper.h"
#include"projectform.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myHelper::setStyle(":/qss/psblack.css");
    this->setWindowTitle("CloudSharedCoding");

    QToolButton* undoButton = new QToolButton(this);
    undoButton->setIcon(QIcon("://qss/darkblack/add_left.png"));
    undoButton->setFixedSize(20,20);
    ui->mainToolBar->addWidget(undoButton);
    QToolButton* forwordButton = new QToolButton(this);
    forwordButton->setIcon(QIcon("://qss/darkblack/add_right.png"));
    forwordButton->setFixedSize(20,20);
    ui->mainToolBar->addWidget(forwordButton);

    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionClose_project,SIGNAL(triggered()),this,SLOT(closeProject()));
    connect(ui->actionCloud_project,SIGNAL(triggered()),this,SLOT(openCloudProj()));
    connect(ui->actionLocal_project,SIGNAL(triggered()),this,SLOT(openLoaclProj()));
    connect(ui->actionNew_local_project,SIGNAL(triggered()),this,SLOT(newLocalProj()));
    connect(ui->actionNew_cloud_project,SIGNAL(triggered()),this,SLOT(newCloudProj()));
    connect(ui->actionSetting,SIGNAL(triggered()),this,SLOT(openSettingDialog()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::close()
{
    this->deleteLater();
}

void MainWindow::openCloudProj()
{
    //若用户未登录则无法使用在线功能，弹出登录界面
    if(!isLogin)
    {
        isLogin = Login();
    }

    //登录成功才可进行下列操作
    if(isLogin)
    {
        //从服务器拉取文件
        ProjectForm *form = new ProjectForm(this);
        form->setWindowFlag(Qt::Window);
        form->show();
    }
}

bool MainWindow::Login()
{

}
