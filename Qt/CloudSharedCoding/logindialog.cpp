#include "logindialog.h"
#include "ui_logindialog.h"
#include"myhelper.h"
#include"package.h"
#include"mainwindow.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    QTcpSocket* socket = MainWindow::socket;

    connect(socket,&QTcpSocket::connected,this,[=](){
        QString data = ui->lineEdit_UserID->text() + "\t" + ui->lineEdit_Password->text();
        userID = ui->lineEdit_UserID->text();
        Package pck(data.toUtf8(),Package::PackageType::LOGIN);
        socket->write(pck.getPdata(),pck.getSize());
    });
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_clicked()
{
    ui->pushButton->setDisabled(true);
    QString IP = ui->lineEdit_ServerIp->text();
    if(ui->lineEdit_UserID->text().size()>32||ui->lineEdit_Password->text().size()>32||!myHelper::isIP(IP))
    {
        QMessageBox box(this);
        box.setText("请确认信息输入无误后重试！");
        box.setWindowTitle("WARING");
        box.exec();
        ui->pushButton->setDisabled(false);
    }
    else
    {
        MainWindow::socket->connectToHost(IP,9098);
    }
}

void LoginDialog::loginSucceed()
{
    QMessageBox::about(this,"登录信息","登陆成功！");
    emit loginAllowded();
    this->deleteLater();
}

