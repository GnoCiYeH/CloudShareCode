#include "privilegelistitem.h"
#include "ui_privilegelistitem.h"

PrivilegeListItem::PrivilegeListItem(QString userId,short level,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivilegeListItem)
{
    ui->setupUi(this);

    this->userId = userId;
    this->level = level;

    ui->userId->setText(userId);
    ui->comboBox->setCurrentIndex(4-level);

    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChanged(int)));
}

PrivilegeListItem::~PrivilegeListItem()
{
    delete ui;
}

void PrivilegeListItem::comboxChanged(int index)
{
    emit itemChanged(userId,4-index);
}

