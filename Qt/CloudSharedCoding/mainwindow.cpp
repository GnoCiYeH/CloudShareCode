#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"myhelper.h"
#include"projectform.h"
#include"logindialog.h"
#include<QAction>
#include<QFileDialog>
#include"package.h"
#include"newprojectdialog.h"
#include "newfiledialog.h"
#include<QPoint>


QTcpSocket* MainWindow::socket = new QTcpSocket();
QHash<int,Project>* MainWindow::userProjs = new QHash<int,Project>();
QString MainWindow::userId = "";
bool MainWindow::isLogin = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    encodingType->setAttribute(Qt::WA_DeleteOnClose);

    socket = new QTcpSocket(this);
    heartTimer = new QTimer(this);
    detectTimer = new QTimer(this);
    connect(heartTimer,&QTimer::timeout,this,[=](){
        Package pck("",(int)Package::PackageType::HEART_PCK);
        socket->write(pck.getPdata(),pck.getSize());
    });
    connect(detectTimer,&QTimer::timeout,this,[=]()mutable{
        if(!isAlive)
            {
            qDebug()<<"离线！！！！";
        }
        isAlive = false;
    });

    //设置主窗口基本属性
    this->setWindowFlags(Qt::FramelessWindowHint);
    myHelper::setStyle(":/qss/flatwhite.css");
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
    ui->tabWidget->removeTab(1);

    //右键菜单
    submitProject = new QAction("提交项目",ui->treeWidget);
    closeProject = new QAction("关闭项目",ui->treeWidget);
    newFile = new QAction("新建文件",ui->treeWidget);
    deleteFile = new QAction("删除",ui->treeWidget);
    openFile = new QAction("打开",ui->treeWidget);
    attribute = new QAction("属性",ui->treeWidget);
    rename = new QAction("重命名",ui->treeWidget);

    //状态栏(显示状态和时间）
    setStatusBar(status_bar);
    status_bar->addWidget(label1);
    status_bar->setStyleSheet("color::rgb(0,0,0");
    timer->start(1000);
    connect(timer,&QTimer::timeout,this,[=](){
        QString str;
        QDateTime time_date=QDateTime::currentDateTime();
        str=time_date.toString("yyyy-MM-dd hh:mm:ss");
        label2->setText(str);
        status_bar->addPermanentWidget(label2);
    });

    //在状态栏显示编码方式
    EncodingTypeLabel->setText("        当前的编码方式为：ASCII (默认为ASCII编码，如需更改，请打开“设置-编码方式”中进行选择）");
    status_bar->addWidget(EncodingTypeLabel);
    EncodingTypeLabel->setAlignment(Qt::AlignCenter);




    //右键菜单槽
    connect(openFile,SIGNAL(triggered(bool)),this,SLOT(openProjFile()));
    connect(newFile,SIGNAL(triggered(bool)),this,SLOT(newProFile()));
    connect(deleteFile,SIGNAL(triggered(bool)),this,SLOT(deleteProFile()));

    //主菜单栏槽
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionCloud_project,SIGNAL(triggered()),this,SLOT(openCloudProj()));
    connect(ui->actionLocal_project,SIGNAL(triggered()),this,SLOT(openLoaclProj()));
    connect(ui->actionNew_local_project,SIGNAL(triggered()),this,SLOT(newLocalProj()));
    connect(ui->actionNew_cloud_project,SIGNAL(triggered()),this,SLOT(newCloudProj()));
    connect(ui->Setting,SIGNAL(triggered()),this,SLOT(openSettingDialog()));

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

    //选择编码方式
    connect(ui->actionSwitching,&QAction::triggered,this,&MainWindow::selectencodingMode);

    //关闭
    connect(ui->actionClose,&QAction::triggered,this,&QMainWindow::close);

    //新建文件
    connect(ui->new_file_action,&QAction::triggered,this,[=](){QFileDialog::getOpenFileName(this,"新建文件","C:/Users");});

    //添加文件
    connect(ui->add_file_action,&QAction::triggered,this,[=](){QFileDialog::getOpenFileName(this,"添加文件","C:/Users");});
}

MainWindow::~MainWindow()
{
    delete ui;
    socket->close();
    socket->deleteLater();
    delete userProjs;
}

