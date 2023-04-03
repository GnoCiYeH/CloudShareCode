#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include"mainwindow.h"
#include"package.h"

NewProjectDialog::NewProjectDialog(bool isLocal,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
    this->isLocal = isLocal;

    if(!isLocal)
    {
        ui->label_pro_dir->hide();
        ui->lineEdit_path->hide();
        ui->pushButton_select_dir->hide();
    }
    else
    {
        ui->label_pro_dir->show();
        ui->lineEdit_path->show();
        ui->pushButton_select_dir->show();
    }
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

void NewProjectDialog::on_pushButton_clicked()
{
    if(!isLocal)
    {
        QString name = ui->lineEdit_pro_name->text();
        Package pck(name.toUtf8(),Package::PackageType::NEW_PROJECT);
        MainWindow::socket->write(pck.getPdata(),pck.getSize());

        this->close();
    }
    else
    {
        emit newLocalProInfo(ui->lineEdit_pro_name->text(),ui->lineEdit_path->text());
    }
}


void NewProjectDialog::on_pushButton_2_clicked()
{
    this->close();
}

