#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"myhelper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myHelper::setStyle(":/qss/psblack.css");
    this->setWindowTitle("CloudSharedCoding");
}

MainWindow::~MainWindow()
{
    delete ui;
}