void MainWindow::newCloudProj()
{
    //若用户未登录则无法使用在线功能，弹出登录界面
    if(!isLogin)
    {
        Login();
        if(isLogin)
        {
            //从服务器拉取项目信息
            Package pck("",(int)Package::PackageType::INIT_PROJS);
            socket->write(pck.getPdata(),pck.getSize());
        }
    }

    //登录成功才可进行下列操作
    if(isLogin)
    {
        NewProjectDialog dialog(false,this);
        dialog.exec();
    }
}

void MainWindow::deleteProFile()
{
    auto item = ui->treeWidget->currentItem();
    auto var = item->data(0,Qt::UserRole);
    auto file = var.value<FileInfo>();

    QMessageBox::StandardButton result = QMessageBox::warning(this,"确定删除？","您确定要删除文件："+file.file_name+"?");
    if(result!=QMessageBox::StandardButton::Ok)
        return;

    QString data = QString::number(file.file_id) + "\t" + file.file_path + "\t" + QString::number(file.file_project);
    Package pck(data.toUtf8(),(int)Package::PackageType::DEL_FILE);

    socket->write(pck.getPdata(),pck.getSize());
}

void MainWindow::newProFile()
{
    auto item = ui->treeWidget->currentItem();
    QVariant var = item->data(0,Qt::UserRole);
    auto dir = var.value<std::shared_ptr<Directory>>();

    if(dir->pro_id!=-1)
    {
        //向服务器发送请求
        NewFileDialog wind(dir,this);
        wind.exec();
    }
}

void MainWindow::addFileWidget(std::shared_ptr<FileInfo> file)
{
    CodeEdit* widget = new CodeEdit(this);
    widget->setFile(file);
    ui->tabWidget->addTab(widget,file->file_name);
    fileWidgets.insert(file->file_id,widget);
    file->is_open = true;
}

void MainWindow::openProjFile()
{
    MyTreeItem* item = (MyTreeItem*)ui->treeWidget->currentItem();
    QVariant var = item->data(0,Qt::UserRole);
    std::shared_ptr<FileInfo> file = var.value<std::shared_ptr<FileInfo>>();

    if(!file->is_open)
    {
        addFileWidget(file);
        Package pck(QString::number(file->file_id).toUtf8(),(int)Package::PackageType::GET_FILE);
        socket->write(pck.getPdata(),pck.getSize());
        socket->flush();
    }
    else
    {
        QWidget* widget = fileWidgets.value(file->file_id);
        ui->tabWidget->setCurrentWidget(widget);
    }
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
            menu->addAction(rename);
            menu->addAction(deleteFile);
            menu->addAction(attribute);
            break;
        }
        case MyTreeItem::Type::PROJECT:
        {
            menu->addAction(submitProject);
            menu->addAction(newFile);
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
    qApp->exit(0);
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
            Package pck("",(int)Package::PackageType::INIT_PROJS);
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
    Package pck(QString::number(id).toUtf8(),(int)Package::PackageType::GET_PROJECT);
    socket->write(pck.getPdata(),pck.getSize());
}

