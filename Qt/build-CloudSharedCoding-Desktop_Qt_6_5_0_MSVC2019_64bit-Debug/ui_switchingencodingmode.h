/********************************************************************************
** Form generated from reading UI file 'switchingencodingmode.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SWITCHINGENCODINGMODE_H
#define UI_SWITCHINGENCODINGMODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SwitchingEncodingMode
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QRadioButton *ASCII;
    QSpacerItem *verticalSpacer;
    QRadioButton *UTF8;
    QSpacerItem *verticalSpacer_2;
    QRadioButton *GBK;
    QSpacerItem *verticalSpacer_3;
    QRadioButton *ISO;
    QLabel *label;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_confirm;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_cancel;

    void setupUi(QDialog *SwitchingEncodingMode)
    {
        if (SwitchingEncodingMode->objectName().isEmpty())
            SwitchingEncodingMode->setObjectName("SwitchingEncodingMode");
        SwitchingEncodingMode->resize(400, 300);
        widget = new QWidget(SwitchingEncodingMode);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(20, 50, 281, 161));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        ASCII = new QRadioButton(widget);
        ASCII->setObjectName("ASCII");

        verticalLayout->addWidget(ASCII);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        UTF8 = new QRadioButton(widget);
        UTF8->setObjectName("UTF8");

        verticalLayout->addWidget(UTF8);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        GBK = new QRadioButton(widget);
        GBK->setObjectName("GBK");

        verticalLayout->addWidget(GBK);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        ISO = new QRadioButton(widget);
        ISO->setObjectName("ISO");

        verticalLayout->addWidget(ISO);

        label = new QLabel(SwitchingEncodingMode);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 30, 131, 16));
        widget_2 = new QWidget(SwitchingEncodingMode);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(80, 230, 261, 51));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        pushButton_confirm = new QPushButton(widget_2);
        pushButton_confirm->setObjectName("pushButton_confirm");

        horizontalLayout->addWidget(pushButton_confirm);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_cancel = new QPushButton(widget_2);
        pushButton_cancel->setObjectName("pushButton_cancel");

        horizontalLayout->addWidget(pushButton_cancel);


        retranslateUi(SwitchingEncodingMode);

        QMetaObject::connectSlotsByName(SwitchingEncodingMode);
    } // setupUi

    void retranslateUi(QDialog *SwitchingEncodingMode)
    {
        SwitchingEncodingMode->setWindowTitle(QCoreApplication::translate("SwitchingEncodingMode", "Dialog", nullptr));
        ASCII->setText(QCoreApplication::translate("SwitchingEncodingMode", "ASCII", nullptr));
        UTF8->setText(QCoreApplication::translate("SwitchingEncodingMode", "UTF-8", nullptr));
        GBK->setText(QCoreApplication::translate("SwitchingEncodingMode", "GBK", nullptr));
        ISO->setText(QCoreApplication::translate("SwitchingEncodingMode", "ISO", nullptr));
        label->setText(QCoreApplication::translate("SwitchingEncodingMode", "\350\257\267\351\200\211\346\213\251\347\274\226\347\240\201\346\226\271\345\274\217\357\274\232", nullptr));
        pushButton_confirm->setText(QCoreApplication::translate("SwitchingEncodingMode", "\347\241\256\345\256\232", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("SwitchingEncodingMode", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SwitchingEncodingMode: public Ui_SwitchingEncodingMode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SWITCHINGENCODINGMODE_H
