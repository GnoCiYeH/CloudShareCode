#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "projectform.h"
#include "logindialog.h"
#include <QAction>
#include <QFileDialog>
#include <QToolBar>
#include <QToolButton>
#include "package.h"
#include "newprojectdialog.h"
#include "newfiledialog.h"
#include <QPoint>
#include "privilegemanager.h"
#include "newlocalproject.h"
#include <QScrollBar>
#include<QTemporaryFile>>

QTcpSocket *MainWindow::socket = new QTcpSocket();
QHash<int, Project> *MainWindow::userProjs = new QHash<int, Project>();
QString MainWindow::userId = "";
QHash<int, QMultiHash<QString, int> *> *MainWindow::debugInfo = new QHash<int, QMultiHash<QString, int> *>();
bool MainWindow::isLogin = false;

QStringList *MainWindow::fileName = new QStringList();
int MainWindow::local_project_id = -1; // é—???â???????‰é???????‘ç€???????
int MainWindow::local_file_id = -1;    // é—???â???????‰é???????‘ç€???????
LoginDialog *MainWindow::loginDialog;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    current_project_name = "";
    current_project_path = "";

    // timer_for_save->start(5000);
    // connect(timer_for_save,&QTimer::timeout,this,&MainWindow::saveLocalProj);

    tree_widget_item_file_information->setText(0, "CMakeLists.txt");
    tree_widget_item_source_file_name->setText(0, "Source");
    tree_widget_item_header_file_name->setText(0, "Header");

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
            //qDebug()<<"????¤?šé”›ä?Šç´’é”›ä?Šç??";
        }
        isAlive = false; });

    // ç’??‰§ç–?????¤ç?é???…ç†€éˆî„€ç˜é???
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
    ui->actionCloud_project->setText("Open cloud project");
    ui->actionOpenLocal_project->setText("Open local project");
    ui->actionNew_cloud_project->setText("New cloud project");
    ui->actionNew_local_project->setText("New local project");

    connect(runbutton, SIGNAL(clicked()), this, SLOT(runProject()));
    connect(stopRun, SIGNAL(clicked()), this, SLOT(stopProject()));
    connect(debugbutton, SIGNAL(clicked()), this, SLOT(debugProject()));

    // é?????•­é‘?????´Ÿ
    submitProject = new QAction("Submit project", ui->treeWidget);
    closeProject = new QAction("Close project", ui->treeWidget);
    newFile = new QAction("New File", ui->treeWidget);
    deleteFile = new QAction("Delete file", ui->treeWidget);
    openFile = new QAction("Open file", ui->treeWidget);
    attribute = new QAction("Attribute", ui->treeWidget);
    rename = new QAction("Rename", ui->treeWidget);

    QSize size(25, 25);
    statusIcon->setFixedSize(size);
    stateokMovie->setScaledSize(size);
    buildingMovie->setScaledSize(size);
    runningMovie->setScaledSize(size);
    debugingMovie->setScaledSize(size);

    // é????â‚????çˆ?(é??‰§?šé????â‚?????‹°é?? •???é”??
    setStatusBar(status_bar);
    status_bar->addWidget(statusIcon);
    statusIcon->setMovie(stateokMovie);
    stateokMovie->start();
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

    // é??„§?§¸é????çˆ?é??‰§?š??????çˆœé‚ç‘°ç??
    EncodingTypeLabel->setText("        ?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”??­TF-8 (??????î…???“ç??TF-8??????çˆœé”›???î›§é—‡â‚?é??­˜???é”???ƒî??é????ç´‘é????†î†•????-??????çˆœé‚ç‘°ç??éˆ?????…‘?????œî”‘é–??¤‹???");
    status_bar->addWidget(EncodingTypeLabel);
    EncodingTypeLabel->setAlignment(Qt::AlignCenter);

    // é?????•­é‘?????´Ÿ
    connect(openFile, SIGNAL(triggered(bool)), this, SLOT(openProjFile()));
    connect(newFile, SIGNAL(triggered(bool)), this, SLOT(newProFile()));
    connect(deleteFile, SIGNAL(triggered(bool)), this, SLOT(deleteProFile()));

    // ????˜??é—?›Ÿçˆ?
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionCloud_project, SIGNAL(triggered()), this, SLOT(openCloudProj()));
    connect(ui->actionNew_cloud_project, SIGNAL(triggered()), this, SLOT(newCloudProj()));
    connect(ui->actionSetting, SIGNAL(triggered()), this, SLOT(showSetting()));

    //*************************************************************************************************
    connect(ui->actionNew_local_project, SIGNAL(triggered()), this, SLOT(newLocalProj()));   // é??????“éˆî„€????¤¤?œ­?´°
    connect(ui->actionAdd_Local_File, &QAction::triggered, this, &MainWindow::addLocalFile); // ???????§éˆî„€??´é‚??????
    connect(ui->actionOpenLocal_project, SIGNAL(triggered()), this, SLOT(openLocalProj()));  // é????ç´‘éˆî„€????¤¤?œ­?´°
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveLocalProj);          // ?????†ç“¨éˆî„€????¤¤?œ­?´°
    //*************************************************************************************************

    connect(ui->Setting, SIGNAL(triggered()), this, SLOT(openSettingDialog()));

    // socket
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataProgress()));

    // ç€????ç?
    projectForm = new ProjectForm(this);
    projectForm->setWindowFlag(Qt::Window);
    loginDialog = new LoginDialog(this);

    buildDock = new QDockWidget(this);
    buildDockwidget = new QTextEdit(buildDock);
    buildDockwidget->setFocusPolicy(Qt::NoFocus);
    buildDock->setWidget(buildDockwidget);
    buildDock->setWindowTitle("Build");
    this->addDockWidget(Qt::BottomDockWidgetArea, buildDock);

    runDock = new QDockWidget(this);
    runDock->setWindowTitle("Console");
    runDockwidget = new QTextEdit(runDock);
    runDockwidget->setFocusPolicy(Qt::NoFocus);
    runDock->setWidget(runDockwidget);
    this->addDockWidget(Qt::BottomDockWidgetArea, runDock);
    connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));

    stackDock = new QDockWidget(this);
    stackList = new QListWidget(stackDock);
    stackDock->setHidden(true);
    stackDock->setWidget(stackList);
    connect(stackList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(gotoStackFrame(QListWidgetItem *)));

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

    varInfo->setColumnCount(2);
    varInfo->setHorizontalHeaderLabels({"Variable", "Value"});

    debugToolBar = new QToolBar(this);
    this->addToolBar(Qt::ToolBarArea::RightToolBarArea, debugToolBar);
    debugToolBar->setHidden(true);

    continueDebugButton = new QToolButton(debugToolBar);
    continueDebugButton->setIcon(QIcon("://icon/continue.png"));
    continueDebugButton->setDisabled(true);
    connect(continueDebugButton, &QToolButton::clicked, this, [=]()
            {
        varInfo->clearContents();
        varInfo->setRowCount(0);
        if(runningProject<0)
        {
            QString data = "";
            data+="c\ninfo local\n";
            debugThread->writeGdbOrStdin(data);
        }
        else
        {
            QString data = Package::intToByteArr(currentProject);
            data+="c\ninfo local\n";
            Package pck(data.toUtf8(),(int)Package::PackageType::POST_STDIN);
            socket->write(pck.getPdata(),pck.getSize());
        }

        continueDebugButton->setDisabled(true);
        nextDebugButton->setDisabled(true);
        stepIntoDubugButton->setDisabled(true);
        stepOutDebugButton->setDisabled(true); });

    stopDebugButton = new QToolButton(debugToolBar);
    stopDebugButton->setIcon(QIcon("://icon/stop.png"));
    stopDebugButton->setDisabled(true);
    connect(stopDebugButton, &QToolButton::clicked, this, [=]()
            {
        if(runningProject<0)
        {
            QString data = "";
            data+="q\ny\n";
            debugThread->writeGdbOrStdin(data);
        }
        else
        {
            QString data = Package::intToByteArr(currentProject);
            data+="q\ny\n";
            Package pck(data.toUtf8(),(int)Package::PackageType::POST_STDIN);
            socket->write(pck.getPdata(),pck.getSize());
        }

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

        varInfo->clearContents();
        varInfo->setRowCount(0);
        if(runningProject<0)
        {
            QString data = "";
            data+="n\ninfo local\n";
            debugThread->writeGdbOrStdin(data);
        }
        else
        {
            QString data = Package::intToByteArr(currentProject);
            data+="n\ninfo local\n";
            Package pck(data.toUtf8(),(int)Package::PackageType::POST_STDIN);
            socket->write(pck.getPdata(),pck.getSize());
        }

        continueDebugButton->setDisabled(true);
        nextDebugButton->setDisabled(true);
        stepIntoDubugButton->setDisabled(true);
        stepOutDebugButton->setDisabled(true); });

    stepIntoDubugButton = new QToolButton(debugToolBar);
    stepIntoDubugButton->setIcon(QIcon("://icon/stepinto.png"));
    stepIntoDubugButton->setDisabled(true);
    connect(stepIntoDubugButton, &QToolButton::clicked, this, [=]()
            {
                varInfo->clearContents();
                varInfo->setRowCount(0);
                if(runningProject<0)
                {
                    QString data = "";
                    data+="step\ninfo local\nbacktrace\n";
                    debugThread->writeGdbOrStdin(data);
                }
                else
                {
                    QString data = Package::intToByteArr(currentProject);
                    data+="step\ninfo local\nbacktrace\n";
                    Package pck(data.toUtf8(),(int)Package::PackageType::POST_STDIN);
                    socket->write(pck.getPdata(),pck.getSize());
                }

                continueDebugButton->setDisabled(true);
                nextDebugButton->setDisabled(true);
                stepIntoDubugButton->setDisabled(true);
                stepOutDebugButton->setDisabled(true); });

    stepOutDebugButton = new QToolButton(debugToolBar);
    stepOutDebugButton->setIcon(QIcon("://icon/stepout.png"));
    stepOutDebugButton->setDisabled(true);
    connect(stepOutDebugButton, &QToolButton::clicked, this, [=]()
            {
        varInfo->clearContents();
                varInfo->setRowCount(0);
        if(runningProject<0)
        {
            QString data = "";
            data+="finish\ninfo local\nbacktrace\n";
            debugThread->writeGdbOrStdin(data);
        }
        else
        {
            QString data = Package::intToByteArr(currentProject);
            data+="finish\ninfo local\nbacktrace\n";
            Package pck(data.toUtf8(),(int)Package::PackageType::POST_STDIN);
            socket->write(pck.getPdata(),pck.getSize());
        }

        continueDebugButton->setDisabled(true);
        nextDebugButton->setDisabled(true);
        stepIntoDubugButton->setDisabled(true);
        stepOutDebugButton->setDisabled(true); });

    debugToolBar->addWidget(continueDebugButton);
    debugToolBar->addWidget(stopDebugButton);
    debugToolBar->addWidget(nextDebugButton);
    debugToolBar->addWidget(stepIntoDubugButton);
    debugToolBar->addWidget(stepOutDebugButton);

    // ç€????ç?é??????
    connect(this, &MainWindow::loginAllowed, loginDialog, &LoginDialog::loginSucceed);
    connect(this, SIGNAL(projInited()), projectForm, SLOT(init()));
    connect(projectForm, SIGNAL(openProj(int)), this, SLOT(openProj(int)));
    connect(ui->treeWidget, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(projectItemPressedSlot(QTreeWidgetItem *, int)));

    // é–??¤‹??¨ç?????çˆœé‚ç‘°ç??
    connect(ui->actionSwitching, &QAction::triggered, this, &MainWindow::selectencodingMode);

    // é???????
    connect(ui->actionClose, &QAction::triggered, this, &QMainWindow::close);

    // é??????“é‚??????
    connect(ui->new_file_action, &QAction::triggered, this, [=]()
            { QFileDialog::getOpenFileName(this, "é??????“é‚??????", "C:/Users"); });

    // ???????§é???????
    connect(ui->actionAdd_Cloud_File, &QAction::triggered, this, [=]()
            { QFileDialog::getOpenFileName(this, "å¨£è¯²å§é‚å›¦æ¬¢", "C:/Users"); });
    // connect(ui->add_file_action,&QAction::triggered,this,[=](){QFileDialog::getOpenFileName(this,"å¨£è¯²å§é‚å›¦æ¬¢","C:/Users");});

    connect(ui->actionConsole, &QAction::triggered, this, [=]()
            { runDock->setHidden(false); });

    connect(ui->actionBuild_Dock, &QAction::triggered, this, [=]()
            { buildDock->setHidden(false); });

    process = new QProcess(this);
    connect(process, &QProcess::readyRead, this, [&]() mutable
            {

        this->data+=QString::fromLocal8Bit(process->readAll());
        qDebug()<<this->data; });

    QSettings settings("./configs/configs.ini", QSettings::IniFormat, this);
    settings.beginGroup("CXX_INCLUDE");
    int includeNum = settings.beginReadArray("INCLUDES");
    for (int i = 0; i < includeNum; i++)
    {
        settings.setArrayIndex(i);
        QString includePath = settings.value("include").toString();
    }
    settings.endArray();
    settings.endGroup();

    //    setSystemVar();
    //    findFileName(systemVar);
}

