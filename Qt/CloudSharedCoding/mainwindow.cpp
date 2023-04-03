#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"myhelper.h"
#include"projectform.h"
#include"logindialog.h"
#include<QAction>
#include<QFileDialog>
#include"package.h"
#include"newprojectdialog.h"
#include<QPoint>

QTcpSocket* MainWindow::socket = new QTcpSocket();
QHash<int,Project>* MainWindow::userProjs = new QHash<int,Project>();
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
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    ui->treeWidget->setLayout(layout);
    ui->treeWidget->header()->hide();
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    //右键菜单
    submitProject = new QAction("提交项目",ui->treeWidget);
    closeProject = new QAction("关闭项目",ui->treeWidget);
    newFile = new QAction("新建文件",ui->treeWidget);
    newFileFolder = new QAction("新建文件夹",ui->treeWidget);
    deleteFile = new QAction("删除",ui->treeWidget);
    openFile = new QAction("打开",ui->treeWidget);
    attribute = new QAction("属性",ui->treeWidget);
    rename = new QAction("重命名",ui->treeWidget);

    //主菜单栏槽
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
    connect(projectForm,SIGNAL(openProj(int)),this,SLOT(openProj(int)));

    connect(ui->treeWidget, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(projectItemPressedSlot(QTreeWidgetItem*,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    socket->close();
    socket->deleteLater();
    delete userProjs;
}

void MainWindow::projectItemPressedSlot(QTreeWidgetItem* i,int column)
{
    if(qApp->mouseButtons() == Qt::RightButton)
    {
        QMenu* menu = new QMenu(ui->treeWidget);
        MyTreeItem* item = (MyTreeItem*)i;

        switch (item->getType()) {
        case MyTreeItem::Type::DIR:
        {
            menu->addAction(newFile);
            menu->addAction(newFileFolder);
            menu->addAction(rename);
            menu->addAction(deleteFile);
            menu->addAction(attribute);
            break;
        }
        case MyTreeItem::Type::PROJECT:
        {
            menu->addAction(submitProject);
            menu->addAction(newFile);
            menu->addAction(newFileFolder);
            menu->addAction(rename);
            menu->addAction(closeProject);
            menu->addAction(deleteFile);
            menu->addAction(attribute);
            break;
        }
        case MyTreeItem::Type::FILE:
        {
            menu->addAction(openFile);
            menu->addAction(rename);
            menu->addAction(deleteFile);
            menu->addAction(attribute);
            break;
        }
        default:
            break;
        }

        menu->exec(QCursor::pos());
    }
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
        if(isLogin)
        {
            //从服务器拉取项目信息
            Package pck("",Package::PackageType::INIT_PROJS);
            socket->write(pck.getPdata(),pck.getSize());
        }
    }

    //登录成功才可进行下列操作
    if(isLogin)
    {
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

void MainWindow::openProj(int id)
{
    Package pck(QString::number(id).toUtf8(),Package::PackageType::GET_PROJECT);
    socket->write(pck.getPdata(),pck.getSize());
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
        QByteArray arr = socket->read(packageSize);
        QMessageBox box;
        box.setWindowTitle("发生错误");
        box.setText("错误码："+QString(arr));
        emit recvError();
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
                userProjs->insert(id,Project(id,row[1],row[2]));
            }
        }

        emit projInited();
        break;
    }
    case Package::ReturnType::NEW_PROJ_INFO:
    {
        QByteArray arr = socket->read(packageSize);
        QString data(arr);
        QStringList list = data.split("\t");
        Project proj(list[0].toInt(),list[1],list[2]);
        projectForm->addItem(proj);
        userProjs->insert(proj.pro_id,proj);
        break;
    }
    case Package::ReturnType::PROJ_FILE_INFO:
    {
        QString data(socket->read(packageSize));
        QStringList list = data.split("\n",Qt::SkipEmptyParts);

        QStringList proInfo = list[0].split("\t");
        int pro_id = proInfo[0].toInt();
        MyTreeItem* item = new MyTreeItem(MyTreeItem::Type::PROJECT);
        auto i = userProjs->value(pro_id);
        item->setText(0,i.pro_name);
        QString path = "./"+i.pro_owner+"/"+i.pro_name+"/";
        std::shared_ptr<Directory> mainDir(new Directory(i.pro_name,path,item));
        QVariant var;
        var.setValue(mainDir);
        item->setData(0,Qt::UserRole,var);
        ui->treeWidget->addTopLevelItem(item);

        if(list.empty())
            return;

        QVector<FileInfo> fileVec;
        for(int j = 1 ; j < list.size() ;j++)
        {
            auto i = list[j];
            auto info = i.split("\t",Qt::SkipEmptyParts);
            FileInfo file;
            file.file_id = info[0].toInt();
            file.file_name = info[1];
            file.file_user = info[2];
            file.file_path = info[3];
            file.file_project = info[4].toInt();
            file.file_privilege = info[5].toShort();
            file.file_isProtect = info[6].toInt();

            fileVec.append(file);
        }

        pro_fileMap.insert(pro_id,fileVec);

        for(int i = 0; i < fileVec.size() ; i++)
        {
            //Path格式： ./UserId/ProName/Dir(File)...
            FileInfo file = fileVec[i];
            QStringList list = file.file_path.split("/",Qt::SkipEmptyParts);
            int size = list.size();
            std::shared_ptr<Directory> dir = mainDir;
            for(int i = 3; i < size ; i++)
            {
                if(dir->sub_dirs.contains(list[i]))
                {
                    dir = dir->sub_dirs.value(list[i]);
                }
                else if(i != size-1)
                {
                    MyTreeItem* item = new MyTreeItem(MyTreeItem::Type::DIR);
                    item->setText(0,list[i]);
                    dir->dir_item->addChild(item);
                    std::shared_ptr<Directory> subDir(new Directory(list[i],dir->dir_path+list[i]+"/",item));
                    dir->sub_dirs.insert(list[i],subDir);
                    dir = dir->sub_dirs.value(list[i]);
                    QVariant var;
                    var.setValue(subDir);
                    item->setData(0,Qt::UserRole,var);
                }
                else
                {
                    MyTreeItem* item = new MyTreeItem(MyTreeItem::Type::FILE);
                    item->setText(0,file.file_name);
                    QVariant var;
                    var.setValue(file);
                    item->setData(0,Qt::UserRole,var);
                    dir->dir_item->addChild(item);
                }
            }
        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::newLocalProj()
{

}
