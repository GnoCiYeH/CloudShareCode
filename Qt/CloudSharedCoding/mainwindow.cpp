#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myhelper.h"
#include "projectform.h"
#include "logindialog.h"
#include <QAction>
#include <QFileDialog>
#include "package.h"
#include "newprojectdialog.h"
#include "newfiledialog.h"
#include <QPoint>
#include "privilegemanager.h"
#include "newlocalproject.h"

QTcpSocket *MainWindow::socket = new QTcpSocket();
QStringList *MainWindow::fileName = new QStringList();
QHash<int, Project> *MainWindow::userProjs = new QHash<int, Project>();
QString MainWindow::userId = "";
QHash<int, QMultiHash<QString, int> *> *MainWindow::debugInfo = new QHash<int, QMultiHash<QString, int> *>();
bool MainWindow::isLogin = false;
LoginDialog *MainWindow::loginDialog;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    tree_widget_item_file_information->setText(0, "项目信息.txt");
    tree_widget_item_source_file_name->setText(0, "源文�?");
    tree_widget_item_header_file_name->setText(0, "头文�?");

    ui->setupUi(this);
    settingWind = new SettingForm(this);
    settingWind->hide();
    settingWind->setWindowFlag(Qt::Window);

    encodingType->setAttribute(Qt::WA_DeleteOnClose);

    socket = new QTcpSocket(this);
    heartTimer = new QTimer(this);
    detectTimer = new QTimer(this);
    connect(heartTimer, &QTimer::timeout, this, [=]()
            {
        Package pck("",(int)Package::PackageType::HEART_PCK);
        socket->write(pck.getPdata(),pck.getSize()); });
    connect(detectTimer, &QTimer::timeout, this, [=]() mutable
            {
        if(!isAlive)
            {
            qDebug()<<"离线！！！！";
        }
        isAlive = false; });

    // 设置主窗口基本属�?
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("CloudSharedCoding");
    runbutton = new QToolButton(this);
    runbutton->setIcon(QIcon("://icon/kaishi_yunhang.png"));
    runbutton->setFixedSize(40, 40);
    runbutton->setToolTip("Run");
    debugbutton = new QToolButton(this);
    debugbutton->setIcon(QIcon("://icon/debug.png"));
    debugbutton->setFixedSize(40, 40);
    debugbutton->setToolTip("Debug");
    stopRun = new QToolButton(this);
    stopRun->setEnabled(false);
    stopRun->setIcon(QIcon(":/qss_icons/light/rc/window_close_focus@2x.png"));
    stopRun->setFixedSize(40, 40);
    stopRun->setToolTip("Stop");

    QToolBar *toolbar = new QToolBar(this);
    toolbar->addWidget(runbutton);
    toolbar->addWidget(debugbutton);
    toolbar->addWidget(stopRun);
    this->addToolBar(Qt::RightToolBarArea, toolbar);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    ui->treeWidget->setLayout(layout);
    ui->treeWidget->header()->hide();
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tabWidget->removeTab(1);
    ui->actionCloud_project->setText("打开云项�?");
    ui->actionOpenLocal_project->setText("打开本地项目");
    ui->actionNew_cloud_project->setText("新建云项�?");
    ui->actionNew_local_project->setText("新建本地项目");

    connect(runbutton, SIGNAL(clicked()), this, SLOT(runProject()));
    connect(stopRun, SIGNAL(clicked()), this, SLOT(stopProject()));
    connect(debugbutton, SIGNAL(clicked()), this, SLOT(debugProject()));

    // 右键菜单
    submitProject = new QAction("提交项目", ui->treeWidget);
    closeProject = new QAction("关闭项目", ui->treeWidget);
    newFile = new QAction("新建文件", ui->treeWidget);
    deleteFile = new QAction("删除", ui->treeWidget);
    openFile = new QAction("打开", ui->treeWidget);
    attribute = new QAction("属�?", ui->treeWidget);
    rename = new QAction("重命�?", ui->treeWidget);

    // 状态栏(显示状态和时间�?
    status_bar->setContentsMargins(0, 0, 0, 0);
    setStatusBar(status_bar);
    status_bar->addWidget(statusIcon);
    status_bar->addWidget(label1);
    status_bar->setStyleSheet("color::rgb(0,0,0");
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, [=]()
            {
        QString str;
        QDateTime time_date=QDateTime::currentDateTime();
        str=time_date.toString("yyyy-MM-dd hh:mm:ss");
        label2->setText(str);
        status_bar->addPermanentWidget(label2); });

    statusIcon->setContentsMargins(0, 0, 0, 0);
    QSize size = QSize(25, 25);
    statusIcon->setFixedSize(size);
    stateokMovie->setScaledSize(size);
    buildingMovie->setScaledSize(size);
    debugingMovie->setScaledSize(size);
    stateokMovie->setScaledSize(size);

    stateokMovie->start();
    statusIcon->setMovie(stateokMovie);

    // 在状态栏显示编码方式
    EncodingTypeLabel->setText("        当前的编码方式为：UTF-8 (默认为UTF-8编码，如需更改，请打开“设�?-编码方式”中进行选择�?");
    status_bar->addWidget(EncodingTypeLabel);
    EncodingTypeLabel->setAlignment(Qt::AlignCenter);

    // 右键菜单�?
    connect(openFile, SIGNAL(triggered(bool)), this, SLOT(openProjFile()));
    connect(newFile, SIGNAL(triggered(bool)), this, SLOT(newProFile()));
    connect(deleteFile, SIGNAL(triggered(bool)), this, SLOT(deleteProFile()));

    // 主菜单栏�?
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionCloud_project, SIGNAL(triggered()), this, SLOT(openCloudProj()));
    connect(ui->actionNew_cloud_project, SIGNAL(triggered()), this, SLOT(newCloudProj()));
    connect(ui->actionSetting, SIGNAL(triggered()), this, SLOT(showSetting()));

    //*************************************************************************************************
    connect(ui->actionNew_local_project, SIGNAL(triggered()), this, SLOT(newLocalProj()));   // 新建本地项目
    connect(ui->actionAdd_Local_File, &QAction::triggered, this, &MainWindow::addLocalFile); // 添加本地文件
    connect(ui->actionOpenLocal_project, SIGNAL(triggered()), this, SLOT(openLocalProj()));  // 打开本地项目
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveLocalProj);          // 保存本地项目
    //*************************************************************************************************

    connect(ui->Setting, SIGNAL(triggered()), this, SLOT(openSettingDialog()));

    // socket
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataProgress()));

    // 子窗�?
    projectForm = new ProjectForm(this);
    projectForm->setWindowFlag(Qt::Window);
    loginDialog = new LoginDialog(this);

    buildDock = new QDockWidget(this);
    buildDockwidget = new QTextEdit(buildDock);
    buildDockwidget->setFocusPolicy(Qt::NoFocus);
    buildDock->setWidget(buildDockwidget);
    buildDock->setWindowTitle("构建");
    this->addDockWidget(Qt::BottomDockWidgetArea, buildDock);

    runDock = new QDockWidget(this);
    runDock->setWindowTitle("控制�?");
    runDockwidget = new QTextEdit(runDock);
    runDockwidget->setFocusPolicy(Qt::NoFocus);
    runDock->setWidget(runDockwidget);
    this->addDockWidget(Qt::BottomDockWidgetArea, runDock);
    connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));

    stackDock = new QDockWidget(this);
    stackList = new QListWidget(stackDock);
    stackDock->setHidden(true);
    stackDock->setWidget(stackList);
    connect(stackList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(gotoCodeLine(QListWidgetItem *)));

    breakPointDock = new QDockWidget(this);
    varDock = new QDockWidget(this);
    varInfo = new QTableWidget(varDock);
    breakPointInfo = new QListWidget(breakPointDock);
    breakPointDock->setHidden(true);
    varDock->setHidden(true);
    stackDock->setHidden(true);
    breakPointDock->setWidget(breakPointInfo);
    varDock->setWidget(varInfo);
    this->setDockNestingEnabled(false);
    this->addDockWidget(Qt::RightDockWidgetArea, varDock);
    this->setDockNestingEnabled(true);
    this->addDockWidget(Qt::RightDockWidgetArea, stackDock);
    this->addDockWidget(Qt::RightDockWidgetArea, breakPointDock);

    connect(breakPointInfo, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(gotoCodeLine(QListWidgetItem *)));

    varInfo->setColumnCount(2);
    varInfo->setHorizontalHeaderLabels({"变量", "�?"});

    debugToolBar = new QToolBar(this);
    this->addToolBar(Qt::ToolBarArea::RightToolBarArea, debugToolBar);
    debugToolBar->setHidden(true);

    continueDebugButton = new QToolButton(debugToolBar);
    continueDebugButton->setIcon(QIcon("://icon/continue.png"));
    continueDebugButton->setDisabled(true);
    connect(continueDebugButton, &QToolButton::clicked, this, [=]()
            {
        stackList->clear();
        QString data = Package::intToByteArr(currentProject);
        data+="c\ninfo local\nbacktrace\n";
        varInfo->clearContents();
        varInfo->setRowCount(0);
        Package pck(data.toUtf8(),(int)Package::PackageType::POST_STDIN);
        socket->write(pck.getPdata(),pck.getSize());

        continueDebugButton->setDisabled(true);
        nextDebugButton->setDisabled(true);
        stepIntoDubugButton->setDisabled(true);
        stepOutDebugButton->setDisabled(true); });

    stopDebugButton = new QToolButton(debugToolBar);
    stopDebugButton->setIcon(QIcon("://icon/stop.png"));
    stopDebugButton->setDisabled(true);
    connect(stopDebugButton, &QToolButton::clicked, this, [=]()
            {
        QString data = Package::intToByteArr(currentProject);
        data+="q\ny\n";
        Package pck(data.toUtf8(),(int)Package::PackageType::POST_STDIN);
        socket->write(pck.getPdata(),pck.getSize());

        stopDebugButton->setDisabled(true);
        continueDebugButton->setDisabled(true);
        nextDebugButton->setDisabled(true);
        stepIntoDubugButton->setDisabled(true);
        stepOutDebugButton->setDisabled(true); });

    nextDebugButton = new QToolButton(debugToolBar);
    nextDebugButton->setIcon(QIcon("://icon/next.png"));
    nextDebugButton->setDisabled(true);
    connect(nextDebugButton, &QToolButton::clicked, this, [=]()
            {
        fileWidgets.value(currentLine.first->file_id)->gotoline(currentLine.second+1);
        currentLine.second+=1;

        QString data = Package::intToByteArr(currentProject);
        stackList->clear();
        data+="n\ninfo local\nbacktrace\n";
        varInfo->clearContents();
        varInfo->setRowCount(0);
        Package pck(data.toUtf8(),(int)Package::PackageType::POST_STDIN);
        socket->write(pck.getPdata(),pck.getSize());

        continueDebugButton->setDisabled(true);
        nextDebugButton->setDisabled(true);
        stepIntoDubugButton->setDisabled(true);
        stepOutDebugButton->setDisabled(true); });

    stepIntoDubugButton = new QToolButton(debugToolBar);
    stepIntoDubugButton->setIcon(QIcon("://icon/stepinto.png"));
    stepIntoDubugButton->setDisabled(true);
    connect(stepIntoDubugButton, &QToolButton::clicked, this, [=]()
            {
        stackList->clear();
        QString data = Package::intToByteArr(currentProject);
        data+="step\ninfo local\nbacktrace\n";
        varInfo->clearContents();
        varInfo->setRowCount(0);
        Package pck(data.toUtf8(),(int)Package::PackageType::POST_STDIN);
        socket->write(pck.getPdata(),pck.getSize());

        continueDebugButton->setDisabled(true);
        nextDebugButton->setDisabled(true);
        stepIntoDubugButton->setDisabled(true);
        stepOutDebugButton->setDisabled(true); });

    stepOutDebugButton = new QToolButton(debugToolBar);
    stepOutDebugButton->setIcon(QIcon("://icon/stepout.png"));
    stepOutDebugButton->setDisabled(true);
    connect(stepOutDebugButton, &QToolButton::clicked, this, [=]()
            {
        stackList->clear();
        QString data = Package::intToByteArr(currentProject);
        data+="finish\ninfo local\nbacktrace\n";
        varInfo->clearContents();
        varInfo->setRowCount(0);
        Package pck(data.toUtf8(),(int)Package::PackageType::POST_STDIN);
        socket->write(pck.getPdata(),pck.getSize());

        continueDebugButton->setDisabled(true);
        nextDebugButton->setDisabled(true);
        stepIntoDubugButton->setDisabled(true);
        stepOutDebugButton->setDisabled(true); });

    debugToolBar->addWidget(continueDebugButton);
    debugToolBar->addWidget(stopDebugButton);
    debugToolBar->addWidget(nextDebugButton);
    debugToolBar->addWidget(stepIntoDubugButton);
    debugToolBar->addWidget(stepOutDebugButton);

    // 子窗口槽
    connect(this, &MainWindow::loginAllowed, loginDialog, &LoginDialog::loginSucceed);
    connect(this, SIGNAL(projInited()), projectForm, SLOT(init()));
    connect(projectForm, SIGNAL(openProj(int)), this, SLOT(openProj(int)));
    connect(ui->treeWidget, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(projectItemPressedSlot(QTreeWidgetItem *, int)));

    // 选择编码方式
    connect(ui->actionSwitching, &QAction::triggered, this, &MainWindow::selectencodingMode);

    // 关闭
    connect(ui->actionClose, &QAction::triggered, this, &QMainWindow::close);

    // 新建文件
    connect(ui->new_file_action, &QAction::triggered, this, [=]()
            { QFileDialog::getOpenFileName(this, "新建文件", "C:/Users"); });

    // 添加文件
    connect(ui->actionAdd_Cloud_File, &QAction::triggered, this, [=]()
            { QFileDialog::getOpenFileName(this, "添加文件", "C:/Users"); });
    // connect(ui->add_file_action,&QAction::triggered,this,[=](){QFileDialog::getOpenFileName(this,"添加文件","C:/Users");});

    setSystemVar("D:\\mingw64\\bin");
    findFileName(systemVar);
}

