#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include "ui_newprojectdialog.h"

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(bool isLocal,QWidget *parent = nullptr);
    ~NewProjectDialog();

signals:
    void newLocalProInfo(QString,QString);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::NewProjectDialog *ui;

    bool isLocal = false;
    int mode=0;
};

#endif // NEWPROJECTDIALOG_H