MainWindow::~MainWindow()
{
    if (workState == ProjectWorkState::DEBUGING || workState == ProjectWorkState::RUNNING)
    {
        Package pck(QString::number(runningProject).toUtf8(), (int)Package::PackageType::KILL_PROJECT_FORCE);
        socket->write(pck.getPdata(), pck.getSize());
    }

    runThread->deleteLater();
    debugThread->deleteLater();

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
}

void MainWindow::gotoStackFrame(QListWidgetItem *item)
{
    QPair<QString, int> pair = item->data(Qt::UserRole).value<QPair<QString, int>>();
    QString path = pair.first;
    int lineNum = pair.second;
    auto vec = pro_fileMap.value(runningProject);
    std::shared_ptr<FileInfo> file;
    for (auto it : *vec)
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
    if (runningProject < 0)
    {
        if (workState == ProjectWorkState::DEBUGING)
        {
            debugThread->writeGdbOrStdin(data);
        }
        else
        {
            runThread->writeStdin(data);
        }
    }
    else
    {
        Package pck(data.toUtf8(), (int)Package::PackageType::POST_STDIN);
        MainWindow::socket->write(pck.getPdata(), pck.getSize());
        MainWindow::socket->flush();
    }
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
    debugToolBar->setHidden(false);
    workState = ProjectWorkState::DEBUGING;

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

    breakPointInfo->clear();
    varInfo->clearContents();
    varInfo->setRowCount(0);
    stackList->clear();

    if (runningProject < 0)
    {
        if (debugThread)
        {
            disconnect(debugThread);
            debugThread->deleteLater();
        }

        QDir buildDir(current_project_path + "/build");
        buildDir.removeRecursively();

        QString path = current_project_path;
        debugThread = new DebugThread(path, this);
        connect(debugThread, SIGNAL(buildInfo(QString)), this, SLOT(appendBuildText(QString)));
        connect(debugThread, SIGNAL(debugInfo(QString)), this, SLOT(disposeDebugInfo(QString)));
        connect(debugThread, &DebugThread::buildFinish, this, [=]()
                {
            statusIcon->movie()->stop();
            debugingMovie->start();
            statusIcon->setMovie(debugingMovie);

            auto breakPoints = debugInfo->value(runningProject);
            QString data = "file "+current_project_path+"/build/bin/"+current_project_name+".exe\n";
            for (auto it = breakPoints->constBegin(); it != breakPoints->constEnd(); it++)
            {
                data += "b " + it.key() + ":" + QString::number(it.value()) + "\n";
            }
            data += "r\ninfo local\nbacktrace\n";

            debugThread->writeGdbOrStdin(data); });
        connect(debugThread, &DebugThread::debugFinish, this, [=](int n)
                {
                    statusIcon->movie()->stop();
                    stateokMovie->start();
                    statusIcon->setMovie(stateokMovie);
                    ui->tabWidget->setTabsClosable(true);
                    workState = ProjectWorkState::NONE;
                    runbutton->setEnabled(true);
                    debugbutton->setEnabled(true);
                    stopRun->setEnabled(false);
                    runDockwidget->setEnabled(true);

            debugToolBar->setHidden(true);
            breakPointDock->setHidden(true);
            varDock->setHidden(true);
            stackDock->setHidden(true);

            appendRunningText("Debug exit with code "+QString::number(n)); });
        debugThread->run();
    }
    else
    {
        if (workState == ProjectWorkState::DEBUGING || workState == ProjectWorkState::RUNNING)
        {
            QString data = QString::number(userProjs->value(runningProject).pro_id);
            Package pck(data.toUtf8(), (int)Package::PackageType::KILL_PROJECT_FORCE);
            socket->write(pck.getPdata(), pck.getSize());
        }

        QString data = QString::number(userProjs->value(currentProject).pro_id);
        Package pck(data.toUtf8(), (int)Package::PackageType::DEBUG_PROJECT);
        socket->write(pck.getPdata(), pck.getSize());
    }
}