MainWindow::~MainWindow()
{
    mp.clear();
    delete ui;
    socket->close();
    socket->deleteLater();
    delete userProjs;
    delete debugInfo;
    delete tree_widget_item_project_name;
    delete tree_widget_item_file_information;
    delete tree_widget_item_header_file_name;
    delete tree_widget_item_source_file_name;
    delete loginDialog;
    delete fileName;
}

void MainWindow::gotoCodeLine(QListWidgetItem *item)
{
    QPair<QString, int> pair = item->data(Qt::UserRole).value<QPair<QString, int>>();
    QString path = pair.first;
    int lineNum = pair.second;
    auto vec = pro_fileMap.value(runningProject);
    std::shared_ptr<FileInfo> file;
    for (auto it : vec)
    {
        if (it->file_path == path)
        {
            file = it;
            break;
        }
    }
    if (file.get())
    {
        CodeEdit *widget = fileWidgets.value(file->file_id);
        ui->tabWidget->setCurrentWidget(widget);
        widget->gotoline(lineNum);
    }
}

void MainWindow::cmdStdin(int pos, int charRemoved, int charAdded)
{
    QString data = Package::intToByteArr(currentProject);
    for (int var = 0; var < charRemoved; ++var)
    {
        data += "\b \b";
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
    Package pck(data.toUtf8(), (int)Package::PackageType::POST_STDIN);
    MainWindow::socket->write(pck.getPdata(), pck.getSize());
    MainWindow::socket->flush();
}

void MainWindow::stopProject()
{
    if (runningProject < 0)
    {
    }
    else
    {
        runDockwidget->setEnabled(false);
        Package pck(QString::number(runningProject).toUtf8(), (int)Package::PackageType::KILL_PROJECT);
        socket->write(pck.getPdata(), pck.getSize());
    }
}

void MainWindow::debugProject()
{
    statusIcon->movie()->stop();
    buildingMovie->start();
    statusIcon->setMovie(buildingMovie);
    ui->tabWidget->setTabsClosable(false);

    stopDebugButton->setDisabled(false);

    runningProject = currentProject;
    varDock->setHidden(false);
    breakPointDock->setHidden(false);
    stackDock->setHidden(false);

    disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
    buildDockwidget->clear();
    connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
    disconnect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
    runDockwidget->clear();
    connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));

    breakPointInfo->clear();
    varInfo->clearContents();
    varInfo->setRowCount(0);
    stackList->clear();

    if (workState == ProjectWorkState::DEBUGING || workState == ProjectWorkState::RUNNING)
    {
        QString data = QString::number(userProjs->value(runningProject).pro_id);
        Package pck(data.toUtf8(), (int)Package::PackageType::KILL_PROJECT_FORCE);
        socket->write(pck.getPdata(), pck.getSize());
    }

    debugToolBar->setHidden(false);
    workState = ProjectWorkState::DEBUGING;
    QString data = QString::number(userProjs->value(currentProject).pro_id);
    Package pck(data.toUtf8(), (int)Package::PackageType::DEBUG_PROJECT);
    socket->write(pck.getPdata(), pck.getSize());
}

