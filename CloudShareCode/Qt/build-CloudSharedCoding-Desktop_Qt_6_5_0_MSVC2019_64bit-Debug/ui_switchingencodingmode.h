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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SwitchingEncodingMode
{
public:
    QLabel *label;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_confirm;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_cancel;
    QListWidget *listWidget;

    void setupUi(QDialog *SwitchingEncodingMode)
    {
        if (SwitchingEncodingMode->objectName().isEmpty())
            SwitchingEncodingMode->setObjectName("SwitchingEncodingMode");
        SwitchingEncodingMode->resize(400, 300);
        label = new QLabel(SwitchingEncodingMode);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 20, 191, 31));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);
        label->setScaledContents(false);
        widget_2 = new QWidget(SwitchingEncodingMode);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(80, 230, 261, 51));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        pushButton_confirm = new QPushButton(widget_2);
        pushButton_confirm->setObjectName("pushButton_confirm");
        pushButton_confirm->setMinimumSize(QSize(80, 30));
        pushButton_confirm->setFont(font);

        horizontalLayout->addWidget(pushButton_confirm);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_cancel = new QPushButton(widget_2);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setMinimumSize(QSize(80, 30));
        pushButton_cancel->setFont(font);

        horizontalLayout->addWidget(pushButton_cancel);

        listWidget = new QListWidget(SwitchingEncodingMode);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(40, 70, 341, 151));

        retranslateUi(SwitchingEncodingMode);

        QMetaObject::connectSlotsByName(SwitchingEncodingMode);
    } // setupUi

    void retranslateUi(QDialog *SwitchingEncodingMode)
    {
        SwitchingEncodingMode->setWindowTitle(QCoreApplication::translate("SwitchingEncodingMode", "Dialog", nullptr));
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
