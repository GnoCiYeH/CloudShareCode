#include "useredittip.h"
#include "ui_useredittip.h"

UserEditTip::UserEditTip(QString str,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserEditTip)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("QLabel{background-color:rgb(255,250,205);}");
    ui->label->setText(str);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        this->hide();
    });
}

UserEditTip::~UserEditTip()
{
    delete ui;
}

void UserEditTip::showTip()
{
    this->show();
    timer->setInterval(3000);
    timer->start();
}
