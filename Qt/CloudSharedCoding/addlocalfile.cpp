#include "addlocalfile.h"
#include "ui_addlocalfile.h"

AddLocalFile::AddLocalFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLocalFile)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Source(.cpp)");
    ui->comboBox->addItem("Header(.h)");
    ui->comboBox->addItem("Class");
    ui->comboBox->setCurrentIndex(0);

    //取消
    connect(ui->pushButton_cancel,&QPushButton::clicked,this,[=](){
        this->close();
    });

    //变更路径
    connect(ui->pushButton_change,&QPushButton::clicked,this,[=](){

    });
}

bool AddLocalFile::isLegal(QString str)
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

QPushButton* AddLocalFile::get_pushButton_add()
{
    return ui->pushButton_add;
}

QLineEdit* AddLocalFile::get_lineEdit_name()
{
    return ui->lineEdit_name;
}

void AddLocalFile::set_lineEdit_path(QString str)
{
    path=str;
    ui->lineEdit_path->setText(path);
}

int AddLocalFile::get_comboBox_current_index()
{
    return ui->comboBox->currentIndex();
}

AddLocalFile::~AddLocalFile()
{
    delete ui;
}
