#include "welcomewind.h"
#include "ui_welcomewind.h"

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
