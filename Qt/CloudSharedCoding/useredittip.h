#ifndef USEREDITTIP_H
#define USEREDITTIP_H

#include <QWidget>
#include<QTimer>

namespace Ui {
class UserEditTip;
}

class UserEditTip : public QWidget
{
    Q_OBJECT

public:
    explicit UserEditTip(QString str,QWidget *parent = nullptr);
    ~UserEditTip();

    void showTip();

private:
    Ui::UserEditTip *ui;

    QTimer* timer;
};

#endif // USEREDITTIP_H
