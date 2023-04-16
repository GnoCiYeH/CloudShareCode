#ifndef SETTINGFORM_H
#define SETTINGFORM_H

#include <QListWidgetItem>
#include <QWidget>

namespace Ui {
class SettingForm;
}

class SettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingForm(QWidget *parent = nullptr);
    ~SettingForm();

private slots:
    void itemChanged(int);

private:
    Ui::SettingForm *ui;
};

#endif // SETTINGFORM_H
