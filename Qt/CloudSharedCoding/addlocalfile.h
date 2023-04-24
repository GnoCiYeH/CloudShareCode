#ifndef ADDLOCALFILE_H
#define ADDLOCALFILE_H

#include<QPushButton>
#include<QMessageBox>
#include <QDialog>
#include<QFileDialog>
#include<QLineEdit>

namespace Ui {
class AddLocalFile;
}

class AddLocalFile : public QDialog
{
    Q_OBJECT

public:
    explicit AddLocalFile(QWidget *parent = nullptr);
    QPushButton* get_pushButton_add();
    QLineEdit* get_lineEdit_name();
    void set_lineEdit_path(QString);
    bool isLegal(QString str);//判断文件名的格式是否正确
    int get_comboBox_current_index();
    ~AddLocalFile();

private:
    Ui::AddLocalFile *ui;
    QString name;
    QString path;
};

#endif // ADDLOCALFILE_H
