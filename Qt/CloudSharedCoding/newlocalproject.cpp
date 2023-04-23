#include "newlocalproject.h"
#include "ui_newlocalproject.h"
#include<QDebug>

NewLocalProject::NewLocalProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewLocalProject)
{
    ui->setupUi(this);

    //选择按钮
    connect(ui->pushButton_select,&QPushButton::clicked,this,[=](){
        QString folder_path = QFileDialog::getExistingDirectory(this, tr("选择目录"),"/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        ui->lineEdit_location->setText(folder_path);
        this->project_path=folder_path;
    });

    //新建按钮
    /*
    connect(ui->pushButton_new,&QPushButton::clicked,this,[=](){
        if(ui->lineEdit_name->text()=="")
        {
            QMessageBox::critical(this,"错误","请输入项目名称");
            return;
        }
        else if(ui->lineEdit_location->text()=="")
        {
            QMessageBox::critical(this,"错误","请选择新建项目的路径");
            return;
        }
        else
        {
            QMessageBox::information(this,"信息","新建项目成功");
            this->project_name=ui->lineEdit_name->text();
            QDir dir(this->project_path+"/"+project_name);
            if(!dir.exists())
            {
                dir.mkdir(".");
            }
            this->close();
        }
    });
    */

    //取消按钮
    connect(ui->pushButton_cancel,&QPushButton::clicked,this,[=](){
        this->close();
    });
}

//封装
QPushButton* NewLocalProject::get_pushButton_new()
{
    return ui->pushButton_new;
}

QLineEdit* NewLocalProject::get_lineEdit_name()
{
    return ui->lineEdit_name;
}

QLineEdit* NewLocalProject::get_lineEdit_location()
{
    return ui->lineEdit_location;
}

NewLocalProject::~NewLocalProject()
{
    delete ui;
}
