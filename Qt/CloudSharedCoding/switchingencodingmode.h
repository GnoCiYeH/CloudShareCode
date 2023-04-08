#ifndef SWITCHINGENCODINGMODE_H
#define SWITCHINGENCODINGMODE_H

#include <QDialog>
#include<QRadioButton>
#include<QPushButton>
namespace Ui {
class SwitchingEncodingMode;
}

class SwitchingEncodingMode : public QDialog
{
    Q_OBJECT

public:
    explicit SwitchingEncodingMode(QWidget *parent = nullptr);
    QPushButton* getButtonConfirm();
    QPushButton* getButtonCancel();
    QRadioButton* getRadioButtonASCII();
    QRadioButton* getRadioButtonUTF8();
    QRadioButton* getRadioButtonGBK();
    QRadioButton* getRadioButtonISO();
    ~SwitchingEncodingMode();
    Ui::SwitchingEncodingMode *ui;



};

#endif // SWITCHINGENCODINGMODE_H
