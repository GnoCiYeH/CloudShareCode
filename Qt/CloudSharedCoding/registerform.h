#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include<QTcpSocket>
#include <QMessageBox>

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = nullptr);
    ~RegisterForm();

signals:
    void succeedRig();
private slots:
    void on_pushButton_clicked();

private:
    Ui::RegisterForm *ui;
    QTcpSocket* socket;
    QMessageBox* box;
};

#endif // REGISTERFORM_H
