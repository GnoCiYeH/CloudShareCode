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
#include"privilegemanager.h"


QTcpSocket* MainWindow::socket = new QTcpSocket();
QHash<int,Project>* MainWindow::userProjs = new QHash<int,Project>();
QString MainWindow::userId = "";
bool MainWindow::isLogin = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settingWind = new SettingForm(this);
    settingWind->hide();
    settingWind->setWindowFlag(Qt::Window);

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
    this->setWindowTitle("CloudSharedCoding");
    runbutton = new QToolButton(this);
    runbutton->setIcon(QIcon("://icon/kaishi_yunhang.png"));
    runbutton->setFixedSize(40,40);
    runbutton->setToolTip("Run");
    debugbutton = new QToolButton(this);
    debugbutton->setIcon(QIcon("://icon/debug.png"));
    debugbutton->setFixedSize(40,40);
    debugbutton->setToolTip("Debug");
    stopRun = new QToolButton(this);
    stopRun->setEnabled(false);
    stopRun->setIcon(QIcon(":/qss_icons/light/rc/window_close_focus@2x.png"));
    stopRun->setFixedSize(40,40);
    stopRun->setToolTip("Stop");

    QToolBar* toolbar = new QToolBar(this);
    toolbar->addWidget(runbutton);
    toolbar->addWidget(debugbutton);
    toolbar->addWidget(stopRun);
    this->addToolBar(Qt::RightToolBarArea,toolbar);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    ui->treeWidget->setLayout(layout);
    ui->treeWidget->header()->hide();
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tabWidget->removeTab(1);
    ui->actionCloud_project->setText("打开云项目");
    ui->actionLocal_project->setText("打开本地项目");
    ui->actionNew_cloud_project->setText("新建云项目");
    ui->actionNew_local_project->setText("新建本地项目");

    connect(runbutton,SIGNAL(clicked()),this,SLOT(runProject()));
    connect(stopRun,SIGNAL(clicked()),this,SLOT(stopProject()));


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
    EncodingTypeLabel->setText("        当前的编码方式为：UTF-8 (默认为UTF-8编码，如需更改，请打开“设置-编码方式”中进行选择）");
    status_bar->addWidget(EncodingTypeLabel);
    EncodingTypeLabel->setAlignment(Qt::AlignCenter);

    //右键菜单槽
    connect(openFile,SIGNAL(triggered(bool)),this,SLOT(openProjFile()));
    connect(newFile,SIGNAL(triggered(bool)),this,SLOT(newProFile()));
    connect(deleteFile,SIGNAL(triggered(bool)),this,SLOT(deleteProFile()));

    //主菜单栏槽
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(close()));

    //*************************************************************************************************
    connect(ui->actionCloud_project,SIGNAL(triggered()),this,SLOT(openCloudProj()));
    connect(ui->actionNew_cloud_project,SIGNAL(triggered()),this,SLOT(newCloudProj()));
    connect(ui->actionSetting,SIGNAL(triggered()),this,SLOT(showSetting()));

    connect(ui->actionLocal_project,SIGNAL(triggered()),this,SLOT(openLocalProj()));
    connect(ui->actionSave,&QAction::triggered,this,&MainWindow::saveLocalProj);

    connect(ui->actionNew_local_project,SIGNAL(triggered()),this,SLOT(newLocalProj()));
    //*************************************************************************************************

    connect(ui->Setting,SIGNAL(triggered()),this,SLOT(openSettingDialog()));

    //socket
    connect(socket,SIGNAL(readyRead()),this,SLOT(dataProgress()));

    //子窗口
    projectForm = new ProjectForm(this);
    projectForm->setWindowFlag(Qt::Window);
    loginDialog=new LoginDialog(this);

    buildDock = new QDockWidget(this);
    buildDockwidget = new QTextEdit(buildDock);
    buildDockwidget->setFocusPolicy(Qt::NoFocus);
    buildDock->setWidget(buildDockwidget);
    buildDock->setWindowTitle("构建");
    this->addDockWidget(Qt::BottomDockWidgetArea,buildDock);

    runDock = new QDockWidget(this);
    runDock->setWindowTitle("控制台");
    runDockwidget = new QTextEdit(runDock);
    runDockwidget->setFocusPolicy(Qt::NoFocus);
    runDock->setWidget(runDockwidget);
    this->addDockWidget(Qt::BottomDockWidgetArea,runDock);
    connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int,int,int)));

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
    mp.clear();
    delete ui;
    socket->close();
    socket->deleteLater();
    delete userProjs;
}

