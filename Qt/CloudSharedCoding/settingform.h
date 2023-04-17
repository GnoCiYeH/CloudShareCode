#ifndef SETTINGFORM_H
#define SETTINGFORM_H

#include <QListWidgetItem>
#include <QWidget>
#include <QQueue>
#include<QSettings>
#include"InfoType.h"

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
    void themeChanged(int);
    void codeThemeChanged(HightlightType,QColor);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    enum SettingType{
        THEME,
        CODETHEME
    };

    Ui::SettingForm *ui;
    QSettings* settings;
    QQueue<QPair<SettingType,QVariant>> settingQueue;
};

#endif // SETTINGFORM_H