void MainWindow::newCloudProj()
{
    // 若用户未登录则无法使用在线功能，弹出登录界面
    if (!isLogin)
    {
        Login();
        if (isLogin)
        {
            // 从服务器拉取项目信息
            Package pck("", (int)Package::PackageType::INIT_PROJS);
            socket->write(pck.getPdata(), pck.getSize());
        }
    }

    // 登录成功才可进行下列操作
    if (isLogin)
    {
        NewProjectDialog dialog(false, this);
        dialog.exec();
    }
}

void MainWindow::showSetting()
{
    settingWind->show();
}

void MainWindow::deleteProFile()
{
    auto item = (MyTreeItem *)ui->treeWidget->currentItem();
    auto var = item->data(0, Qt::UserRole);
    if (item->getType() == MyTreeItem::FILE)
    {
        auto file = var.value<std::shared_ptr<FileInfo>>();

        QMessageBox::StandardButton result = QMessageBox::warning(this, "确定删除�?", "您确定要删除文件�?" + file->file_name + "?");
        if (result != QMessageBox::StandardButton::Ok)
            return;

        QString data = QString::number(file->file_id) + "\t" + file->file_path + "\t" + QString::number(file->file_project);
        Package pck(data.toUtf8(), (int)Package::PackageType::DEL_FILE);

        socket->write(pck.getPdata(), pck.getSize());
    }
    else
    {
        //        auto dir = var.value<std::shared_ptr<Directory>>();

        //        QMessageBox::StandardButton result = QMessageBox::warning(this,"确定删除�?","您确定要删除文件�?"+dir->dir_name+"?");
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
    QVariant var = item->data(0, Qt::UserRole);
    auto dir = var.value<std::shared_ptr<Directory>>();

    if (dir->pro_id != -1)
    {
        // 向服务器发送请�?
        NewFileDialog wind(dir, this);
        wind.exec();
    }
}

bool MainWindow::addFileWidget(std::shared_ptr<FileInfo> file)
{
    auto projPrivilege = MainWindow::userProjs->find(file->file_project)->pro_privilege_level;
    switch (projPrivilege)
    {
    case 2:
    {
        if (file->file_privilege == 0)
        {
            QMessageBox::about(this, "tips", "Sorry, you don't have enough permissions to open this file!");
            file->is_open = false;
            return false;
        }
        break;
    }
    case 3:
    {
        if (file->file_privilege == 1)
        {
            QMessageBox::about(this, "tips", "Sorry, you don't have enough permissions to open this file!");
            file->is_open = false;
            return false;
        }
        break;
    }
    default:
        break;
    }
    CodeEdit *widget = new CodeEdit(file, this);
    ui->tabWidget->addTab(widget, file->file_name);
    fileWidgets.insert(file->file_id, widget);
    file->is_open = true;
    return true;
}

void MainWindow::openProjFile()
{
    MyTreeItem *item = (MyTreeItem *)ui->treeWidget->currentItem();
    QVariant var = item->data(0, Qt::UserRole);
    std::shared_ptr<FileInfo> file = var.value<std::shared_ptr<FileInfo>>();

    if (!file->is_open)
    {
        if (addFileWidget(file))
        {
            Package pck(QString::number(file->file_id).toUtf8(), (int)Package::PackageType::GET_FILE);
            socket->write(pck.getPdata(), pck.getSize());
            socket->flush();
        }
    }
    else
    {
        QWidget *widget = fileWidgets.value(file->file_id);
        ui->tabWidget->setCurrentWidget(widget);
    }
}

void MainWindow::openProjFile(std::shared_ptr<FileInfo> file)
{
    if (file->file_id >= 0)
    {
        if (addFileWidget(file))
        {
            Package pck(QString::number(file->file_id).toUtf8(), (int)Package::PackageType::GET_FILE);
            socket->write(pck.getPdata(), pck.getSize());
            socket->flush();
        }
    }
    else
    {
    }
}

void MainWindow::projectItemPressedSlot(QTreeWidgetItem *i, int column)
{
    if (qApp->mouseButtons() == Qt::RightButton)
    {
        QMenu *menu = new QMenu(ui->treeWidget);
        MyTreeItem *item = (MyTreeItem *)i;

        switch (item->getType())
        {
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
    // 若用户未登录则无法使用在线功能，弹出登录界面
    if (!isLogin)
    {
        Login();
        if (isLogin)
        {
            // 从服务器拉取项目信息
            Package pck("", (int)Package::PackageType::INIT_PROJS);
            socket->write(pck.getPdata(), pck.getSize());
        }
    }

    // 登录成功才可进行下列操作
    if (isLogin)
    {
        projectForm->show();
    }
}

void MainWindow::Login()
{
    loginDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(loginDialog, &LoginDialog::loginAllowded, [=]() mutable
            {
        loginDialog->deleteLater();
        isLogin = true;
        userId = loginDialog->userID; });
    loginDialog->exec();
}

void MainWindow::openProj(int id)
{
    this->currentProject = id;
    debugInfo->insert(id, new QMultiHash<QString, int>());
    Package pck(QString::number(id).toUtf8(), (int)Package::PackageType::GET_PROJECT);
    socket->write(pck.getPdata(), pck.getSize());
}

void MainWindow::dataProgress()
{
    QByteArray arr = socket->read(8);
    int type = Package::ByteArrToInt(arr, 0);
    int packageSize = Package::ByteArrToInt(arr, 4);

    switch (type)
    {
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
        box.setText("错误码：" + QString(arr));
        box.exec();
        break;
    }
    case (int)Package::ReturnType::USER_PROJS:
    {
        QByteArray arr = socket->read(packageSize);
        QString data(arr);
        QStringList rows = data.split("\n", Qt::SkipEmptyParts);

        for (auto i : rows)
        {
            QStringList row = i.split("\t");
            if (row.size())
            {
                int id = row[0].toInt();
                short level = row[3].toShort();
                userProjs->insert(id, Project(id, row[1], row[2], level, row[4]));
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
        Project proj(list[0].toInt(), list[1], list[2], 0, list[3]);
        projectForm->addItem(proj);
        userProjs->insert(proj.pro_id, proj);
        break;
    }
    case (int)Package::ReturnType::PROJ_FILE_INFO:
    {
        QString data(socket->read(packageSize));
        QStringList list = data.split("\n", Qt::SkipEmptyParts);

        QStringList proInfo = list[0].split("\t");
        int pro_id = proInfo[0].toInt();
        MyTreeItem *item = new MyTreeItem(MyTreeItem::Type::PROJECT);
        auto i = userProjs->value(pro_id);
        item->setText(0, i.pro_name);
        QString path = "./" + i.pro_owner + "/" + i.pro_name + "/";
        std::shared_ptr<Directory> mainDir(new Directory(pro_id, i.pro_name, path, item));
        QVariant var;
        var.setValue(mainDir);
        item->setData(0, Qt::UserRole, var);
        item->setIcon(0, QIcon("://icon/PROJECT.png"));
        ui->treeWidget->addTopLevelItem(item);

        mainDirMap.insert(i.pro_id, mainDir);

        if (list.empty())
            return;

        QVector<std::shared_ptr<FileInfo>> fileVec;
        for (int j = 1; j < list.size(); j++)
        {
            auto i = list[j];
            auto info = i.split("\t", Qt::SkipEmptyParts);
            std::shared_ptr<FileInfo> file(new FileInfo);
            file->file_id = info[0].toInt();
            file->file_name = info[1];
            file->file_user = info[2];
            file->file_path = info[3];
            file->file_project = info[4].toInt();
            file->file_privilege = info[5].toShort();

            fileVec.append(file);
        }

        pro_fileMap.insert(pro_id, fileVec);

        for (int i = 0; i < fileVec.size(); i++)
        {
            // Path格式�? ./UserId/ProName/Dir(File)...
            std::shared_ptr<FileInfo> file = fileVec[i];
            QStringList list = file->file_path.split("/", Qt::SkipEmptyParts);
            int size = list.size();
            std::shared_ptr<Directory> dir = mainDir;
            for (int i = 3; i < size; i++)
            {
                if (dir->sub_dirs.contains(list[i]))
                {
                    dir = dir->sub_dirs.value(list[i]);
                }
                else if (i != size - 1)
                {
                    MyTreeItem *item = new MyTreeItem(MyTreeItem::Type::DIR);
                    item->setText(0, list[i]);
                    dir->dir_item->addChild(item);
                    std::shared_ptr<Directory> subDir(new Directory(pro_id, list[i], dir->dir_path + list[i] + "/", item));
                    dir->sub_dirs.insert(list[i], subDir);
                    dir = dir->sub_dirs.value(list[i]);
                    QVariant var;
                    var.setValue(subDir);
                    item->setData(0, Qt::UserRole, var);
                    item->setIcon(0, QIcon("://icon/Directory-tree.png"));
                }
                else
                {
                    MyTreeItem *item = new MyTreeItem(MyTreeItem::Type::FILE);
                    item->setText(0, file->file_name);
                    if (file->file_name.mid(file->file_name.lastIndexOf('.')) == ".cpp")
                    {
                        item->setIcon(0, QIcon("://icon/cpp.png"));
                    }
                    else if (file->file_name.mid(file->file_name.lastIndexOf('.')) == ".h")
                    {
                        item->setIcon(0, QIcon("://icon/H-.png"));
                    }
                    else
                    {
                        item->setIcon(0, QIcon("://icon/24gl-fileEmpty.png"));
                    }
                    QVariant var;
                    var.setValue(file);
                    item->setData(0, Qt::UserRole, var);
                    dir->dir_item->addChild(item);
                    file->file_item = item;
                }
            }
        }
        break;
    }
    case (int)Package::ReturnType::FILE:
    {
        // 接收文件，并显示在CodeEdit�?
        QByteArray temp = socket->read(4);
        int fid = Package::ByteArrToInt(temp, 0);

        QByteArray data = socket->read(packageSize - 4);

        auto fileEditor = (CodeEdit *)fileWidgets.value(fid);

        fileEditor->addText(QString(data));

        break;
    }
    case (int)Package::ReturnType::FILE_TRANSOVER:
    {
        QString temp(socket->read(packageSize));
        int fileId = temp.toInt();
        auto wind = fileWidgets.value(fileId);
        ui->tabWidget->setCurrentWidget(wind);
        if (is_wait_file && currentLine.first->file_id == fileId)
        {
            is_wait_file = false;
            wind->gotoline(currentLine.second);
        }
        break;
    }
    case (int)Package::ReturnType::NEW_FILE_INFO:
    {
        QString data(socket->read(packageSize));
        auto info = data.split("\t");
        std::shared_ptr<FileInfo> file(new FileInfo);
        file->file_id = info[0].toInt();
        file->file_name = info[1];
        file->file_user = info[2];
        file->file_path = info[3];
        file->file_project = info[4].toInt();
        file->file_privilege = info[5].toShort();

        pro_fileMap.value(file->file_project).append(file);

        // x01234/678
        QString dir_path = file->file_path.mid(0, file->file_path.lastIndexOf("/"));
        QStringList dirList = dir_path.split("/");

        auto dir = mainDirMap.value(file->file_project);
        auto item = dir->dir_item;
        for (int i = 3; i < dirList.size(); ++i)
        {

            bool flag = false;
            for (auto it : dir->sub_dirs)
            {

                if (it->dir_name == dirList[i])
                {
                    item = it->dir_item;
                    flag = true;
                    dir = it;
                    break;
                }
            }
            if (!flag)
            {
                for (int j = i; j < dirList.size(); j++)
                {
                    MyTreeItem *dir_item = new MyTreeItem(MyTreeItem::Type::DIR);
                    auto pdir = item->data(0, Qt::UserRole).value<std::shared_ptr<Directory>>();
                    std::shared_ptr<Directory> dir_ptr(new Directory(file->file_project, dirList[j], pdir->dir_path + dirList[j] + "/", dir_item));
                    QVariant var;
                    var.setValue(dir_ptr);
                    pdir->sub_dirs.insert(dir_ptr->dir_name, dir_ptr);
                    dir_item->setData(0, Qt::UserRole, var);
                    dir_item->setText(0, dirList[j]);
                    item->addChild(dir_item);
                    item = dir_item;
                }
            }
        }

        MyTreeItem *file_item = new MyTreeItem(MyTreeItem::Type::FILE);
        QVariant var;
        var.setValue(file);
        file_item->setData(0, Qt::UserRole, var);
        file_item->setText(0, file->file_name);
        if (file->file_name.mid(file->file_name.lastIndexOf('.')) == ".cpp")
        {
            file_item->setIcon(0, QIcon("://icon/cpp.png"));
        }
        else if (file->file_name.mid(file->file_name.lastIndexOf('.')) == ".h")
        {
            file_item->setIcon(0, QIcon("://icon/H-.png"));
        }
        else
        {
            file_item->setIcon(0, QIcon("://icon/24gl-fileEmpty.png"));
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

        if (pro_fileMap.contains(pid))
        {
            auto vec = pro_fileMap.value(pid);
            for (int i = 0; i < vec.size(); i++)
            {
                auto file = vec[i];
                if (file->file_id == fid)
                {
                    file->file_item->parent()->removeChild(file->file_item);
                    delete file->file_item;
                    vec.removeAt(i);
                    break;
                }
            }
        }

        if (fileWidgets.contains(fid))
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
        if (fileWidgets.contains(file_id))
        {
            int position = (list[0] + list[1] + list[2] + list[3] + list[4]).length() + 4;
            CodeEdit *wind = fileWidgets.value(file_id);
            wind->changeText(pos, charRemoved, list[4], data.mid(position + 1));
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

        PrivilegeManager *manager = new PrivilegeManager(data, this);
        manager->setWindowFlag(Qt::Window);
        manager->show();
        break;
    }
    case (int)Package::ReturnType::SERVER_OK:
    {
        QString data(socket->read(packageSize));
        QMessageBox::about(this, "Tips", data);
        break;
    }
    case (int)Package::ReturnType::BUILD_INFO:
    {
        QString data(socket->read(packageSize));
        disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
        buildDockwidget->insertPlainText(data);
        connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
        buildDockwidget->verticalScrollBar()->setValue(buildDockwidget->verticalScrollBar()->maximum());
        break;
    }
    case (int)Package::ReturnType::RUN_INFO:
    {
        if (runDockwidget->isEnabled())
        {
            stopRun->setEnabled(true);
            runDockwidget->setFocusPolicy(Qt::StrongFocus);
            QString data(socket->read(packageSize));
            disconnect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            runDockwidget->insertPlainText(data);
            connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
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
        auto codee = ((CodeEdit *)ui->tabWidget->currentWidget());
        codee->highlightError(buildDockwidget->toPlainText());
        //
        if (workState == ProjectWorkState::DEBUGING)
        {
            statusIcon->movie()->stop();
            statusIcon->setMovie(debugingMovie);
            debugingMovie->start();
            auto breakPoints = debugInfo->value(currentProject);
            QString data = Package::intToByteArr(currentProject);
            for (auto it = breakPoints->constBegin(); it != breakPoints->constEnd(); it++)
            {
                data += "b " + it.key() + ":" + QString::number(it.value()) + "\n";
            }
            data += "r\ninfo local\nbacktrace\n";
            Package pck(data.toUtf8(), (int)Package::PackageType::POST_STDIN);
            socket->write(pck.getPdata(), pck.getSize());
        }
        else
        {
            statusIcon->movie()->stop();
            runningMovie->start();
            statusIcon->setMovie(runningMovie);
        }
        break;
    }
    case (int)Package::ReturnType::RUN_FINISH:
    {
        statusIcon->movie()->stop();
        stateokMovie->start();
        statusIcon->setMovie(stateokMovie);
        if (workState == ProjectWorkState::DEBUGING)
        {
            varDock->setHidden(true);
            stackDock->setHidden(true);
            breakPointDock->setHidden(true);
            debugToolBar->setHidden(true);
        }
        ui->tabWidget->setTabsClosable(true);
        workState = ProjectWorkState::NONE;
        runbutton->setEnabled(true);
        debugbutton->setEnabled(true);
        stopRun->setEnabled(false);
        runDockwidget->setEnabled(true);
        QString data(socket->read(packageSize));
        runDockwidget->append(data);
        break;
    }
    case (int)Package::ReturnType::DEBUG_INFO:
    {
        QString data(socket->read(packageSize));
        disposeDebugInfo(data);
        break;
    }
    default:
        break;
    }
}

void MainWindow::disposeDebugInfo(QString data)
{
    QRegularExpression breakpointRegex("(Breakpoint \\d+) at (.*): file (.*) line (\\d+)"); // 断点信息
    QRegularExpression tobreakpointRegex("Breakpoint \\d+, .* \\(\\) at (.*):(\\d+)");      // 运行到断点信�?
    QRegularExpression crashRegex("Program received signal .*");                            // 程序崩溃信息
    QRegularExpression varValueRegex("(.*) = (.*)");                                        // 变量值信�?
    QRegularExpression stackFrameRegex("#\\d+ .* \\(\\) at (.*):(\\d+)");                   // 栈帧信息
    QRegularExpression segFaultRegex("(Program received signal SIGSEGV.*)");                // 段错误信�?
    QRegularExpression leakRegex("(LEAK SUMMARY:).*");                                      // 内存泄露信息
    QRegularExpression unhandledExceptionRegex("(terminate called after throwing.*)");      // 未处理的异常信息
    QRegularExpression assertRegex("(Assertion.*)");                                        // 断言失败信息
    QRegularExpression errorRegex("(.*):(\\d+):(\\d+):\\s+(error|warning):(.*)");           // 错误信息
    QRegularExpression gotoLine("\\w+ \\(\\) at (.*):(\\d+)");
    QRegularExpression lineinfo("\\d+\\s.*;");
    QRegularExpression reg("\\d+\\s.*{?}?");
    qDebug() << "data:" << data;
    QStringList list = data.split("\n", Qt::SkipEmptyParts);
    for (auto buf : list)
    {
        QRegularExpressionMatch match;
        if ((match = breakpointRegex.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size())
            {
                QString num = match.captured(1);
                QString address = match.captured(2);
                QString file = match.captured(3);
                QString path = "." + file.mid(46);
                path = path.left(path.size() - 1);
                QString lineNum = match.captured(4);

                QListWidgetItem *item = new QListWidgetItem(breakPointInfo);
                buf.replace(file, path);
                item->setText(buf);
                QVariant var;
                QPair<QString, int> pair(path, lineNum.toInt());
                var.setValue(pair);
                item->setData(Qt::UserRole, var);
                breakPointInfo->addItem(item);
                continue;
            }
        }
        if ((match = tobreakpointRegex.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size())
            {
                continueDebugButton->setDisabled(false);
                nextDebugButton->setDisabled(false);
                stepIntoDubugButton->setDisabled(false);
                stepOutDebugButton->setDisabled(false);
                int lineNum = match.captured(2).toInt();
                QString path = "." + match.captured(1).mid(46);
                auto vec = pro_fileMap.value(runningProject);
                std::shared_ptr<FileInfo> file;
                for (auto it : vec)
                {
                    if (it->file_path == path)
                    {
                        file = it;
                        break;
                    }
                }
                if (file.get())
                {
                    currentLine.first = file;
                    currentLine.second = lineNum;
                    CodeEdit *widget = fileWidgets.value(file->file_id);
                    ui->tabWidget->setCurrentWidget(widget);
                    widget->gotoline(lineNum);
                }
                else
                {
                    buildDockwidget->insertPlainText(buf);
                }
                continue;
            }
        }
        if ((match = stackFrameRegex.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size())
            {
                //"#\\d+ .* \\(\\) at (.*):(\\d+)"
                QString path = "." + match.captured(1).mid(46);
                int line = match.captured(2).toInt();
                QPair<QString, int> pair(path, line);
                QListWidgetItem *item = new QListWidgetItem(stackList);
                QVariant var;
                var.setValue(pair);
                item->setData(Qt::UserRole, var);
                item->setText(buf);
                stackList->addItem(item);
                continue;
            }
        }
        if ((match = gotoLine.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size())
            {
                continueDebugButton->setDisabled(false);
                nextDebugButton->setDisabled(false);
                stepIntoDubugButton->setDisabled(false);
                stepOutDebugButton->setDisabled(false);
                int lineNum = match.captured(2).toInt();
                QString path = "." + match.captured(1).mid(46);
                auto vec = pro_fileMap.value(runningProject);
                std::shared_ptr<FileInfo> file;
                for (auto it : vec)
                {
                    if (it->file_path == path)
                    {
                        file = it;
                        break;
                    }
                }
                if (file.get())
                {
                    currentLine.first = file;
                    currentLine.second = lineNum;
                    if (file->is_open)
                    {
                        CodeEdit *widget = fileWidgets.value(file->file_id);
                        ui->tabWidget->setCurrentWidget(widget);
                        widget->gotoline(lineNum);
                    }
                    else
                    {
                        openProjFile(file);
                        is_wait_file = true;
                    }
                }
                else
                {
                    buildDockwidget->insertPlainText(buf);
                }
                continue;
            }
        }
        if ((match = lineinfo.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size())
            {
                continueDebugButton->setDisabled(false);
                nextDebugButton->setDisabled(false);
                stepIntoDubugButton->setDisabled(false);
                stepOutDebugButton->setDisabled(false);
                continue;
            }
        }
        if ((match = reg.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size())
            {
                continueDebugButton->setDisabled(false);
                nextDebugButton->setDisabled(false);
                stepIntoDubugButton->setDisabled(false);
                stepOutDebugButton->setDisabled(false);
                continue;
            }
        }
        if ((match = varValueRegex.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size())
            {
                QString var = match.captured(1);
                QString val = match.captured(2);

                int row = varInfo->rowCount();
                varInfo->insertRow(row);
                varInfo->setItem(row, 0, new QTableWidgetItem(var));
                varInfo->setItem(row, 1, new QTableWidgetItem(val));
                continue;
            }
        }
        if ((match = crashRegex.match(buf)).hasMatch() || (match = segFaultRegex.match(buf)).hasMatch() || (match = unhandledExceptionRegex.match(buf)).hasMatch() || (match = assertRegex.match(buf)).hasMatch() || (match = errorRegex.match(buf)).hasMatch())
        {
            disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            buildDockwidget->append(buf);
            connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            QMessageBox::warning(this, "错误", buf);
            continue;
        }
        if ((match = leakRegex.match(buf)).hasMatch())
        {
            disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            buildDockwidget->append(buf);
            connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            continue;
        }
        if (buf == "(gdb) " || buf == "Continuing." || buf == "Quit anyway? (y or n) [answered Y; input not from terminal]")
            continue;
        else
        {
            disconnect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            runDockwidget->append(buf);
            connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
        }
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    CodeEdit *wind = (CodeEdit *)ui->tabWidget->widget(index);
    if (wind->getFile().get())
    {
        fileWidgets.remove(wind->getFile()->file_id);
        wind->getFile()->is_open = false;
        wind->deleteLater();
    }
    else
    {
        ui->tabWidget->widget(index)->deleteLater();
    }
    ui->tabWidget->removeTab(index);
}

void MainWindow::selectencodingMode()
{
    encodingType->show();
    connect(encodingType->getButtonConfirm(), &QPushButton::clicked, this, [=]()
            {
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
            EncodingTypeLabel->setText("当前的编码方式为：UTF-8(默认为UTF-8编码，如需更改，请打开“设�?-编码方式”中进行选择�?");
        encodingType->close(); });

    connect(encodingType->getButtonCancel(), &QPushButton::clicked, this, &QDialog::close);
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    MyTreeItem *treeItem = (MyTreeItem *)item;
    if (treeItem->getType() != MyTreeItem::FILE)
    {
        return;
    }

    openProjFile();
}

// 打开本地项目文件
void MainWindow::openLocalProj()
{
    debugInfo->insert(-1, new QMultiHash<QString, int>());
    // 把之前项目中的树节点删除
    int header_count = tree_widget_item_header_file_name->childCount();
    for (int i = 0; i < header_count; i++)
    {
        delete tree_widget_item_header_file_name->child(i);
    }

    int source_count = tree_widget_item_source_file_name->childCount();
    for (int i = 0; i < source_count; i++)
    {
        delete tree_widget_item_source_file_name->child(i);
    }

    // 获取文件夹的目录
    QString folder_path = QFileDialog::getExistingDirectory(this, tr("选择目录"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    current_project_path = folder_path;
    QStringList dir_list;
    bool res = get_SubDir_Under_Dir(folder_path, dir_list);
    if (res == true && dir_list.size() == 2)
    {
        if (dir_list[0] != "头文�?" || dir_list[1] != "源文�?")
            return;
    }
    else
    {
        return;
    }

    // 获取项目的名字，并设置顶层节点的内容
    int last_index = folder_path.lastIndexOf('/');
    QString project_name = folder_path.mid(last_index + 1);
    tree_widget_item_project_name->setText(0, project_name);

    // 为新的项目添加文件树
    ui->treeWidget->addTopLevelItem(tree_widget_item_project_name);
    tree_widget_item_project_name->addChild(tree_widget_item_file_information);
    tree_widget_item_project_name->addChild(tree_widget_item_header_file_name);
    tree_widget_item_project_name->addChild(tree_widget_item_source_file_name);

    // 导入项目中的所有头文件
    QString header_path = current_project_path + "/头文�?";
    QStringList header_list;
    get_SubFile_Under_SubDir(header_path, header_list, 0);
    for (int i = 0; i < header_list.size(); i++)
    {
        // 为头文件树节点新建新节点
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, header_list[i]);
        tree_widget_item_header_file_name->addChild(item);
    }

    // 导入项目中的所有源文件
    QString source_path = current_project_path + "/源文�?";
    QStringList source_list;
    get_SubFile_Under_SubDir(source_path, source_list, 1);
    for (int i = 0; i < source_list.size(); i++)
    {
        // 为源文件树节点新建新节点
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, source_list[i]);
        tree_widget_item_source_file_name->addChild(item);
    }

    /*
    //文件的信�? info实例化file_information
    QFileInfo info(folder_path);
    std::shared_ptr<FileInfo> file_information(new FileInfo);
    file_information->file_name=info.fileName();
    file_information->file_path=info.filePath();

    //file_information构造出一个code_edit文本编辑�?
    CodeEdit* code_edit=new CodeEdit(file_information,this);

    //新建一个tab加入到tabWidget�?
    ui->tabWidget->addTab(code_edit,file_information->file_name);
    file_information->is_open=true;

    //读取文件的内容并打印到code_edit编辑�?
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray array=file.readAll();
    code_edit->addText(array);

    //一个path对应一个code_edit指针，添加到映射表中
    mp[file_information->file_path]=code_edit;
    */
}

// 保存本地项目文件
void MainWindow::saveLocalProj()
{
    QFileDialog file_dialog;
    // 获取文件的路�?
    QString file_path = file_dialog.getSaveFileName(this, tr("打开文件"));

    if (file_path == "")
        return;
    QFile file(file_path);

    // 在map映射表中进行查询此路径是否对应有code_edit
    if (mp.count(file_path) == 1) // 打开的方式进行保�?
    {
        CodeEdit *code_edit = mp[file_path];
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream cout(&file);
            QString str = code_edit->getText();
            cout << str;
            QMessageBox::information(this, "提示", "保存成功");
            file.close();
        }
        else
        {
            QMessageBox::critical(this, "错误", "保存失败");
            return;
        }
    }
    else // 新建的方式进行保�?
    {
    }
}

// 新建本地项目
void MainWindow::newLocalProj()
{
    NewLocalProject *dialog = new NewLocalProject(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    // 新建按钮
    connect(dialog->get_pushButton_new(), &QPushButton::clicked, this, [=]()
            {
        if(dialog->get_lineEdit_name()->text()=="")
        {
            QMessageBox::critical(this,"错误","请输入项目名�?");
            return;
        }
        else if(dialog->get_lineEdit_location()->text()=="")
        {
            QMessageBox::critical(this,"错误","请选择新建项目的路�?");
            return;
        }
        else
        {
            dialog->project_name=dialog->get_lineEdit_name()->text();
            //记录当前项目的路径到mainwindow中，便于后续的添�?
            current_project_path=dialog->project_path+"/"+dialog->project_name;
            QDir dir;
            if(dir.mkdir(current_project_path))
            {
                QMessageBox::information(this,"信息","新建项目成功");
                QString header_file=current_project_path+"/头文�?";
                QString cpp_file=current_project_path+"/源文�?";
                QString information_file=current_project_path+"/项目信息.txt";
                QFile file(information_file);
                file.open(QIODevice::ReadWrite);
                file.close();
                dir.mkdir(header_file);
                dir.mkdir(cpp_file);

                //为tree_widget添加顶层节点
                /*
                QTreeWidgetItem* topItem=new QTreeWidgetItem(ui->treeWidget);
                topItem->setText(0,dialog->get_lineEdit_name()->text());
                ui->treeWidget->addTopLevelItem(topItem);*/

                tree_widget_item_project_name->setText(0,dialog->get_lineEdit_name()->text());
                ui->treeWidget->addTopLevelItem(tree_widget_item_project_name);

                //为tree_widget添加第二层节�?
                /*
                QTreeWidgetItem* secondItem1=new QTreeWidgetItem(topItem);
                QTreeWidgetItem* secondItem2=new QTreeWidgetItem(topItem);
                QTreeWidgetItem* secondItem3=new QTreeWidgetItem(topItem);
                secondItem1->setText(0,"项目信息.txt");
                secondItem2->setText(0,"头文�?");
                secondItem3->setText(0,"源文�?");
                topItem->addChild(secondItem1);
                topItem->addChild(secondItem2);
                topItem->addChild(secondItem3);*/

                tree_widget_item_project_name->addChild(tree_widget_item_file_information);
                tree_widget_item_project_name->addChild(tree_widget_item_header_file_name);
                tree_widget_item_project_name->addChild(tree_widget_item_source_file_name);
            }
            else
            {
                QMessageBox::critical(this,"错误","新建项目失败");
            }
            dialog->close();
        } });
}

// 添加本地文件
void MainWindow::addLocalFile()
{
    if (current_project_path == "")
    {
        QMessageBox::critical(this, "错误", "当前文件为空，请先新建文件或打开文件在执行添加文件操�?");
        return;
    }
    AddLocalFile *dialog = new AddLocalFile(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->set_lineEdit_path(current_project_path);
    dialog->show();
    // 确定路径以及文件名称
    connect(dialog->get_pushButton_add(), &QPushButton::clicked, this, [=]()
            {
        if(dialog->get_lineEdit_name()->text()=="")
        {
            QMessageBox::warning(this,"警告","请输入文件名�?");
            return;
        }
        else if(!dialog->isLegal(dialog->get_lineEdit_name()->text()))
        {
            QMessageBox::warning(this,"警告","请输入合法的文件名（只允许包含字母和数字�?");
            return;
        }
        else
        {
            //路径名字
            if(dialog->get_comboBox_current_index()==0)//.cpp
            {
               QString file_path=current_project_path+"/源文�?/"+dialog->get_lineEdit_name()->text()+".cpp";
               this->addFile(file_path);

               QTreeWidgetItem* item=new QTreeWidgetItem();
               item->setText(0,dialog->get_lineEdit_name()->text()+".cpp");
               tree_widget_item_source_file_name->addChild(item);
            }
            else//.h
            {
                QString  file_path1=current_project_path+"/头文�?/"+dialog->get_lineEdit_name()->text()+".h";
                QString  file_path2=current_project_path+"/源文�?/"+dialog->get_lineEdit_name()->text()+".cpp";
                this->addFile(file_path1);
                this->addFile(file_path2);

                QTreeWidgetItem* item1=new QTreeWidgetItem();
                QTreeWidgetItem* item2=new QTreeWidgetItem();
                item1->setText(0,dialog->get_lineEdit_name()->text()+".h");
                item2->setText(0,dialog->get_lineEdit_name()->text()+".cpp");
                tree_widget_item_header_file_name->addChild(item1);
                tree_widget_item_source_file_name->addChild(item2);
            }
            dialog->close();
        } });
}

// 本函数的作用是在指定的路径下新建一个新的文件（可以使任何类型的文件，例�?.cpp .h .txt)
// 例如 D:D:/4.23/123.txt 将在D�?4.23文件目录下新建一个名�?123.txt的文本文�?
void MainWindow::addFile(QString file_path)
{
    // 新建文件
    QFile *new_file = new QFile(this);
    new_file->setFileName(file_path);
    bool res = new_file->open(QIODevice::ReadWrite | QIODevice::Text);
    new_file->close();
    // 判断是否新建成功
    if (!res)
    {
        QMessageBox::critical(this, "错误", "文件新建失败");
        return;
    }
    else
    {
        QMessageBox::information(this, "新建文件", "新建文件成功");
        openFileAndAddTab(file_path);
    }
}

// 本函数的作用是在给定的路径下将文件打开并构造一个文本编辑器和添加到tabWidget�?
void MainWindow::openFileAndAddTab(QString file_path)
{
    QFileInfo info(file_path);
    std::shared_ptr<FileInfo> file_information(new FileInfo);
    file_information->file_name = info.fileName();
    file_information->file_path = info.filePath();

    file_information->file_project = -1;

    // file_information构造出一个code_edit文本编辑�?
    CodeEdit *code_edit = new CodeEdit(file_information, this);

    // 新建一个tab加入到tabWidget�?
    ui->tabWidget->addTab(code_edit, file_information->file_name);
    file_information->is_open = true;

    // 读取文件的内容并打印到code_edit编辑�?
    QFile file(file_path);
    file.open(QIODevice::ReadOnly);
    QByteArray array = file.readAll();
    code_edit->addText(array);

    // 一个path对应一个code_edit指针，添加到映射表中
    mp[file_information->file_path] = code_edit;
}

// 该函数的作用是在给定的路径下获取当中的所有文件夹，并添加到参数QStringList�?
bool MainWindow::get_SubDir_Under_Dir(QString path, QStringList &list)
{
    QDir *dir = new QDir(path);
    // 不存在此目录
    if (!dir->exists())
    {
        delete dir;
        dir = nullptr;
        return false;
    }
    else
    {
        list = dir->entryList(QDir::Dirs); // 指明仅接受文件夹
        list.removeOne(".");
        list.removeOne("..");
        delete dir;
        dir = nullptr;
        return true;
    }
}

// 该函数的作用是在给定的文件夹下获取当中的所有文件，并添加到参数QStringList�?(参数tag1==0指明要获取的�?"*.h"文件，tag==1指明要获取的�?"*.cpp"文件)
bool MainWindow::get_SubFile_Under_SubDir(QString path, QStringList &list, int tag)
{
    QDir *dir = new QDir(path);
    // 不存在此目录
    if (!dir->exists())
    {
        delete dir;
        dir = nullptr;
        return false;
    }
    else
    {
        if (tag == 0)
            dir->setNameFilters(QStringList("*.h"));
        else if (tag == 1)
            dir->setNameFilters((QStringList("*.cpp")));
        else
        {
            delete dir;
            dir = nullptr;
            return false;
        }
        list = dir->entryList(QDir::Files);
        list.removeOne(".");
        list.removeOne("..");
        delete dir;
        dir = nullptr;
        return true;
    }
}

// run project
void MainWindow::runProject()
{
    statusIcon->movie()->stop();
    buildingMovie->start();
    statusIcon->setMovie(buildingMovie);
    workState = ProjectWorkState::RUNNING;
    runbutton->setEnabled(false);
    debugbutton->setEnabled(false);

    disconnect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
    runDockwidget->clear();
    connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));

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

    if (currentProject < 0)
    {
        runningProject = currentProject;
    }
    else
    {
        runningProject = currentProject;
        Package pck(QString::number(currentProject).toUtf8(), (int)Package::PackageType::RUN_PROJECT);
        socket->write(pck.getPdata(), pck.getSize());
    }
}

QString MainWindow::runCompilerAndGetOutput(QString pro_Path)
{
    QProcess *process = new QProcess(this);
    process->setProgram("cmake");
    process->setNativeArguments(pro_Path + " -B " + pro_Path + "\\build -G \"Unix Makefiles\"");
    process->start();
    process->waitForStarted();
    process->waitForFinished();

    process->setProgram("mingw32-make");
    process->setNativeArguments(" -C " + pro_Path + "\\build");
    process->start();
    process->waitForStarted();
    process->waitForFinished();
    auto data = process->readAllStandardOutput();
    auto error = process->readAllStandardError();
    QString data_text = QString::fromLocal8Bit(data);
    QString error_text = QString::fromLocal8Bit(error);
    buildDockwidget->insertPlainText(data_text + error_text);
    return error_text;
}

void MainWindow::findFileName(const QString &path)
{

    QDir dir(path);
    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &fileInfo : files)
    {
        if (fileInfo.isFile())
        {
            fileName->append("\"" + fileInfo.fileName() + "\"");
        }
        else
        {
            findFileName(path + "\\" + fileInfo.fileName());
        }
    }
}

void MainWindow::setSystemVar(const QString &bin_Path)
{
    this->systemVar = bin_Path;
}
