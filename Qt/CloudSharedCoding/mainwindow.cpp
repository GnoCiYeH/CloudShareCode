#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"myhelper.h"
#include"projectform.h"
#include"logindialog.h"
#include<QAction>
#include"package.h"

QTcpSocket* MainWindow::socket = new QTcpSocket();
QVector<MainWindow::Project>* MainWindow::userProjs = new QVector<MainWindow::Project>();
QString MainWindow::userId = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    //设置主窗口基本属性
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

    //菜单栏槽
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionClose_project,SIGNAL(triggered()),this,SLOT(closeProject()));
    connect(ui->actionCloud_project,SIGNAL(triggered()),this,SLOT(openCloudProj()));
    connect(ui->actionLocal_project,SIGNAL(triggered()),this,SLOT(openLoaclProj()));
    connect(ui->actionNew_local_project,SIGNAL(triggered()),this,SLOT(newLocalProj()));
    connect(ui->actionNew_cloud_project,SIGNAL(triggered()),this,SLOT(newCloudProj()));
    connect(ui->actionSetting,SIGNAL(triggered()),this,SLOT(openSettingDialog()));

    //socket
    connect(socket,SIGNAL(readyRead()),this,SLOT(dataProgress()));

    //子窗口
    projectForm = new ProjectForm(this);
    projectForm->setWindowFlag(Qt::Window);
    loginDialog=new LoginDialog(this);

    //子窗口槽
    connect(this,&MainWindow::loginAllowed,loginDialog,&LoginDialog::loginSucceed);
    connect(this,SIGNAL(projInited()),projectForm,SLOT(init()));
}

MainWindow::~MainWindow()
{
    delete ui;
    socket->close();
    socket->deleteLater();
    delete userProjs;
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
        Login();
    }

    //登录成功才可进行下列操作
    if(isLogin)
    {
        //从服务器拉取文件
        Package pck("",Package::PackageType::INIT_PROJS);
        socket->write(pck.getPdata(),pck.getSize());
        qDebug()<<pck.getPdata();
        projectForm->show();
    }
}

void MainWindow::Login()
{
    loginDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(loginDialog,&LoginDialog::loginAllowded,this,[=]()mutable{
        loginDialog->deleteLater();
        this->isLogin = true;
        this->userId = loginDialog->userID;
    });
    loginDialog->exec();
}

void MainWindow::dataProgress()
{
    QByteArray arr = socket->read(8);
    int type = Package::ByteArrToInt(arr,0);
    int packageSize = Package::ByteArrToInt(arr,4);

    switch (type) {
    case Package::ReturnType::SERVER_ALLOW:
    {
        emit loginAllowed();
        break;
    }
    case Package::ReturnType::SERVER_ERROR:
    {
        break;
    }
    case Package::ReturnType::USER_PROJS:
    {
        QByteArray arr = socket->read(packageSize);
        QString data(arr);
        QStringList rows = data.split("\n",Qt::SkipEmptyParts);

        for(auto i : rows)
        {
            QStringList row = i.split("\t");
            if(row.size())
            {
                int id = row[0].toInt();
                userProjs->append(Project(id,row[1],row[2]));
            }
        }

        emit projInited();
    }
    default:
        break;
    }
}
