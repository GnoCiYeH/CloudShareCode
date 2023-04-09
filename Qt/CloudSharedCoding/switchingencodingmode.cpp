#include "switchingencodingmode.h"
#include "ui_switchingencodingmode.h"

SwitchingEncodingMode::SwitchingEncodingMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SwitchingEncodingMode)
{
    ui->setupUi(this);
    ui->listWidget->addItem(item1);
    ui->listWidget->addItem(item2);
    ui->listWidget->addItem(item3);
    ui->listWidget->addItem(item4);
    ui->listWidget->addItem(item5);
    ui->listWidget->addItem(item6);
    ui->listWidget->addItem(item7);
    ui->listWidget->addItem(item8);
    ui->listWidget->addItem(item9);
    ui->listWidget->addItem(item10);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);//设置单选
}

QListWidget* SwitchingEncodingMode::getListWidget()
{
    return ui->listWidget;
}

QListWidgetItem* SwitchingEncodingMode::getListWidgetCurrentItem()
{
    return ui->listWidget->currentItem();
}

QPushButton* SwitchingEncodingMode::getButtonConfirm()
{
    return ui->pushButton_confirm;
}

QPushButton* SwitchingEncodingMode::getButtonCancel()
{
    return ui->pushButton_cancel;
}

QListWidgetItem* SwitchingEncodingMode::getItem1()
{
    return item1;
}

QListWidgetItem* SwitchingEncodingMode::getItem2()
{
    return item2;
}

QListWidgetItem* SwitchingEncodingMode::getItem3()
{
    return item3;
}

QListWidgetItem* SwitchingEncodingMode::getItem4()
{
    return item4;
}

QListWidgetItem* SwitchingEncodingMode::getItem5()
{
    return item5;
}

QListWidgetItem* SwitchingEncodingMode::getItem6()
{
    return item6;
}

QListWidgetItem* SwitchingEncodingMode::getItem7()
{
    return item7;
}

QListWidgetItem* SwitchingEncodingMode::getItem8()
{
    return item8;
}

QListWidgetItem* SwitchingEncodingMode::getItem9()
{
    return item9;
}

QListWidgetItem* SwitchingEncodingMode::getItem10()
{
    return item10;
}

Ui::SwitchingEncodingMode* SwitchingEncodingMode::getUI()
{
    return ui;
}

SwitchingEncodingMode::~SwitchingEncodingMode()
{
    delete ui;
}
