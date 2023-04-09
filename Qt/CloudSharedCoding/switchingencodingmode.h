#ifndef SWITCHINGENCODINGMODE_H
#define SWITCHINGENCODINGMODE_H

#include <QDialog>
#include<QRadioButton>
#include<QPushButton>
#include<QListWidget>
namespace Ui {
class SwitchingEncodingMode;
}

class SwitchingEncodingMode : public QDialog
{
    Q_OBJECT

public:
    explicit SwitchingEncodingMode(QWidget *parent = nullptr);
    Ui::SwitchingEncodingMode* getUI();
    QPushButton* getButtonConfirm();
    QPushButton* getButtonCancel();
    QListWidget* getListWidget();
    QListWidgetItem* getListWidgetCurrentItem();
    QListWidgetItem* getItem1();
    QListWidgetItem* getItem2();
    QListWidgetItem* getItem3();
    QListWidgetItem* getItem4();
    QListWidgetItem* getItem5();
    QListWidgetItem* getItem6();
    QListWidgetItem* getItem7();
    QListWidgetItem* getItem8();
    QListWidgetItem* getItem9();
    QListWidgetItem* getItem10();
    ~SwitchingEncodingMode();
private:
    Ui::SwitchingEncodingMode *ui;
    QListWidgetItem* item1=new QListWidgetItem("ASCII");
    QListWidgetItem* item2=new QListWidgetItem("UTF-8");
    QListWidgetItem* item3=new QListWidgetItem("UTF-16");
    QListWidgetItem* item4=new QListWidgetItem("UTF-32");
    QListWidgetItem* item5=new QListWidgetItem("GBK");
    QListWidgetItem* item6=new QListWidgetItem("ISO-8859-1");
    QListWidgetItem* item7=new QListWidgetItem("ISO-8859-2");
    QListWidgetItem* item8=new QListWidgetItem("ISO-8859-3");
    QListWidgetItem* item9=new QListWidgetItem("ISO-8859-4");
    QListWidgetItem* item10=new QListWidgetItem("ISO-8859-5");




};

#endif // SWITCHINGENCODINGMODE_H
