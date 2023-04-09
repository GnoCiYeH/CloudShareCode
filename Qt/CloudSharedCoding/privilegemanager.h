#ifndef PRIVILEGEMANAGER_H
#define PRIVILEGEMANAGER_H

#include <QWidget>
#include<QHash>

namespace Ui {
class PrivilegeManager;
}

class PrivilegeManager : public QWidget
{
    Q_OBJECT

public:
    explicit PrivilegeManager(QString data,QWidget *parent = nullptr);
    ~PrivilegeManager();

public slots:
    void itemChange(QString,int);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::PrivilegeManager *ui;

    QHash<QString,int> levelMap;
    QString proId;
};

#endif // PRIVILEGEMANAGER_H