void MainWindow::newCloudProj()
{
    // é‘??ƒ§?•¤é´é”‹??­é§?????é’???????¨‰?›š?¨‡é??????????????§›é‘??–¤ç´????ç‘??š­é??????é????????
    if (!isLogin)
    {
        Login();
        if (isLogin)
        {
            // ????????‡é”â€???’é???¤Š????¤¤?œ­?´°??‡â„ƒ???
            Package pck("", (int)Package::PackageType::INIT_PROJS);
            socket->write(pck.getPdata(), pck.getSize());
        }
    }

    // é??????é´????§›é????????????œî”‘?????????é???„ç??
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

        QMessageBox::StandardButton result = QMessageBox::warning(this, "??­î??ç•?é’ç??????", "é??„§â€˜ç€????î›?é’ç????é‚???????" + file->file_name + "?");
        if (result != QMessageBox::StandardButton::Ok)
            return;

        QString data = QString::number(file->file_id) + "\t" + file->file_path + "\t" + QString::number(file->file_project);
        Package pck(data.toUtf8(), (int)Package::PackageType::DEL_FILE);

        socket->write(pck.getPdata(), pck.getSize());
    }
    else
    {
        //        auto dir = var.value<std::shared_ptr<Directory>>();

        //        QMessageBox::StandardButton result = QMessageBox::warning(this,"??­î??ç•?é’ç??????","é??„§â€˜ç€????î›?é’ç????é‚???????"+dir->dir_name+"?");
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
        // é??ˆ??‡é”â€???’é™???â‚????î‡?????
        NewFileDialog wind(dir, this);
        wind.exec();
    }
}

// ???????§tab
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

