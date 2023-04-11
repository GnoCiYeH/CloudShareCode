/********************************************************************************
** Form generated from reading UI file 'settingform.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGFORM_H
#define UI_SETTINGFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingForm
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QWidget *tab_2;
    QWidget *tab_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *SettingForm)
    {
        if (SettingForm->objectName().isEmpty())
            SettingForm->setObjectName("SettingForm");
        SettingForm->resize(492, 517);
        verticalLayout = new QVBoxLayout(SettingForm);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 5);
        tabWidget = new QTabWidget(SettingForm);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        gridLayout = new QGridLayout(tab);
        gridLayout->setObjectName("gridLayout");
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        label = new QLabel(tab);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_2 = new QLineEdit(tab);
        lineEdit_2->setObjectName("lineEdit_2");

        gridLayout->addWidget(lineEdit_2, 1, 1, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        tabWidget->addTab(tab_3, QString());

        verticalLayout->addWidget(tabWidget);

        widget = new QWidget(SettingForm);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout->addWidget(widget);


        retranslateUi(SettingForm);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingForm);
    } // setupUi

    void retranslateUi(QWidget *SettingForm)
    {
        SettingForm->setWindowTitle(QCoreApplication::translate("SettingForm", "Form", nullptr));
        label->setText(QCoreApplication::translate("SettingForm", "\351\241\271\347\233\256\344\277\235\345\255\230\350\267\257\345\276\204", nullptr));
        label_2->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("SettingForm", "\346\234\254\345\234\260\351\241\271\347\233\256\350\256\276\347\275\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("SettingForm", "\344\272\221\351\241\271\347\233\256\350\256\276\347\275\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("SettingForm", "\344\270\273\351\242\230\351\243\216\346\240\274", nullptr));
        pushButton->setText(QCoreApplication::translate("SettingForm", "PushButton", nullptr));
        pushButton_2->setText(QCoreApplication::translate("SettingForm", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingForm: public Ui_SettingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGFORM_H
