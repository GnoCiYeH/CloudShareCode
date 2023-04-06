#ifndef PROJECTFORM_H
#define PROJECTFORM_H

#include <QWidget>
#include<QTcpSocket>
#include<QMessageBox>
#include<QListWidget>
#include"InfoType.h"

namespace Ui {
class ProjectForm;
}

class ProjectForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectForm(QWidget *parent = nullptr);
    ~ProjectForm();

    void addItem(Project proj);

signals:
    void openProj(int);

public slots:
    void init();

private slots:
    void on_toolButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::ProjectForm *ui;

    QMessageBox * box;
};

#endif // PROJECTFORM_H