// é???????file???????•é???
void MainWindow::openProjFile()
{
    MyTreeItem *item = (MyTreeItem *)ui->treeWidget->currentItem();
    QVariant var = item->data(0, Qt::UserRole);
    std::shared_ptr<FileInfo> file = var.value<std::shared_ptr<FileInfo>>();
    if (file->file_project < 0)
    {
        // é????ç´‘éˆî„€??´é‚??????
        if (!file->is_open)
        {
            if (addFileWidget(file))
            {

                CodeEdit *widget = fileWidgets.value(file->file_id);
                // ç’??????‡é‚?????????????Šé”ç???ŸŒcode_edit????
                QFile read_file(file->file_path);
                read_file.open(QIODevice::ReadWrite);
                QByteArray array = read_file.readAll();
                qDebug() << array;
                widget->addText(array);
            }
        }
        else
        {
            QWidget *widget = fileWidgets.value(file->file_id);
            ui->tabWidget->setCurrentWidget(widget);
        }
    }
    else
    {
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
}

void MainWindow::openProjFile(std::shared_ptr<FileInfo> file)
{
    if (file->file_project < 0)
    {
        // é????ç´‘éˆî„€??´é‚??????
        if (!file->is_open)
        {
            if (addFileWidget(file))
            {

                CodeEdit *widget = fileWidgets.value(file->file_id);
                // ç’??????‡é‚?????????????Šé”ç???ŸŒcode_edit????
                QFile read_file(file->file_path);
                read_file.open(QIODevice::ReadWrite);
                QByteArray array = read_file.readAll();
                qDebug() << array;
                widget->addText(array);
                ui->tabWidget->setCurrentWidget(widget);
                if (is_wait_file)
                {
                    is_wait_file = false;
                    widget->gotoline(currentLine.second);
                }
            }
        }
        else
        {
            QWidget *widget = fileWidgets.value(file->file_id);
            ui->tabWidget->setCurrentWidget(widget);
        }
    }
    else
    {
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
    // é‘??ƒ§?•¤é´é”‹??­é§?????é’???????¨‰?›š?¨‡é??????????????§›é‘??–¤ç´????ç‘??š­é??????é????????
    if (!isLogin)
    {
        Login();
        if (isLogin)
        {
            // ????????‡é”â€???’é???¤Š????¤¤?œ­?´°??‡â„ƒ???
            Package pck("", (int)Package::PackageType::INIT_PROJS);
            socket->write(pck.getPdata(), pck.getSize());
        }
    }

    // é??????é´????§›é????????????œî”‘?????????é???„ç??
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
        box.setWindowTitle("é??ˆ ?•“é–???’î‡¤");
        box.setText("é–???’î‡¤é???Šç´°" + QString(arr));
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

        QVector<std::shared_ptr<FileInfo>> *fileVec = new QVector<std::shared_ptr<FileInfo>>;
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

            fileVec->append(file);
        }

        pro_fileMap.insert(pro_id, fileVec);

        for (int i = 0; i < fileVec->size(); i++)
        {
            // Path????????????? ./UserId/ProName/Dir(File)...
            std::shared_ptr<FileInfo> file = (*fileVec)[i];
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
        // é???????é???????é”?????‹Ÿé??‰§?šé?â€“odeEdit????
        QByteArray temp = socket->read(4);
        int fid = Package::ByteArrToInt(temp, 0);

        QByteArray data = socket->read(packageSize - 4);

        auto fileEditor = (CodeEdit *)fileWidgets.value(fid);

        fileEditor->addText(QString(data));

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

        pro_fileMap.value(file->file_project)->append(file);

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
            for (int i = 0; i < vec->size(); i++)
            {
                auto file = (*vec)[i];
                if (file->file_id == fid)
                {
                    file->file_item->parent()->removeChild(file->file_item);
                    delete file->file_item;
                    vec->removeAt(i);
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
        if (workState == ProjectWorkState::DEBUGING)
        {
            statusIcon->movie()->stop();
            debugingMovie->start();
            statusIcon->setMovie(debugingMovie);
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
    QRegularExpression breakpointRegex("(Breakpoint \\d+) at (.*): file (.*) line (\\d+)"); // è­ï½­è½¤ï½¹è«ï½¡è«±ï½¯
    QRegularExpression tobreakpointRegex("Breakpoint \\d+, .* \\(\\) at (.*):(\\d+)");      // éœ‘å ï½¡æ‚ŸèŠ¦è­ï½­è½¤ï½¹è«ï½¡è«±?
    QRegularExpression tobreakpointRegex2("Thread\\s\\d+\\shit\\sBreakpoint\\s\\d+,\\s\\w+\\s\\(\\)\\sat\\s(.*):(\\d+)");
    QRegularExpression crashRegex("Program received signal .*");                       // éå¥ï½ºä¸ï½´ï½©è²…ï¿½ï½¿ï½¡è«±ï½¯
    QRegularExpression varValueRegex("(.*) = (.*)");                                   // èœ¿å€¬ã¼è›Ÿï½¼è«ï½¡è«??
    QRegularExpression stackFrameRegex("#\\d+ .* \\(\\) at (.*):(\\d+)");              // è­¬äº¥ï½¸ï½§è«ï½¡è«±ï½¯
    QRegularExpression segFaultRegex("(Program received signal SIGSEGV.*)");           // è°¿ï½µé«å‘µï½¯ï½¯è«ï½¡è«??
    QRegularExpression leakRegex("(LEAK SUMMARY:).*");                                 // èœ€ï¿½ï½­ä¿¶ï½³ï¿½æ„†è«ï½¡è«±ï½¯
    QRegularExpression unhandledExceptionRegex("(terminate called after throwing.*)"); // è­›ï½ªèŸï¿½ç‚Šé€§ï¿½ï½¼ã‚‡ï½¸ï½¸è«ï½¡è«±ï½¯
    QRegularExpression assertRegex("(Assertion.*)");                                   // è­ï½­éš?èŸï½±é›ï½¥è«ï½¡è«±ï½¯
    QRegularExpression errorRegex("(.*):(\\d+):(\\d+):\\s+(error|warning):(.*)");      // é«å‘µï½¯ï½¯è«ï½¡è«±ï½¯
    QRegularExpression gotoLine("\\w+ \\(\\) at (.*):(\\d+)");
    QRegularExpression lineinfo("\\d+\\s.*;");
    QRegularExpression reg("\\d+\\s.*{?}?");
    QRegularExpression newThreadInfo("\\[New Thread \\d+.0x.*\\]");
    qDebug() << "data:" << data;
    QStringList list = data.split("\n", Qt::SkipEmptyParts);
    for (auto buf : list)
    {
        if (buf == "(gdb) ")
        {
            continue;
        }
        if (buf.startsWith("(gdb) "))
        {
            buf = buf.mid(6);
        }
        if (buf.isEmpty())
            continue;
        if (buf == "No locals.")
        {
            disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            buildDockwidget->append(buf);
            connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            continue;
        }
        QRegularExpressionMatch match;
        if ((match = newThreadInfo.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size() || match.captured(0).size() == buf.size() - 1)
            {
                disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
                buildDockwidget->insertPlainText(buf);
                connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            }
        }
        if ((match = breakpointRegex.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size() || match.captured(0).size() == buf.size() - 1)
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
                QString path;
                if (runningProject >= 0)
                {
                    path = "." + match.captured(1).mid(46);
                }
                else
                {
                    path = match.captured(1);
                }
                auto vec = pro_fileMap.value(runningProject);
                std::shared_ptr<FileInfo> file;
                for (auto it : *vec)
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
                    disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
                    buildDockwidget->insertPlainText(buf);
                    connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
                }
                continue;
            }
        }
        if ((match = tobreakpointRegex2.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size())
            {
                continueDebugButton->setDisabled(false);
                nextDebugButton->setDisabled(false);
                stepIntoDubugButton->setDisabled(false);
                stepOutDebugButton->setDisabled(false);
                int lineNum = match.captured(2).toInt();
                QString path;
                if (runningProject >= 0)
                {
                    path = "." + match.captured(1).mid(46);
                }
                else
                {
                    path = match.captured(1);
                }
                auto vec = pro_fileMap.value(runningProject);
                std::shared_ptr<FileInfo> file;
                for (auto it : *vec)
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
                    disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
                    buildDockwidget->insertPlainText(buf);
                    connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
                }
                continue;
            }
        }
        if ((match = stackFrameRegex.match(buf)).hasMatch())
        {
            if (match.captured(0).size() == buf.size())
            {
                //"#\\d+ .* \\(\\) at (.*):(\\d+)"
                QString path;
                if (runningProject >= 0)
                {
                    path = "." + match.captured(1).mid(46);
                }
                else
                {
                    path = match.captured(1);
                }
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
                QString path;
                if (runningProject >= 0)
                {
                    path = "." + match.captured(1).mid(46);
                }
                else
                {
                    path = match.captured(1);
                }
                auto vec = pro_fileMap.value(runningProject);
                std::shared_ptr<FileInfo> file;
                for (auto it : *vec)
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
                        is_wait_file = true;
                        openProjFile(file);
                    }
                }
                else
                {
                    disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
                    buildDockwidget->insertPlainText(buf);
                    connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
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
            QMessageBox::warning(this, "error:", buf);
            continue;
        }
        if ((match = leakRegex.match(buf)).hasMatch())
        {
            disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            buildDockwidget->append(buf);
            connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            continue;
        }
        if (buf == "Continuing." || buf == "Quit anyway? (y or n) [answered Y; input not from terminal]")
            continue;
        else
        {
            disconnect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
            runDockwidget->append(buf);
            connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
        }
    }
}

// é??????´tab
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
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”???°SCII");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem2())
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”??­TF-8");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem3())
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”??­TF-16");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem4())
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”??­TF-32");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem5())
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”???¸BK");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem6())
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”????SO-8859-1");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem7())
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”????SO-8859-2");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem8())
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”????SO-8859-3");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem9())
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”????SO-8859-4");
        else if(encodingType->getListWidgetCurrentItem()==encodingType->getItem10())
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”????SO-8859-5");
        else
            EncodingTypeLabel->setText("?¤°????? é¨?‹­ç´?é?????ŸŸ???????´Ÿé”??­TF-8(??????î…???“ç??TF-8??????çˆœé”›???î›§é—‡â‚?é??­˜???é”???ƒî??é????ç´‘é????†î†•?????-??????çˆœé‚ç‘°ç??éˆ?????…‘?????œî”‘é–??¤‹???");
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

// é??????“éˆî„€????¤¤?œ­?´°
void MainWindow::newLocalProj()
{
    // é???ç??é?????€é??î†??…‘é????çˆ?é‘??‚œ???é’ç?????
    int header_count = tree_widget_item_header_file_name->childCount();
    for (int i = 0; i < header_count; i++)
    {
        delete tree_widget_item_header_file_name->child(0);
    }

    int source_count = tree_widget_item_source_file_name->childCount();
    for (int i = 0; i < source_count; i++)
    {
        delete tree_widget_item_source_file_name->child(0);
    }

    NewLocalProject *dialog = new NewLocalProject(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    // é??????“é¸?¤???
    connect(dialog->get_pushButton_new(), &QPushButton::clicked, this, [=]()
            {
        if(dialog->get_lineEdit_name()->text()=="")
        {
            QMessageBox::critical(this,"é–???’î??","ç’‡ç????­é????€é??î†??‚•");
            return;
        }
        else if(dialog->get_lineEdit_location()->text()=="")
        {
            QMessageBox::critical(this,"é–???’î??","ç’‡ç??â‚??¤‹??¨é‚???????¤¤?œ­?´°é????çŸ?????");
            return;
        }
        else
        {
            dialog->project_name=dialog->get_lineEdit_name()->text();
            current_project_name=dialog->project_name;
            //ç’????????¤°???????¤¤?œ­?´°é????çŸ????????ŸŒmainwindow??“î…¨ç´??¸š?¤¸ç°?é????ç”?é?????Šé??
            current_project_path=dialog->project_path+"/"+current_project_name;
            QDir dir;
            if(dir.mkdir(current_project_path))
            {
                QMessageBox::information(this,"??‡â„ƒ???","é????????¤¤?œ­?´°é???????");

                //???????€é??î†???????????????“î…?ƒ???????™é”?????”é???????éŠ†ä??ç°?é???????   é?????€é??î†???Šé???.txt
                QString header_file=current_project_path+"/Header";
                QString cpp_file=current_project_path+"/Source";
                QString information_file=current_project_path+"/CMakeLists.txt";
                QFile file(information_file);
                file.open(QIODevice::ReadWrite);

                file.write("cmake_minimum_required(VERSION 3.9)\n");
                file.write(("project(" + dialog->project_name + ")\n").toUtf8());
                file.write("set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)\n");
                file.write("set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)\n");
                file.write("set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)\n");
                file.write("SET(CMAKE_BUILD_TYPE \"Debug\")\n");
                file.write("SET(CMAKE_CXX_FLAGS_DEBUG \"$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb\")\n");
                file.write("SET(CMAKE_CXX_FLAGS_RELEASE \"$ENV{CXXFLAGS} -O3 -Wall\")\n");
                file.write("include_directories(Header)\n");
                file.write("file(GLOB SOURCES Source/*.cpp)\n");
                file.write(("add_executable("+dialog->project_name+" ${SOURCES})\n").toUtf8());

                file.close();
                dir.mkdir(header_file);
                dir.mkdir(cpp_file);

                //??“çƒ˜?ŸŠ?¤¤?œ­?´°é??—›?¤id********************************************************************************
                currentProject=local_project_id;
                local_project_id--;
                Project current_project(currentProject,current_project_name);
                debugInfo->insert(currentProject,new QMultiHash<QString,int>());

                //ç??—›?€é??î†???‘idé??­­roject????’´ç€?????’´?Šé”ç???ŸŒuserProjs????****************************************************************************************
                userProjs->insert(currentProject,current_project);

                //é???†îé??œŒirectoryé???????********************************************************************************
                Directory* dir=new Directory(currentProject,current_project_name,current_project_path,tree_widget_item_project_name);
                std::shared_ptr<Directory> Dir(dir);

                //??“çƒ˜ç‰´é???‚œ????????????(?¤¤?œ­?´°é?????é”›?¤Œ??‘treeItem???????§é—?????§????********************************************************************************
                QVariant var;
                var.setValue(Dir);
                tree_widget_item_project_name->setData(0,Qt::UserRole,var);
                tree_widget_item_project_name->setIcon(0, QIcon("://icon/PROJECT.png"));
                mainDirMap.insert(currentProject,Dir);

                //???????§éç????­é???
                tree_widget_item_project_name->setText(0,dialog->get_lineEdit_name()->text());
                ui->treeWidget->addTopLevelItem(tree_widget_item_project_name);

                //???????§ç€??????­é?
                tree_widget_item_project_name->addChild(tree_widget_item_file_information);
                tree_widget_item_header_file_name->setIcon(0,QIcon("://icon/H-.png"));
                tree_widget_item_project_name->addChild(tree_widget_item_header_file_name);
                tree_widget_item_source_file_name->setIcon(0,QIcon("://icon/cpp.png"));
                tree_widget_item_project_name->addChild(tree_widget_item_source_file_name);
            }
            else
            {
                QMessageBox::critical(this,"é–???’î??","é????????¤¤?œ­?´°?????????");
            }
            dialog->close();
        } });
}

// é????ç´‘éˆî„€????¤¤?œ­?´°é???????
void MainWindow::openLocalProj()
{
    // é???ç??é?????€é??î†??…‘é????çˆ?é‘??‚œ???é’ç?????
    int header_count = tree_widget_item_header_file_name->childCount();
    for (int i = 0; i < header_count; i++)
    {
        delete tree_widget_item_header_file_name->child(0);
    }

    int source_count = tree_widget_item_source_file_name->childCount();
    for (int i = 0; i < source_count; i++)
    {
        delete tree_widget_item_source_file_name->child(0);
    }

    // é‘??????‡é‚??????????œ­??‘é??î†????
    QString folder_path = QFileDialog::getExistingDirectory(this, tr("é–??¤‹??¨é??î†????"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    current_project_path = folder_path;

    // é‘??????‡ç€?????´°?¤°?
    QStringList dir_list;
    bool res = get_SubDir_Under_Dir(folder_path, dir_list);
    if (res == true)
    {

        if (is_legal_CSC_file("Header", dir_list) && is_legal_CSC_file("Source", dir_list))
        {
            QMessageBox::information(this, tr("?ˆ?ŠŸ"), "?ˆ?ŠŸ?‰“??€???ç›?");
        }
        else
        {
            QMessageBox::critical(this, tr("é”????"), "????‰“??€?ˆ??•çš„CloudSharedCoding???ç›?");
            return;
        }
    }
    else
    {
        QMessageBox::critical(this, tr("é”????"), "?‰“??€???ç›???????");
        return;
    }

    // ????–???ç›?çš??ç§?
    int last_index = folder_path.lastIndexOf('/');
    current_project_name = folder_path.mid(last_index + 1);

    // ??“é??î‡??¤¤?œ­?´°é??—›?¤id****************************************************************************************
    currentProject = local_project_id;
    local_project_id--;

    Project current_project(currentProject, current_project_name);
    debugInfo->insert(currentProject, new QMultiHash<QString, int>());

    // ?°†???ç›?çš„id?’ŒProject????„???????Š ?ˆ°userProjsä¸?****************************************************************************************
    userProjs->insert(currentProject, current_project);

    // ?????????????Š‚ç‚?çš??†…???
    tree_widget_item_project_name->setText(0, current_project_name);

    // ???????€Šç?‚??­é?§„?Šé”ç‡širectoryé…é???…˜é???????****************************************************************************************
    std::shared_ptr<Directory> Dir(new Directory(currentProject, current_project_name, current_project_path, tree_widget_item_project_name));
    QVariant var;
    var.setValue(Dir);
    tree_widget_item_project_name->setData(0, Qt::UserRole, var);
    tree_widget_item_project_name->setIcon(0, QIcon("://icon/PROJECT.png"));
    mainDirMap.insert(currentProject, Dir);

    // ??“çƒ˜?ŸŠé?????€é??î†??Šé”ç???ƒ??????çˆ?
    ui->treeWidget->addTopLevelItem(tree_widget_item_project_name);
    tree_widget_item_project_name->addChild(tree_widget_item_file_information);
    tree_widget_item_project_name->addChild(tree_widget_item_header_file_name);
    tree_widget_item_project_name->addChild(tree_widget_item_source_file_name);

    // ?–°???ä¸€ä¸?vector?­˜???ç€????œ°?–‡????‰€?œ‰çš„ä?????****************************************************************************************
    QVector<std::shared_ptr<FileInfo>> *file_info_ptr_vector = new QVector<std::shared_ptr<FileInfo>>;

    // ç€?ç…??†?¤¤?œ­?´°??“î… ??‘é??â‚?é??¤Š?”é???????
    QString header_path = current_project_path + "/Header";
    QStringList header_list;
    get_SubFile_Under_SubDir(header_path, header_list, 0);
    for (int i = 0; i < header_list.size(); i++)
    {
        std::shared_ptr<FileInfo> file_info_ptr(new FileInfo);
        // ??ä???Œ–file_info_ptrçš??†…???****************************************************************************************
        int current_file_id = local_file_id;
        local_file_id--;
        file_info_ptr->file_id = current_file_id;
        file_info_ptr->file_name = header_list[i];
        file_info_ptr->file_path = header_path + "/" + header_list[i];
        file_info_ptr->file_project = currentProject;
        file_info_ptr->file_privilege = 0;

        // ????Š ?ˆ°file_info_vectorä¸?****************************************************************************************
        file_info_ptr_vector->append(file_info_ptr);

        // ????“„?”é???????é??????­é?§„?ŸŠ??¤çƒ˜?ŸŠé‘??‚œ???
        MyTreeItem *item = new MyTreeItem(MyTreeItem::Type::FILE);
        item->setText(0, header_list[i]);
        item->setIcon(0, QIcon("://icon/H-.png"));
        tree_widget_item_header_file_name->addChild(item);

        // ä¸???ä¸€*.hçš„treeItemé™??Š ?†…?????Œé™„?Š çš??†…???ä¸?????–‡???çš??????????????Œ‡é’?****************************************************************************************
        QVariant var;
        var.setValue(file_info_ptr);
        item->setData(0, Qt::UserRole, var);
    }

    // ç€?ç…??†?¤¤?œ­?´°??“î… ??‘é??â‚?é??¤‹ç°?é???????
    QString source_path = current_project_path + "/Source";
    QStringList source_list;
    get_SubFile_Under_SubDir(source_path, source_list, 1);
    for (int i = 0; i < source_list.size(); i++)
    {
        std::shared_ptr<FileInfo> file_info_ptr(new FileInfo);
        // ??ä???Œ–file_info_ptrçš??†…???****************************************************************************************
        int current_file_id = local_file_id;
        local_file_id--;
        file_info_ptr->file_id = current_file_id;
        file_info_ptr->file_name = source_list[i];
        file_info_ptr->file_path = source_path + "/" + source_list[i];
        file_info_ptr->file_project = currentProject;
        file_info_ptr->file_privilege = 0;

        // ????Š ?ˆ°file_info_vectorä¸?****************************************************************************************
        file_info_ptr_vector->append(file_info_ptr);

        // ??“çƒ˜ç°?é???????é??????­é?§„?ŸŠ??¤çƒ˜?ŸŠé‘??‚œ???
        MyTreeItem *item = new MyTreeItem(MyTreeItem::Type::FILE);
        item->setText(0, source_list[i]);
        item->setIcon(0, QIcon("://icon/cpp.png"));
        tree_widget_item_source_file_name->addChild(item);

        // ä¸???ä¸€*.cppçš„treeItemé™??Š ?†…?????Œé™„?Š çš??†…???ä¸?????–‡???çš??????????????Œ‡é’?****************************************************************************************
        QVariant var;
        var.setValue(file_info_ptr);
        item->setData(0, Qt::UserRole, var);
    }

    // ç€?ç­?ro_fileMap??“î…Ÿ?Šé”ç???€é??ç”€Dé„ç??çš é??â‚?é??¤‹?ƒ????????Šé­îˆ???¤é???????šé???????‘vectoré???????
    pro_fileMap.insert(currentProject, file_info_ptr_vector);
}

// ???????§éˆî„€??´é‚??????
void MainWindow::addLocalFile()
{
    // é??‚†?Ÿ‡é„îˆš?ƒé™îˆ™??????????§é???????
    if (current_project_path == "")
    {
        QMessageBox::critical(this, "é”????", "????‰?–‡???ä¸???????????…ˆ?–°????–‡????ˆ–?‰“??€?–‡????œ¨?‰§???????Š ?–‡????“???");
        return;
    }

    // ????–???????Š çš??–‡?????­—?’Œ??????
    AddLocalFile *dialog = new AddLocalFile(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->set_lineEdit_path(current_project_path);
    dialog->show();

    // ??­î??ç•?ç’?îˆšç???????????é???????é??????
    connect(dialog->get_pushButton_add(), &QPushButton::clicked, this, [=]()
            {
        if(dialog->get_lineEdit_name()->text()=="")
        {
            QMessageBox::warning(this,"ç’€??€???","ç’‡ç????­é????ƒ?????????");
            return;
        }
        else if(!dialog->isLegal(dialog->get_lineEdit_name()->text()))
        {
            QMessageBox::warning(this,"ç’€??€???","ç’‡ç????­é????‚?¨‰?› ??‘é‚??????é???ç´™é™î„?‘ç’ç????˜éšî‚?ç“????????‹°é????ç“?");
            return;
        }
        else if(is_contain_file_name(dialog->get_lineEdit_name()->text(),*(pro_fileMap.value(currentProject))))
        {
            QMessageBox::warning(this,"ç’€??€???","?§?‚†?ƒ?????????ç€???????é”???ƒî??é??­˜???é???????é?????•€?????????");
            return;
        }
        else
        {
            if(dialog->get_comboBox_current_index()==0)//.cpp
            {
               //????–???????Š çš??–‡????????????????Š ?–‡???
               QString file_path=current_project_path+"/Source/"+dialog->get_lineEdit_name()->text()+".cpp";
               bool res=this->addFile(file_path);
               if(res==true)
                   QMessageBox::information(this,"é???????","???????§é???????é???????");
               else
               {
                   QMessageBox::information(this,"?????????","???????§é????????????????");
                   return;
               }

               //é?????????‡â„ƒ??…é¸??????******************************************************
               std::shared_ptr<FileInfo> file_info_ptr(new FileInfo);

               //??ä???Œ–?–‡??????????Œ‡é’?******************************************************
               int current_file_id=local_file_id;
               local_file_id--;
               file_info_ptr->file_id=current_file_id;
               file_info_ptr->file_name=dialog->get_lineEdit_name()->text()+".cpp";
               file_info_ptr->file_path=file_path;
               file_info_ptr->file_project=currentProject;
               file_info_ptr->file_privilege=0;

               //????Š ?ˆ°file_info_ptr_vector******************************************************
               //pro_fileMap.value(currentProject).push_back(file_info_ptr);
               auto vec = pro_fileMap.value(currentProject);
               vec->append(file_info_ptr);

               //????“„?”é???????é??????­é?§„?ŸŠ??¤çƒ˜?ŸŠé‘??‚œ???
               MyTreeItem* item=new MyTreeItem(MyTreeItem::Type::FILE);
               item->setText(0,dialog->get_lineEdit_name()->text()+".cpp");
               item->setIcon(0,QIcon("://icon/cpp.png"));
               tree_widget_item_source_file_name->addChild(item);

               //??“çƒ˜ç˜???“â????“î„?”é???????é—?????§é??‘­î†é”›??????é”ç????‘é?‘­î†???“é??î‡?????­˜?ƒ????‰??‘é…é¸??…˜??‡â„ƒ??…é¸??????******************************************************
               QVariant var;
               var.setValue(file_info_ptr);
               item->setData(0,Qt::UserRole,var);
            }
            else//.h
            {
                //????–???????Š çš??–‡????????????????Š ?–‡???
                QString  file_path1=current_project_path+"/Header/"+dialog->get_lineEdit_name()->text()+".h";
                QString  file_path2=current_project_path+"/Source/"+dialog->get_lineEdit_name()->text()+".cpp";
                bool res1=this->addFile(file_path1);
                bool res2=this->addFile(file_path2);
                if(res1==true&&res2==true)
                    QMessageBox::information(this,"é???????","???????§é???????é???????");
                else
                {
                    QMessageBox::information(this,"?????????","???????§é????????????????");
                    return;
                }

                //é?????????‡â„ƒ??…é¸??????******************************************************
                std::shared_ptr<FileInfo> file_info_ptr1(new FileInfo);
                std::shared_ptr<FileInfo> file_info_ptr2(new FileInfo);

                //??ä???Œ–?–‡??????????Œ‡é’?******************************************************
                //"*.h"?–‡???
                int current_file_id1=local_file_id;
                local_file_id--;
                file_info_ptr1->file_id=current_file_id1;
                file_info_ptr1->file_name=dialog->get_lineEdit_name()->text()+".h";
                file_info_ptr1->file_path=file_path1;
                file_info_ptr1->file_project=currentProject;
                file_info_ptr1->file_privilege=0;

                //"*.cpp"é???????
                int current_file_id2=local_file_id;
                local_file_id--;
                file_info_ptr2->file_id=current_file_id2;
                file_info_ptr2->file_name=dialog->get_lineEdit_name()->text()+".cpp";
                file_info_ptr2->file_path=file_path2;
                file_info_ptr2->file_project=currentProject;
                file_info_ptr2->file_privilege=0;

                //????Š ?ˆ°file_info_ptr_vector******************************************************
                pro_fileMap.value(currentProject)->append(file_info_ptr1);
                pro_fileMap.value(currentProject)->append(file_info_ptr2);

                //????“„?”é???????é??????­é?§„?ŸŠ??¤çƒ˜?ŸŠé‘??‚œ???
                MyTreeItem* item1=new MyTreeItem(MyTreeItem::Type::FILE);
                MyTreeItem* item2=new MyTreeItem(MyTreeItem::Type::FILE);
                item1->setText(0,dialog->get_lineEdit_name()->text()+".h");
                item2->setText(0,dialog->get_lineEdit_name()->text()+".cpp");
                item1->setIcon(0,QIcon("://icon/H-.png"));
                item2->setIcon(0,QIcon("://icon/cpp.png"));
                tree_widget_item_header_file_name->addChild(item1);
                tree_widget_item_source_file_name->addChild(item2);

                //??“çƒ˜ç˜???“â????“î„?”é???????é—?????§é??‘­î†é”›??????é”ç????‘é?‘­î†???“é??î‡?????­˜?ƒ????‰??‘é…é¸??…˜??‡â„ƒ??…é¸??????******************************************************
                //".h"é???????
                QVariant var1;
                var1.setValue(file_info_ptr1);
                item1->setData(0,Qt::UserRole,var1);

                //"cpp"é???????
                QVariant var2;
                var2.setValue(file_info_ptr2);
                item2->setData(0,Qt::UserRole,var2);
            }
            dialog->close();
        } });
}

// ?????†ç“¨éˆî„€????¤¤?œ­?´°é???????
void MainWindow::saveLocalProj()
{
    // ????œ‰?‰“??€???ç›?
    if (current_project_name == "" || current_project_path == "")
        return;

    // ????–?­˜????–‡??????????Œ‡é’ˆçš„vector?•°???
    QVector<std::shared_ptr<FileInfo>> *ptr_vector = pro_fileMap.value(currentProject);

    for (int i = 0; i < ptr_vector->size(); i++)
    {
        // ??ä???­˜çš??–‡???id?’Œcode_edit
        int file_id = ptr_vector->at(i)->file_id;
        if (!fileWidgets.contains(file_id))
            continue;
        CodeEdit *code_edit = fileWidgets.value(file_id);

        // ??ä???­˜çš??–‡?????????
        QString file_path = ptr_vector->at(i)->file_path;
        QFile file(file_path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream cout(&file);
            QString str = code_edit->getText();
            cout << str;
        }
        else
        {
            QMessageBox::critical(this, "é–???’î??", "?¤¤?œ­?´°?????†ç“¨?????????");
            return;
        }
    }
    QMessageBox::information(this, "é???????", "?¤¤?œ­?´°?????†ç“¨é???????");
}

// ?ˆ¤?–­????–°???çš??–‡?????????????????­˜???
bool MainWindow::is_contain_file_name(QString file_name, QVector<std::shared_ptr<FileInfo>> ptr_vector)
{
    for (int i = 0; i < ptr_vector.size(); i++)
    {
        if (file_name + ".h" == ptr_vector[i]->file_name || file_name + ".cpp" == ptr_vector[i]->file_name)
            return true;
        else
            continue;
    }
    return false;
}

// ?ˆ¤?–­?–‡???????????œ¨listä¸?
bool MainWindow::is_legal_CSC_file(QString file_name, QStringList list)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i] == file_name)
            return true;
        else
            continue;
    }
    return false;
}

// ???????•°çš„ä?œç”¨????œ¨?Œ‡??šçš„?????„ä¸‹?–°???ä¸€ä¸??–°çš??–‡????????????????????•ç???‹çš??–‡?????Œä?????.cpp .h .txt)
// ?????? D:D:/4.23/123.txt ?°†?œ¨D/4.23?–‡???ç›???•ä¸‹?–°???ä¸€ä¸?????123.txtçš??–‡????–‡????
bool MainWindow::addFile(QString file_path)
{
    // é??????“é‚??????
    QFile *new_file = new QFile(this);
    new_file->setFileName(file_path);
    bool res = new_file->open(QIODevice::ReadWrite | QIODevice::Text);
    new_file->close();

    return res;
}

// ???????•°çš„ä?œç”¨????œ¨?????šçš„?????„ä¸‹?°†?–‡????‰“??€????„é€ ä¸€ä¸??–‡??????????™¨?’Œ????Š ?ˆ°tabWidgetä¸?
void MainWindow::openFileAndAddTab(QString file_path)
{
    QFileInfo info(file_path);
    std::shared_ptr<FileInfo> file_information(new FileInfo);
    file_information->file_name = info.fileName();
    file_information->file_path = info.filePath();

    file_information->file_project = -1;

    // file_information?„é€????ä¸€ä¸?code_edit?–‡????????????????
    CodeEdit *code_edit = new CodeEdit(file_information, this);

    // ?–°???ä¸€ä¸?tab?Š ????ˆ°tabWidget???????
    ui->tabWidget->addTab(code_edit, file_information->file_name);
    file_information->is_open = true;

    // ????–?–‡???çš??†…???????‰“?°?ˆ°code_edit?????????????
    QFile file(file_path);
    file.open(QIODevice::ReadOnly);
    QByteArray array = file.readAll();
    code_edit->addText(array);

    // ??“â????“ç??athç€?ç‘°ç????“â????“çŒšode_edité???????é”?????Šé”ç???ŸŒé„ç??çš ç›?„¤???
    mp[file_information->file_path] = code_edit;
}

// ???????•°çš„ä?œç”¨????œ¨?????šçš„?????„ä¸‹????–??“ä¸­çš??‰€?œ‰?–‡????????????????Š ?ˆ°?‚?•°QStringList???????
bool MainWindow::get_SubDir_Under_Dir(QString path, QStringList &list)
{
    QDir *dir = new QDir(path);
    // ?????…ç“¨é????î„é??î†????
    if (!dir->exists())
    {
        delete dir;
        dir = nullptr;
        return false;
    }
    else
    {
        list = dir->entryList(QDir::Dirs); // é??›¨?§‘???????¸´é?????ƒ???????™
        list.removeOne(".");
        list.removeOne("..");
        delete dir;
        dir = nullptr;
        return true;
    }
}

// ???????•°çš„ä?œç”¨????œ¨?????šçš„?–‡??????ä¸?????–??“ä¸­çš??‰€?œ‰?–‡?????????????Š ?ˆ°?‚?•°QStringList???????(?‚?•°tag1==0?Œ‡?˜???????–çš?"*.h"?–‡?????Œtag==1?Œ‡?˜???????–çš?"*.cpp"?–‡???)
bool MainWindow::get_SubFile_Under_SubDir(QString path, QStringList &list, int tag)
{
    QDir *dir = new QDir(path);
    // ?????…ç“¨é????î„é??î†????
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
    runbutton->setEnabled(false);
    debugbutton->setEnabled(false);

    runningProject = currentProject;

    disconnect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
    runDockwidget->clear();
    connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));

    if (currentProject < 0)
    {
        if (runThread != nullptr)
        {
            disconnect(runThread);
            runThread->deleteLater();
        }
        QString path = current_project_path;
        runThread = new RunThread(path, this);
        connect(runThread, SIGNAL(buildInfo(QString)), this, SLOT(appendBuildText(QString)));
        connect(runThread, SIGNAL(stdOut(QString)), this, SLOT(appendRunningText(QString)));
        connect(runThread, &RunThread::buildFinish, this, [=]()
                {
            statusIcon->movie()->stop();
            runningMovie->start();
            statusIcon->setMovie(runningMovie); });
        connect(runThread, &RunThread::runFinish, this, [=](int n)
                {

            statusIcon->movie()->stop();
            stateokMovie->start();
            statusIcon->setMovie(stateokMovie);
            ui->tabWidget->setTabsClosable(true);
            workState = ProjectWorkState::NONE;
            runbutton->setEnabled(true);
            debugbutton->setEnabled(true);
            stopRun->setEnabled(false);
            runDockwidget->setEnabled(true);

            appendRunningText("Exit with "+QString::number(n)); });
        runThread->run();
    }
    else
    {
        workState = ProjectWorkState::RUNNING;
        runningProject = currentProject;
        Package pck(QString::number(currentProject).toUtf8(), (int)Package::PackageType::RUN_PROJECT);
        socket->write(pck.getPdata(), pck.getSize());
    }
}

void MainWindow::appendBuildText(QString text)
{
    disconnect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
    buildDockwidget->insertPlainText(text);
    connect(buildDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
    buildDockwidget->verticalScrollBar()->setValue(buildDockwidget->verticalScrollBar()->maximum());
}

void MainWindow::appendRunningText(QString text)
{
    disconnect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
    runDockwidget->insertPlainText(text);
    connect(runDockwidget->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(cmdStdin(int, int, int)));
    runDockwidget->verticalScrollBar()->setValue(runDockwidget->verticalScrollBar()->maximum());
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

void MainWindow::setSystemVar()
{
    process->setProgram("g++");
    process->setNativeArguments(" -v -E -x c++ -");
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start();
    process->waitForStarted();
    // process->waitForFinished();
    QRegularExpression includePath = QRegularExpression("(\\w:.*include).c\\+\\+");
    auto match = includePath.match(data, 0);
    systemVar = match.captured();
    QString b = systemVar;
    QString c = data;
    int a = 0;
}
