#include "newlocalfile.h"
#include "ui_newlocalfile.h"
#include<QDebug>

NewLocalFile::NewLocalFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewLocalFile)
{
    ui->setupUi(this);

    //选择路径
    connect(ui->pushButton_select,&QPushButton::clicked,this,[=](){
        QString path=QFileDialog::getExistingDirectory(this,"选择路径","./");//获取目标路径
        if(path.isEmpty())
            return;
        else
        {
            file_temp_path=path;
            ui->lineEdit_path->setText(file_temp_path);
        }
    });

    /*
    //确定路径以及文件名称
    connect(ui->pushButton_confirm,&QPushButton::clicked,this,[=](){
        if(ui->lineEdit_name->text()=="")
        {
            QMessageBox::warning(this,"警告","请输入文件名称");
            return;
        }
        else if(!isLegal(ui->lineEdit_name->text()))
        {
            QMessageBox::warning(this,"警告","请输入合法的文件名（只允许包含字母和数字）");
            return;
        }
        else
        {
            file_name=ui->lineEdit_name->text()+".txt";
            QString finally_path=file_path+"/"+file_name;
            QFile *new_file=new QFile(this);
            new_file->setFileName(finally_path);
            qDebug()<<finally_path<<"    "<<file_name;
            bool res=new_file->open(QIODevice::ReadWrite|QIODevice::Text);
            new_file->close();
            if(!res)
            {
                QMessageBox::critical(this,"错误","文件新建失败");
                return;
            }
            else
            {
                this->close();
                return;
            }
        }
    });
    */

    //取消
    connect(ui->pushButton_cancel,&QPushButton::clicked,this,&NewLocalFile::close);
}

bool NewLocalFile::isLegal(QString str)
{
    int length=str.length();
    for(int i=0;i<length;i++)
    {
        if((str.mid(i,1)>='a'&&str.mid(i,1)<='z')||(str.mid(i,1)>='A'&&str.mid(i,1)<='Z')||(str.mid(i,1)>='0'&&str.mid(i,1)<='9'))
            continue;
        else
            return 0;
    }
    return 1;
}

QPushButton* NewLocalFile::getPushButtonConfrim()
{
    return ui->pushButton_confirm;
}

QLineEdit* NewLocalFile::getLineEdit_Name()
{
    return ui->lineEdit_name;
}

QLineEdit* NewLocalFile::getLineEdit_Path()
{
    return ui->lineEdit_path;
}

QString NewLocalFile::getFileName()
{
    return this->file_name;
}

QString NewLocalFile::getFileTempPath()
{
    return this->file_temp_path;
}

QString NewLocalFile::getFilePath()
{
    return this->file_path;
}

void NewLocalFile::setFileName(QString str)
{
    this->file_name=str;
}

void NewLocalFile::setFileTempPath(QString str)
{
    this->file_temp_path=str;
}
void NewLocalFile::setFilePath(QString str)
{
    this->file_path=str;
}

NewLocalFile::~NewLocalFile()
{
    delete ui;
}
