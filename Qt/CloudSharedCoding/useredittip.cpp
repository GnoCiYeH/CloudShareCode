#include "useredittip.h"
#include "ui_useredittip.h"

UserEditTip::UserEditTip(QString str,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserEditTip)
{
    ui->setupUi(this);
    ui->label->setText(str);
}

UserEditTip::~UserEditTip()
{
    delete ui;
}
