#ifndef NEWLOCALFILE_H
#define NEWLOCALFILE_H

#include <QDialog>
#include<QPushButton>
#include<QFileDialog>
#include<QMessageBox>
#include<QFile>
#include<QLineEdit>
#include<QString>

namespace Ui {
class NewLocalFile;
}

class NewLocalFile : public QDialog
{
    Q_OBJECT

public:
    explicit NewLocalFile(QWidget *parent = nullptr);
    ~NewLocalFile();
    bool isLegal(QString str);//判断文件名的格式是否正确
    QPushButton* getPushButtonConfrim();
    QLineEdit* getLineEdit_Name();
    QLineEdit* getLineEdit_Path();
    QString file_name;
    QString file_temp_path;
    QString file_path;

    QString getFileName();
    QString getFileTempPath();
    QString getFilePath();

    void setFileName(QString);
    void setFileTempPath(QString);
    void setFilePath(QString);
private:
    Ui::NewLocalFile *ui;
};

#endif // NEWLOCALFILE_H
