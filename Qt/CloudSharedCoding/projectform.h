#ifndef PROJECTFORM_H
#define PROJECTFORM_H

#include <QWidget>
#include<QTcpSocket>
#include<QMessageBox>

namespace Ui {
class ProjectForm;
}

class ProjectForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectForm(QWidget *parent = nullptr);
    ~ProjectForm();

public slots:
    void init();

private slots:
    void on_toolButton_clicked();

private:
    Ui::ProjectForm *ui;

    QMessageBox * box;
};

#endif // PROJECTFORM_H
