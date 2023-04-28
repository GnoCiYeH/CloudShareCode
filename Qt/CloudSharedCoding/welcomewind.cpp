#include "welcomewind.h"
#include "ui_welcomewind.h"
#include"mainwindow.h"
#include"package.h"

WelcomeWind::WelcomeWind(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWind)
{
    ui->setupUi(this);
}

WelcomeWind::~WelcomeWind()
{
    delete ui;
}

void WelcomeWind::on_pushButton_clicked()
{
    MainWindow::Login();
    if(MainWindow::loginState())
    {
        //从服务器拉取项目信息
        Package pck("",(int)Package::PackageType::INIT_PROJS);
        MainWindow::socket->write(pck.getPdata(),pck.getSize());
        ui->pushButton->setHidden(true);
        ui->pushButton_4->setHidden(true);
    }
}


void WelcomeWind::on_pushButton_4_clicked()
{
    emit registerRequest();
}

void WelcomeWind::addHistoryListItem(QString listInfo)
{
    QStringList list = listInfo.split("\n");
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setIcon(QIcon("://icon/PROJECT.png"));
    item->setText(list[0]+"\t\t"+list[1]);
    item->setData(Qt::UserRole,listInfo);
}



void WelcomeWind::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString listinfo = item->data(Qt::UserRole).toString();
    QStringList list = listinfo.split("\n");
    emit historyProjectemit(list[1]);
}

void WelcomeWind::update(QQueue<QString> historyQueue)
{
    ui->listWidget->clear();
    for(int i = historyQueue.length()-1,n = 0;i >= 0;i--,n++)
    {
        addHistoryListItem(historyQueue.at(i));
    }
}


void WelcomeWind::on_pushButton_3_clicked()
{
    emit openProj();
}


void WelcomeWind::on_pushButton_2_clicked()
{
    emit newProj();
}