void MainWindow::cmdStdin(int pos,int charRemoved,int charAdded)
{
    qDebug() << pos << " " << charRemoved << " " << charAdded;
    QString data = Package::intToByteArr(currentProject);
    for (int var = 0; var < charRemoved; ++var) {
        data+="\b \b";
    }
    for (int var = pos; var < pos + charAdded; ++var)
    {
        if (runDockwidget->document()->characterAt(var) == QChar(8233) || runDockwidget->document()->characterAt(var) == QChar(8232))
        {
            if (charRemoved == 1 && charAdded == 1)
            {
                return;
            }
            data += "\n";
        }
        else
            data += runDockwidget->document()->characterAt(var);
    }
    qDebug() << data;
    Package pck(data.toUtf8(), (int)Package::PackageType::POST_STDIN);
    MainWindow::socket->write(pck.getPdata(), pck.getSize());
    MainWindow::socket->flush();
}

void MainWindow::stopProject()
{
    if(runningProject<0)
    {

    }
    else{
        runDockwidget->setEnabled(false);
        Package pck(QString::number(runningProject).toUtf8(),(int)Package::PackageType::KILL_PROJECT);
        socket->write(pck.getPdata(),pck.getSize());
    }
}

//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
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
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************

void MainWindow::showSetting()
{
    settingWind->show();
}

void MainWindow::deleteProFile()
{
    auto item = (MyTreeItem*)ui->treeWidget->currentItem();
    auto var = item->data(0,Qt::UserRole);
    if(item->getType()==MyTreeItem::FILE)
    {
        auto file = var.value<std::shared_ptr<FileInfo>>();

        QMessageBox::StandardButton result = QMessageBox::warning(this,"确定删除？","您确定要删除文件："+file->file_name+"?");
        if(result!=QMessageBox::StandardButton::Ok)
            return;

        QString data = QString::number(file->file_id) + "\t" + file->file_path + "\t" + QString::number(file->file_project);
        Package pck(data.toUtf8(),(int)Package::PackageType::DEL_FILE);

        socket->write(pck.getPdata(),pck.getSize());
    }
    else
    {
//        auto dir = var.value<std::shared_ptr<Directory>>();

//        QMessageBox::StandardButton result = QMessageBox::warning(this,"确定删除？","您确定要删除文件："+dir->dir_name+"?");
//        if(result!=QMessageBox::StandardButton::Ok)
//            return;

//        QString data = QString::number(dir->file_id) + "\t" + proj->pro_ + "\t" + QString::number(proj->pro_id);
//        Package pck(data.toUtf8(),(int)Package::PackageType::DEL_FILE);

//        socket->write(pck.getPdata(),pck.getSize());
    }
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

bool MainWindow::addFileWidget(std::shared_ptr<FileInfo> file)
{
    auto projPrivilege = MainWindow::userProjs->find(file->file_project)->pro_privilege_level;
    switch (projPrivilege) {
    case 2:
    {
        if(file->file_privilege==0)
        {
            QMessageBox::about(this,"tips","Sorry, you don't have enough permissions to open this file!");
            file->is_open=false;
            return false;
        }
        break;
    }
    case 3:
    {
        if(file->file_privilege==1)
        {
            QMessageBox::about(this,"tips","Sorry, you don't have enough permissions to open this file!");
            file->is_open=false;
            return false;
        }
        break;
    }
    default:
        break;
    }
    CodeEdit* widget = new CodeEdit(file,this);
    ui->tabWidget->addTab(widget,file->file_name);
    fileWidgets.insert(file->file_id,widget);
    file->is_open = true;
    return true;
}

void MainWindow::openProjFile()
{
    MyTreeItem* item = (MyTreeItem*)ui->treeWidget->currentItem();
    QVariant var = item->data(0,Qt::UserRole);
    std::shared_ptr<FileInfo> file = var.value<std::shared_ptr<FileInfo>>();

    if(!file->is_open)
    {
        if(addFileWidget(file))
        {
            Package pck(QString::number(file->file_id).toUtf8(),(int)Package::PackageType::GET_FILE);
            socket->write(pck.getPdata(),pck.getSize());
            socket->flush();
        }
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

//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
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
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************

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
    this->currentProject=id;
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
        item->setIcon(0,QIcon("://icon/PROJECT.png"));
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
                    item->setIcon(0,QIcon("://icon/Directory-tree.png"));
                }
                else
                {
                    MyTreeItem* item = new MyTreeItem(MyTreeItem::Type::FILE);
                    item->setText(0,file->file_name);
                    if(file->file_name.mid(file->file_name.lastIndexOf('.'))==".cpp")
                    {
                        item->setIcon(0,QIcon("://icon/cpp.png"));
                    }
                    else if(file->file_name.mid(file->file_name.lastIndexOf('.'))==".h")
                    {
                        item->setIcon(0,QIcon("://icon/H-.png"));
                    }
                    else
                    {
                        item->setIcon(0,QIcon("://icon/24gl-fileEmpty.png"));
                    }
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
        if(file->file_name.mid(file->file_name.lastIndexOf('.'))==".cpp")
        {
            file_item->setIcon(0,QIcon("://icon/cpp.png"));
        }
        else if(file->file_name.mid(file->file_name.lastIndexOf('.'))==".h")
        {
            file_item->setIcon(0,QIcon("://icon/H-.png"));
        }
        else
        {
            file_item->setIcon(0,QIcon("://icon/24gl-fileEmpty.png"));
        }
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
            int position = (list[0]+list[1]+list[2]+list[3]+list[4]).length()+4;
            CodeEdit* wind = fileWidgets.value(file_id);
            wind->changeText(pos,charRemoved,list[4],data.mid(position+1));
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

        PrivilegeManager* manager = new PrivilegeManager(data,this);
        manager->setWindowFlag(Qt::Window);
        manager->show();
        break;
    }
    case (int)Package::ReturnType::SERVER_OK:
    {
        QString data(socket->read(packageSize));
        QMessageBox::about(this,"Tips",data);
        break;
    }
    case (int)Package::ReturnType::BUILD_INFO:
    {
        QString data(socket->read(packageSize));
        disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int,int,int)));
        buildDockwidget->insertPlainText(data);
        connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int,int,int)));
        buildDockwidget->verticalScrollBar()->setValue(buildDockwidget->verticalScrollBar()->maximum());
        break;
    }
    case (int)Package::ReturnType::RUN_INFO:
    {
        if(runDockwidget->isEnabled())
        {
            stopRun->setEnabled(true);
            runDockwidget->setFocusPolicy(Qt::StrongFocus);
            QString data(socket->read(packageSize));
            disconnect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int,int,int)));
            runDockwidget->insertPlainText(data);
            connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int,int,int)));
            runDockwidget->verticalScrollBar()->setValue(runDockwidget->verticalScrollBar()->maximum());
        }
        else
        {
            socket->read(packageSize);
        }
        break;
    }
    case (int)Package::ReturnType::BUILD_FINISH:
    {
        //
        break;
    }
    case (int)Package::ReturnType::RUN_FINISH:
    {
        runbutton->setEnabled(true);
        debugbutton->setEnabled(true);
        stopRun->setEnabled(false);
        runDockwidget->setEnabled(true);
        QString data(socket->read(packageSize));
        runDockwidget->append(data);
        break;
    }
    default:
        break;
    }
}



