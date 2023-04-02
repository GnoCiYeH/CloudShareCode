#include "projectform.h"
#include "ui_projectform.h"
#include"mainwindow.h"
#include"newprojectdialog.h"
#include"package.h"

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
    NewProjectDialog dialog(this);
    dialog.exec();
}

void ProjectForm::addItem(Project proj)
{
    if(proj.pro_owner==MainWindow::userId)
    {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setText(proj.pro_name);
        QVariant var;
        var.setValue(proj);
        item->setData(Qt::UserRole,var);
        ui->listWidget->addItem(item);
    }
    else{
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget_2);
        item->setText(proj.pro_name);
        QVariant var;
        var.setValue(proj);
        item->setData(Qt::UserRole,var);
        ui->listWidget_2->addItem(item);
    }
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


void ProjectForm::on_pushButton_3_clicked()
{
    if(ui->tabWidget->currentWidget()==ui->tab)
    {
        QListWidgetItem* litem = ui->listWidget->currentItem();
        QVariant var = litem->data(Qt::UserRole);
        Project proj = var.value<Project>();

        QMessageBox::StandardButton result = QMessageBox::warning(this,"确定删除？","您确定要删除该项目："+proj.pro_name+"?");
        if(result!=QMessageBox::StandardButton::Ok)
            return;

        QString data = QString::number(proj.pro_id)+"\t"+proj.pro_name;
        Package pck(data.toUtf8(),Package::PackageType::DEL_PROJECT);
        MainWindow::socket->write(pck.getPdata(),pck.getSize());
        ui->listWidget->removeItemWidget(litem);
        delete litem;
        for(int i = 0; i<MainWindow::userProjs->size();i++)
        {
            auto item = MainWindow::userProjs->at(i);
            if(item.pro_id==proj.pro_id)
            {
                MainWindow::userProjs->erase(MainWindow::userProjs->begin()+i);
                break;
            }
        }
    }
    else if(ui->tabWidget->currentWidget()==ui->tab_2)
    {

    }
}


void ProjectForm::on_pushButton_2_clicked()
{
    QListWidgetItem* litem = ui->listWidget->currentItem();
    QVariant var = litem->data(Qt::UserRole);
    Project proj = var.value<Project>();

    emit openProj(proj.pro_id);
}

