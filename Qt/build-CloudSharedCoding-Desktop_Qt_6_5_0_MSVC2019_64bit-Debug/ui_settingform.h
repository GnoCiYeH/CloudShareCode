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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingForm
{
public:
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *comboBox;
    QWidget *page_2;
    QWidget *page_3;
    QFormLayout *formLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QLabel *label_4;
    QLineEdit *lineEdit_3;
    QListWidget *listWidget;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QWidget *SettingForm)
    {
        if (SettingForm->objectName().isEmpty())
            SettingForm->setObjectName("SettingForm");
        SettingForm->resize(798, 464);
        gridLayout = new QGridLayout(SettingForm);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 5);
        stackedWidget = new QStackedWidget(SettingForm);
        stackedWidget->setObjectName("stackedWidget");
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        page = new QWidget();
        page->setObjectName("page");
        formLayout = new QFormLayout(page);
        formLayout->setObjectName("formLayout");
        label = new QLabel(page);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        comboBox = new QComboBox(page);
        comboBox->setObjectName("comboBox");

        formLayout->setWidget(0, QFormLayout::FieldRole, comboBox);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        formLayout_2 = new QFormLayout(page_3);
        formLayout_2->setObjectName("formLayout_2");
        label_2 = new QLabel(page_3);
        label_2->setObjectName("label_2");

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit = new QLineEdit(page_3);
        lineEdit->setObjectName("lineEdit");

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEdit);

        label_3 = new QLabel(page_3);
        label_3->setObjectName("label_3");

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_3);

        lineEdit_2 = new QLineEdit(page_3);
        lineEdit_2->setObjectName("lineEdit_2");

        formLayout_2->setWidget(0, QFormLayout::FieldRole, lineEdit_2);

        label_4 = new QLabel(page_3);
        label_4->setObjectName("label_4");

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_4);

        lineEdit_3 = new QLineEdit(page_3);
        lineEdit_3->setObjectName("lineEdit_3");

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEdit_3);

        stackedWidget->addWidget(page_3);

        gridLayout->addWidget(stackedWidget, 0, 1, 1, 3);

        listWidget = new QListWidget(SettingForm);
        listWidget->setObjectName("listWidget");
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);
        listWidget->setMaximumSize(QSize(190, 16777215));

        gridLayout->addWidget(listWidget, 0, 0, 2, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(SettingForm);
        pushButton_2->setObjectName("pushButton_2");

        gridLayout->addWidget(pushButton_2, 1, 2, 1, 1);

        pushButton = new QPushButton(SettingForm);
        pushButton->setObjectName("pushButton");

        gridLayout->addWidget(pushButton, 1, 3, 1, 1);


        retranslateUi(SettingForm);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(SettingForm);
    } // setupUi

    void retranslateUi(QWidget *SettingForm)
    {
        SettingForm->setWindowTitle(QCoreApplication::translate("SettingForm", "Form", nullptr));
        label->setText(QCoreApplication::translate("SettingForm", "\344\270\273\351\242\230", nullptr));
        label_2->setText(QCoreApplication::translate("SettingForm", "\345\214\205\345\220\253\350\267\257\345\276\204", nullptr));
        label_3->setText(QCoreApplication::translate("SettingForm", "\351\241\271\347\233\256\350\276\223\345\207\272\350\267\257\345\276\204", nullptr));
        label_4->setText(QCoreApplication::translate("SettingForm", "link", nullptr));
        pushButton_2->setText(QCoreApplication::translate("SettingForm", "\347\241\256\345\256\232", nullptr));
        pushButton->setText(QCoreApplication::translate("SettingForm", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingForm: public Ui_SettingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGFORM_H
