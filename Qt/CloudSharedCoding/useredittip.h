#ifndef USEREDITTIP_H
#define USEREDITTIP_H

#include <QWidget>

namespace Ui {
class UserEditTip;
}

class UserEditTip : public QWidget
{
    Q_OBJECT

public:
    explicit UserEditTip(QString str,QWidget *parent = nullptr);
    ~UserEditTip();

private:
    Ui::UserEditTip *ui;
};

#endif // USEREDITTIP_H
