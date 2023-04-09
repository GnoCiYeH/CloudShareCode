#ifndef PRIVILEGELISTITEM_H
#define PRIVILEGELISTITEM_H

#include <QWidget>

namespace Ui {
class PrivilegeListItem;
}

class PrivilegeListItem : public QWidget
{
    Q_OBJECT

public:
    explicit PrivilegeListItem(QString userId,short level,QWidget *parent = nullptr);
    ~PrivilegeListItem();

signals:
    void itemChanged(QString,int);

private slots:
    void comboxChanged(int);

private:
    Ui::PrivilegeListItem *ui;

    QString userId;
    short level;
};

#endif // PRIVILEGELISTITEM_H
