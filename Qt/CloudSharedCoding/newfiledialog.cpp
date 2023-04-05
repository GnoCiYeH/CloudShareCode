#include "newfiledialog.h"
#include "ui_newfiledialog.h"
#include"mainwindow.h"
#include "package.h"
#include<QMessageBox>

NewFileDialog::NewFileDialog(std::shared_ptr<Directory> dir,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFileDialog)
{
    ui->setupUi(this);
    this->m_dir = dir;
    auto proj = MainWindow::userProjs->value(m_dir->pro_id);
    switch (proj.pro_privilege_level) {
    case 0:
    case 1:
    {
        ui->comboBox->removeItem(3);
        ui->comboBox->removeItem(4);
        break;
    }
    case 2:
    {
        ui->comboBox->removeItem(3);
        ui->comboBox->removeItem(4);
        break;
    }
    case 3:
    {
        ui->comboBox->removeItem(2);
        ui->comboBox->removeItem(3);
        ui->comboBox->removeItem(4);
        break;
    }
    case 4:
    {
        ui->comboBox->setDisabled(true);
        ui->pushButton->setDisabled(true);
        ui->pushButton->setWhatsThis("您的权限不足，不能添加/删除/更改文件");
        break;
    }
    default:
        break;
    }
}

NewFileDialog::~NewFileDialog()
{
    delete ui;
}

void NewFileDialog::on_pushButton_clicked()
{
    int num = m_dir->dir_item->childCount();
    QString filename = ui->lineEdit->text();
    for (int i = 0; i < num; ++i) {
        if(filename == m_dir->dir_item->child(i)->text(0))
        {
            QMessageBox::warning(this,"ERROR","该文件已存在！");
            return;
        }
    }

    QString path = m_dir->dir_path+filename;
    filename = filename.mid(filename.lastIndexOf("/")+1);
    short level = 4 - ui->comboBox->currentIndex();

    if(MainWindow::loginState())
    {
        QString data = filename + "\t" + path + "\t" + QString::number(m_dir->pro_id) + "\t" + QString::number(level);
        Package pck(data.toUtf8(),Package::PackageType::NEW_FILE);

        MainWindow::socket->write(pck.getPdata(),pck.getSize());
    }
    else{

    }

    this->close();
}


void NewFileDialog::on_pushButton_2_clicked()
{

}

