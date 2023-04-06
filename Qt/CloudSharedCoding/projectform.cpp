#include "projectform.h"
#include "ui_projectform.h"
#include"mainwindow.h"
#include"newprojectdialog.h"
#include"package.h"
#include<QInputDialog>

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
    NewProjectDialog dialog(false,this);
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
        Package pck(data.toUtf8(),(int)Package::PackageType::DEL_PROJECT);
        MainWindow::socket->write(pck.getPdata(),pck.getSize());
        ui->listWidget->removeItemWidget(litem);
        delete litem;
        MainWindow::userProjs->remove(proj.pro_id);
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
    this->close();
}


void ProjectForm::on_toolButton_3_clicked()
{
    QString uuid = QInputDialog::getText(this,"加入项目","请输入项目uuid");
    if(uuid.size()!=36)
    {
        QMessageBox::warning(this,"错误","请输入正确uuid");
        return;
    }

    Package pck(uuid.toUtf8(),(int)Package::PackageType::JOIN_PROJECT);
    MainWindow::socket->write(pck.getPdata(),pck.getSize());
}


void ProjectForm::on_pushButton_clicked()
{
    auto item = ui->listWidget->currentItem();
    Project proj = item->data(Qt::UserRole).value<Project>();

    QMessageBox box;
    box.setWindowTitle("分享项目");
    box.setText("项目分享码: " + proj.pro_uuid);
    box.setTextInteractionFlags(Qt::TextSelectableByMouse);
    box.exec();
}


void ProjectForm::on_pushButton_4_clicked()
{
    QListWidgetItem* litem = ui->listWidget_2->currentItem();
    QVariant var = litem->data(Qt::UserRole);
    Project proj = var.value<Project>();

    emit openProj(proj.pro_id);
    this->close();
}

