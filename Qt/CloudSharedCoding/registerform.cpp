#include "registerform.h"
#include "ui_registerform.h"
#include "package.h"

RegisterForm::RegisterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterForm)
{
    ui->setupUi(this);
    ui->lineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lineEdit_2->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lineEdit_3->setAttribute(Qt::WA_InputMethodEnabled, false);

    box = new QMessageBox(this);
    box->hide();
    box->setWindowTitle("Tips");
    box->setText("Wait for the server to respond...");
    socket = new QTcpSocket(this);
    socket->connectToHost("47.115.211.238",9098);

    connect(socket,&QTcpSocket::readyRead,this,[=](){
    QByteArray arr = socket->read(8);
    int type = Package::ByteArrToInt(arr, 0);
    int packageSize = Package::ByteArrToInt(arr, 4);
    switch (type)
    {
        case (int)Package::ReturnType::REGISTER_OK:
        {
            box->close();
            QMessageBox::information(this,"Succeed","Registration successful, about to go to login...");
            emit succeedRig();
            this->deleteLater();
            break;
        }
        case (int)Package::ReturnType::REGISTER_ERROR:
        {
            QString error(socket->read(packageSize));
            box->close();
            QMessageBox::warning(this,"Warning",error);
            break;
        }
    };
    });
}

RegisterForm::~RegisterForm()
{
    delete ui;
    socket->close();
    socket->deleteLater();
}

void RegisterForm::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty())
    {
        QMessageBox::warning(this,"Warning","Ensure the integrity of the submitted data!");
    }
    else if(ui->lineEdit_2->text()!=ui->lineEdit_3->text())
    {
        QMessageBox::warning(this,"Warning","The password is not the same two times!");
    }
    else
    {
        QString data = ui->lineEdit->text()+"\t"+ui->lineEdit_2->text();
        Package pck(data.toUtf8(),(int)Package::PackageType::REGISTER);
        socket->write(pck.getPdata(),pck.getSize());
        box->exec();
    }
}

