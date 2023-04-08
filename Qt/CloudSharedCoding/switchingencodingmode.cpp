#include "switchingencodingmode.h"
#include "ui_switchingencodingmode.h"

SwitchingEncodingMode::SwitchingEncodingMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SwitchingEncodingMode)
{
    ui->setupUi(this);
}

QPushButton* SwitchingEncodingMode::getButtonConfirm()
{
    return ui->pushButton_confirm;
}

QPushButton* SwitchingEncodingMode::getButtonCancel()
{
    return ui->pushButton_cancel;
}

QRadioButton* SwitchingEncodingMode::getRadioButtonASCII()
{
    return ui->ASCII;
}


QRadioButton* SwitchingEncodingMode::getRadioButtonUTF8()
{
    return ui->UTF8;
}

QRadioButton* SwitchingEncodingMode::getRadioButtonGBK()
{
    return ui->GBK;
}

QRadioButton* SwitchingEncodingMode::getRadioButtonISO()
{
    return ui->ISO;
}

SwitchingEncodingMode::~SwitchingEncodingMode()
{
    delete ui;
}
