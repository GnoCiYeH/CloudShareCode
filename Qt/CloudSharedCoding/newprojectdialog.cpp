#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include"mainwindow.h"
#include"package.h"

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

void NewProjectDialog::on_pushButton_clicked()
{
    QString name = ui->lineEdit_pro_name->text();
    Package pck(name.toUtf8(),Package::PackageType::NEW_PROJECT);
    MainWindow::socket->write(pck.getPdata(),pck.getSize());

    this->close();
}


void NewProjectDialog::on_pushButton_2_clicked()
{
    this->close();
}

