#ifndef NEWLOCALPROJECT_H
#define NEWLOCALPROJECT_H
#include<QPushButton>
#include<QMessageBox>
#include <QDialog>
#include<QDir>
#include<QFileDialog>
#include<QLineEdit>

namespace Ui {
class NewLocalProject;
}

class NewLocalProject : public QDialog
{
    Q_OBJECT

public:
    explicit NewLocalProject(QWidget *parent = nullptr);
    ~NewLocalProject();
    //封装
    QPushButton* get_pushButton_new();
    QLineEdit* get_lineEdit_name();
    QLineEdit* get_lineEdit_location();
    QString project_name;
    QString project_path;

private:
    Ui::NewLocalProject *ui;
};

#endif // NEWLOCALPROJECT_H
