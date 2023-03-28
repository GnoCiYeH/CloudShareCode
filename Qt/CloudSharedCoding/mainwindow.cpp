#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"myhelper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myHelper::setStyle(":/qss/psblack.css");
    this->setWindowTitle("CloudSharedCoding");

    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionCloud_project,SIGNAL(triggered()),this,SLOT(openCloudProj()));
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
    bool temp;
    if(!isLogin)
    {
        temp = Login();
    }

    //登录成功才可进行下列操作
    if(temp)
    {
        //从服务器拉取文件
    }
}

bool MainWindow::Login()
{

}
