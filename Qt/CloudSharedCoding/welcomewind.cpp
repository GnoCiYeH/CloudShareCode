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

}

