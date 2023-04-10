/********************************************************************************
** Form generated from reading UI file 'newfiledialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWFILEDIALOG_H
#define UI_NEWFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewFileDialog
{
public:
    QGridLayout *gridLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBox;

    void setupUi(QDialog *NewFileDialog)
    {
        if (NewFileDialog->objectName().isEmpty())
            NewFileDialog->setObjectName("NewFileDialog");
        NewFileDialog->resize(302, 101);
        gridLayout = new QGridLayout(NewFileDialog);
        gridLayout->setObjectName("gridLayout");
        widget = new QWidget(NewFileDialog);
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

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addWidget(widget, 2, 0, 1, 2);

        lineEdit = new QLineEdit(NewFileDialog);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        label = new QLabel(NewFileDialog);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(NewFileDialog);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        comboBox = new QComboBox(NewFileDialog);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        gridLayout->addWidget(comboBox, 1, 1, 1, 1);


        retranslateUi(NewFileDialog);

        QMetaObject::connectSlotsByName(NewFileDialog);
    } // setupUi

    void retranslateUi(QDialog *NewFileDialog)
    {
        NewFileDialog->setWindowTitle(QCoreApplication::translate("NewFileDialog", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("NewFileDialog", "\347\241\256\345\256\232", nullptr));
        pushButton_2->setText(QCoreApplication::translate("NewFileDialog", "\345\217\226\346\266\210", nullptr));
        label->setText(QCoreApplication::translate("NewFileDialog", "\346\226\260\345\273\272\346\226\207\344\273\266", nullptr));
        label_2->setText(QCoreApplication::translate("NewFileDialog", "\346\226\207\344\273\266\346\235\203\351\231\220", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("NewFileDialog", "public", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("NewFileDialog", "default", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("NewFileDialog", "privite", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("NewFileDialog", "protected", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("NewFileDialog", "root", nullptr));

    } // retranslateUi

};

namespace Ui {
    class NewFileDialog: public Ui_NewFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWFILEDIALOG_H
