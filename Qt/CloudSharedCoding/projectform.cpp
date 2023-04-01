#include "projectform.h"
#include "ui_projectform.h"
#include"mainwindow.h"

ProjectForm::ProjectForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectForm)
{
    ui->setupUi(this);
}

ProjectForm::~ProjectForm()
{
    delete ui;
}

void ProjectForm::on_toolButton_clicked()
{

}

void ProjectForm::init()
{
    for(auto i : *MainWindow::userProjs)
    {
        QListWidgetItem* item = new QListWidgetItem(i.pro_name);
        QVariant var;
        var.setValue(i);
        item->setData(Qt::UserRole,var);
        if(i.pro_owner==MainWindow::userId)
        {
            ui->listWidget->addItem(item);
        }
        else
        {
            ui->listWidget_2->addItem(item);
        }
    }
}

