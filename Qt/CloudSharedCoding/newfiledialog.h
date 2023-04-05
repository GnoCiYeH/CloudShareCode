#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include<memory>
#include"InfoType.h"

namespace Ui {
class NewFileDialog;
}

class NewFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileDialog(std::shared_ptr<Directory> dir, QWidget *parent = nullptr);
    ~NewFileDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::NewFileDialog *ui;

    std::shared_ptr<Directory> m_dir;
};

#endif // NEWFILEDIALOG_H
