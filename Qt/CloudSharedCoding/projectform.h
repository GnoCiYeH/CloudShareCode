#ifndef PROJECTFORM_H
#define PROJECTFORM_H

#include <QWidget>
#include<QTcpSocket>

namespace Ui {
class ProjectForm;
}

class ProjectForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectForm(QWidget *parent = nullptr);
    ~ProjectForm();

private slots:
    void on_toolButton_clicked();

private:
    Ui::ProjectForm *ui;
};

#endif // PROJECTFORM_H