void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    CodeEdit* wind = (CodeEdit*)ui->tabWidget->widget(index);
    fileWidgets.remove(wind->getFile()->file_id);
    ui->tabWidget->removeTab(index);
    wind->getFile()->is_open=false;
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
            EncodingTypeLabel->setText("当前的编码方式为：UTF-8(默认为UTF-8编码，如需更改，请打开“设置-编码方式”中进行选择）");
        encodingType->close();
    });

    connect(encodingType->getButtonCancel(),&QPushButton::clicked,this,&QDialog::close);
}





void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    MyTreeItem* treeItem = (MyTreeItem*) item;
    if(treeItem->getType()!=MyTreeItem::FILE)
    {
        return;
    }

    openProjFile();
}

//打开本地项目文件
void MainWindow::openLocalProj()
{
    QString path=QFileDialog::getOpenFileName(this,"打开文件","C://Users");
    //文件的信息 info实例化file_information
    QFileInfo info(path);
    std::shared_ptr<FileInfo> file_information(new FileInfo);
    file_information->file_name=info.fileName();
    file_information->file_path=info.filePath();

    //file_information构造出一个code_edit文本编辑器
    CodeEdit* code_edit=new CodeEdit(file_information,this);

    //新建一个tab加入到tabWidget中
    ui->tabWidget->addTab(code_edit,file_information->file_name);
    file_information->is_open=true;

    //读取文件的内容并打印到code_edit编辑器
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray array=file.readAll();
    code_edit->addText(array);

    //一个path对应一个code_edit指针，添加到映射表中
    mp[file_information->file_path]=code_edit;
}