void MainWindow::dataProgress()
{
    QByteArray arr = socket->read(8);
    int type = Package::ByteArrToInt(arr,0);
    int packageSize = Package::ByteArrToInt(arr,4);

    switch (type) {
    case (int)Package::ReturnType::SERVER_ALLOW:
    {
        emit loginAllowed();
        heartTimer->start(50);
        break;
    }
    case (int)Package::ReturnType::SERVER_ERROR:
    {
        QByteArray arr = socket->read(packageSize);
        QMessageBox box;
        box.setWindowTitle("发生错误");
        box.setText("错误码："+QString(arr));
        box.exec();
        break;
    }
    case (int)Package::ReturnType::USER_PROJS:
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
                short level = row[3].toShort();
                userProjs->insert(id,Project(id,row[1],row[2],level,row[4]));
            }
        }

        emit projInited();
        break;
    }
    case (int)Package::ReturnType::NEW_PROJ_INFO:
    {
        QByteArray arr = socket->read(packageSize);
        QString data(arr);
        QStringList list = data.split("\t");
        Project proj(list[0].toInt(),list[1],list[2],0,list[3]);
        projectForm->addItem(proj);
        userProjs->insert(proj.pro_id,proj);
        break;
    }
    case (int)Package::ReturnType::PROJ_FILE_INFO:
    {
        QString data(socket->read(packageSize));
        QStringList list = data.split("\n",Qt::SkipEmptyParts);

        QStringList proInfo = list[0].split("\t");
        int pro_id = proInfo[0].toInt();
        MyTreeItem* item = new MyTreeItem(MyTreeItem::Type::PROJECT);
        auto i = userProjs->value(pro_id);
        item->setText(0,i.pro_name);
        QString path = "./"+i.pro_owner+"/"+i.pro_name+"/";
        std::shared_ptr<Directory> mainDir(new Directory(pro_id,i.pro_name,path,item));
        QVariant var;
        var.setValue(mainDir);
        item->setData(0,Qt::UserRole,var);
        ui->treeWidget->addTopLevelItem(item);

        mainDirMap.insert(i.pro_id,mainDir);

        if(list.empty())
            return;

        QVector<std::shared_ptr<FileInfo>> fileVec;
        for(int j = 1 ; j < list.size() ;j++)
        {
            auto i = list[j];
            auto info = i.split("\t",Qt::SkipEmptyParts);
            std::shared_ptr<FileInfo> file(new FileInfo);
            file->file_id = info[0].toInt();
            file->file_name = info[1];
            file->file_user = info[2];
            file->file_path = info[3];
            file->file_project = info[4].toInt();
            file->file_privilege = info[5].toShort();

            fileVec.append(file);
        }

        pro_fileMap.insert(pro_id,fileVec);

        for(int i = 0; i < fileVec.size() ; i++)
        {
            //Path格式： ./UserId/ProName/Dir(File)...
            std::shared_ptr<FileInfo> file = fileVec[i];
            QStringList list = file->file_path.split("/",Qt::SkipEmptyParts);
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
                    std::shared_ptr<Directory> subDir(new Directory(pro_id,list[i],dir->dir_path+list[i]+"/",item));
                    dir->sub_dirs.insert(list[i],subDir);
                    dir = dir->sub_dirs.value(list[i]);
                    QVariant var;
                    var.setValue(subDir);
                    item->setData(0,Qt::UserRole,var);
                }
                else
                {
                    MyTreeItem* item = new MyTreeItem(MyTreeItem::Type::FILE);
                    item->setText(0,file->file_name);
                    QVariant var;
                    var.setValue(file);
                    item->setData(0,Qt::UserRole,var);
                    dir->dir_item->addChild(item);
                    file->file_item = item;
                }
            }
        }
        break;
    }
    case (int)Package::ReturnType::FILE:
    {
        //接收文件，并显示在CodeEdit中
        QByteArray temp = socket->read(4);
        int fid = Package::ByteArrToInt(temp,0);

        QByteArray data = socket->read(packageSize-4);

        auto fileEditor = (CodeEdit*)fileWidgets.value(fid);

        fileEditor->addText(QString(data));

        break;
    }
    case (int)Package::ReturnType::NEW_FILE_INFO:
    {
        QString data(socket->read(packageSize));
        auto info = data.split("\t");
        std::shared_ptr<FileInfo>file(new FileInfo);
        file->file_id = info[0].toInt();
        file->file_name = info[1];
        file->file_user = info[2];
        file->file_path = info[3];
        file->file_project = info[4].toInt();
        file->file_privilege = info[5].toShort();

        pro_fileMap.value(file->file_project).append(file);

        //x01234/678
        QString dir_path = file->file_path.mid(0,file->file_path.lastIndexOf("/"));
        QStringList dirList = dir_path.split("/");

        auto dir = mainDirMap.value(file->file_project);
        auto item = dir->dir_item;
        for (int i = 3; i < dirList.size(); ++i) {

            bool flag = false;
            for (auto it : dir->sub_dirs) {

                if(it->dir_name == dirList[i])
                {
                    item = it->dir_item;
                    flag = true;
                    dir = it;
                    break;
                }
            }
            if(!flag)
            {
                for(int j = i ; j < dirList.size() ; j++)
                {
                    MyTreeItem* dir_item = new MyTreeItem(MyTreeItem::Type::DIR);
                    auto pdir = item->data(0,Qt::UserRole).value<std::shared_ptr<Directory>>();
                    std::shared_ptr<Directory> dir_ptr(new Directory(file->file_project,dirList[j],pdir->dir_path+dirList[j]+"/",dir_item));
                    QVariant var;
                    var.setValue(dir_ptr);
                    pdir->sub_dirs.insert(dir_ptr->dir_name,dir_ptr);
                    dir_item->setData(0,Qt::UserRole,var);
                    dir_item->setText(0,dirList[j]);
                    item->addChild(dir_item);
                    item = dir_item;
                }
            }
        }

        MyTreeItem* file_item = new MyTreeItem(MyTreeItem::Type::FILE);
        QVariant var;
        var.setValue(file);
        file_item->setData(0,Qt::UserRole,var);
        file_item->setText(0,file->file_name);
        item->addChild(file_item);
        file->file_item = file_item;

        break;
    }
    case (int)Package::ReturnType::PROJECT_FILE_DELETE:
    {
        QString data(socket->read(packageSize));
        QStringList list = data.split("\t");
        int pid = list[0].toInt();
        int fid = list[1].toInt();

        if(pro_fileMap.contains(pid))
        {
            auto vec = pro_fileMap.value(pid);
            for(int i = 0; i < vec.size() ; i++)
            {
                auto file = vec[i];
                if(file->file_id==fid)
                {
                    file->file_item->parent()->removeChild(file->file_item);
                    delete file->file_item;
                    vec.removeAt(i);
                    break;
                }
            }
        }

        if(fileWidgets.contains(fid))
        {
            fileWidgets.value(fid)->deleteLater();
            fileWidgets.remove(fid);
        }
        break;
    }
    case (int)Package::ReturnType::TEXT_CHANGE:
    {
        QString data(socket->read(packageSize));
        QStringList list = data.split("#");
        int file_id = list[0].toInt();
        int pos = list[1].toInt();
        int charRemoved = list[2].toInt();
        if(fileWidgets.contains(file_id))
        {
            CodeEdit* wind = fileWidgets.value(file_id);
            wind->changeText(pos,charRemoved,list[4]);
        }
        break;
    }
    case (int)Package::ReturnType::HEART_PCK:
    {
        this->isAlive = true;
        break;
    }
    case (int)Package::ReturnType::PRIVILEGE_INFO:
    {
        QString data(socket->read(packageSize));
        //QStringList list =
        break;
    }
    default:
        break;
    }
}

