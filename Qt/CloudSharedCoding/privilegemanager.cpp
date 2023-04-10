#include "privilegemanager.h"
#include "ui_privilegemanager.h"
#include"privilegelistitem.h"
#include"mainwindow.h"
#include "package.h"

PrivilegeManager::PrivilegeManager(QString data,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivilegeManager)
{
    ui->setupUi(this);

    auto itemInfoList = data.split("\n",Qt::SkipEmptyParts);
    for(int i = 1; i < itemInfoList.size() ; i++)
    {
        auto info = itemInfoList[i];
        auto list = info.split("\t");
        PrivilegeListItem* itemWidget = new PrivilegeListItem(list[0],list[1].toShort(),this);
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(itemWidget->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item,itemWidget);
        connect(itemWidget,SIGNAL(itemChanged(QString,int)),this,SLOT(itemChange(QString,int)));
    }

    ui->label_people_count->setText(QString::number(itemInfoList.size()));
    ui->label_pro_name->setText(MainWindow::userProjs->find(itemInfoList[0].toInt()).value().pro_name);
    this->proId = itemInfoList[0];
}

PrivilegeManager::~PrivilegeManager()
{
    delete ui;
}

void PrivilegeManager::itemChange(QString userId,int level)
{
    levelMap.insert(userId,level);
}

void PrivilegeManager::on_pushButton_2_clicked()
{
    this->deleteLater();
}


void PrivilegeManager::on_pushButton_clicked()
{
    QString data = proId + "\n";
    for (auto it = levelMap.begin();it!=levelMap.end();it++) {
        data+=it.key()+"\t"+QString::number(it.value())+"\n";
    }

    Package pck(data.toUtf8(),(int)Package::PackageType::PRIVILEGE_UPDATE);
    MainWindow::socket->write(pck.getPdata(),pck.getSize());

    this->deleteLater();
}