//保存本地项目文件
void MainWindow::saveLocalProj()
{
    QFileDialog file_dialog;
    //获取文件的路径
    QString file_path=file_dialog.getSaveFileName(this,tr("打开文件"));

    if(file_path=="")
        return;
    QFile file(file_path);

    //在map映射表中进行查询此路径是否对应有code_edit
    if(mp.count(file_path)==1)//打开的方式进行保存
    {
        CodeEdit* code_edit=mp[file_path];
        if(file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QTextStream cout(&file);
            QString str=code_edit->getText();
            cout<<str;
            QMessageBox::information(this,"提示","保存成功");
            file.close();
        }
        else
        {
            QMessageBox::critical(this,"错误","保存失败");
            return;
        }
    }
    else//新建的方式进行保存
    {

    }
}


//新建本地项目文件并打开
void MainWindow::newLocalProj()
{
    NewLocalFile* dialog=new NewLocalFile(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    //确定路径以及文件名称
    connect(dialog->getPushButtonConfrim(),&QPushButton::clicked,this,[=](){
        if(dialog->getLineEdit_Name()->text()=="")
        {
            QMessageBox::warning(this,"警告","请输入文件名称");
            return;
        }
        else if(!dialog->isLegal(dialog->getLineEdit_Name()->text()))
        {
            QMessageBox::warning(this,"警告","请输入合法的文件名（只允许包含字母和数字）");
            return;
        }
        else
        {
            dialog->setFileName(dialog->getLineEdit_Name()->text()+".txt");
            dialog->setFilePath(dialog->getFileTempPath()+"/"+dialog->getFileName());
            QString file_path=dialog->getFilePath();
            QFile *new_file=new QFile(this);
            new_file->setFileName(file_path);
            bool res=new_file->open(QIODevice::ReadWrite|QIODevice::Text);
            new_file->close();
            if(!res)
            {
                QMessageBox::critical(this,"错误","文件新建失败");
                return;
            }
            else
            {
                QMessageBox::information(this,"新建文件","新建文件成功");
                QFileInfo info(dialog->getFilePath());
                std::shared_ptr<FileInfo> file_information(new FileInfo);
                file_information->file_name=info.fileName();
                file_information->file_path=info.filePath();

                //file_information构造出一个code_edit文本编辑器
                CodeEdit* code_edit=new CodeEdit(file_information,this);

                //新建一个tab加入到tabWidget中
                ui->tabWidget->addTab(code_edit,file_information->file_name);
                file_information->is_open=true;

                //读取文件的内容并打印到code_edit编辑器
                QFile file(dialog->getFilePath());
                file.open(QIODevice::ReadOnly);
                QByteArray array=file.readAll();
                code_edit->addText(array);

                //一个path对应一个code_edit指针，添加到映射表中
                mp[file_information->file_path]=code_edit;

                dialog->close();
                return;
            }
        }
    });
}

//run project
void MainWindow::runProject()
{
    runbutton->setEnabled(false);
    debugbutton->setEnabled(false);

    disconnect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int,int,int)));
    runDockwidget->clear();
    connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int,int,int)));

//    auto item = (MyTreeItem*)ui->treeWidget->currentItem();
//    bool is_local = true;
//    int pro_id;
//    switch (item->getType()) {
//    case MyTreeItem::DIR:
//    {
//        if((pro_id = item->data(0,Qt::UserRole).value<std::shared_ptr<Directory>>()->pro_id)!=-1)
//        {
//            is_local = false;
//        }
//        break;
//    }
//    case MyTreeItem::FILE:
//    {
//        if((pro_id = item->data(0,Qt::UserRole).value<std::shared_ptr<FileInfo>>()->file_project)!=-1)
//        {
//            is_local = false;
//        }
//        break;
//    }
//    case MyTreeItem::PROJECT:
//    {
//        if((pro_id = item->data(0,Qt::UserRole).value<std::shared_ptr<Project>>()->pro_id)!=-1)
//        {
//            is_local = false;
//        }
//        break;
//    }
//    default:
//        break;
//    }

    if(currentProject<0)
    {
        runningProject = currentProject;
    }
    else
    {
        runningProject = currentProject;
        Package pck(QString::number(currentProject).toUtf8(),(int)Package::PackageType::RUN_PROJECT);
        socket->write(pck.getPdata(),pck.getSize());
    }
}