void MainWindow::newLocalProj()
{

}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    CodeEdit* wind = (CodeEdit*)ui->tabWidget->widget(index);
    fileWidgets.remove(wind->getFile()->file_id);
    ui->tabWidget->removeTab(index);
    wind->deleteLater();
}

void MainWindow::selectencodingMode()
{
    encodingType->show();
    connect(encodingType->getButtonConfirm(),&QPushButton::clicked,this,[=](){
        if(encodingType->getListWidgetCurrentItem()==encodingType->getItem1())
            EncodingTypeLabel->setText("当前的编码方式为：ASCII");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem2())
            EncodingTypeLabel->setText("当前的编码方式为：UTF-8");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem3())
            EncodingTypeLabel->setText("当前的编码方式为：UTF-16");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem4())
            EncodingTypeLabel->setText("当前的编码方式为：UTF-32");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem5())
            EncodingTypeLabel->setText("当前的编码方式为：GBK");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem6())
            EncodingTypeLabel->setText("当前的编码方式为：ISO-8859-1");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem7())
            EncodingTypeLabel->setText("当前的编码方式为：ISO-8859-2");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem8())
            EncodingTypeLabel->setText("当前的编码方式为：ISO-8859-3");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem9())
            EncodingTypeLabel->setText("当前的编码方式为：ISO-8859-4");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem10())
            EncodingTypeLabel->setText("当前的编码方式为：ISO-8859-5");
        else
            EncodingTypeLabel->setText("当前的编码方式为：ASCII(默认为ASCII编码，如需更改，请打开“设置-编码方式”中进行选择）");
        encodingType->close();
    });

    connect(encodingType->getButtonCancel(),&QPushButton::clicked,this,&QDialog::close);
}




