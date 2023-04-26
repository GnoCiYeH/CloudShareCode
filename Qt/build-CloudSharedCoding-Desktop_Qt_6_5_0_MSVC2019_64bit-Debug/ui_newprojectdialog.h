/********************************************************************************
** Form generated from reading UI file 'newprojectdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPROJECTDIALOG_H
#define UI_NEWPROJECTDIALOG_H

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

class Ui_NewProjectDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_pro_dir;
    QLineEdit *lineEdit_path;
    QPushButton *pushButton_select_dir;
    QLineEdit *lineEdit_pro_name;
    QComboBox *comboBox;

    void setupUi(QDialog *NewProjectDialog)
    {
        if (NewProjectDialog->objectName().isEmpty())
            NewProjectDialog->setObjectName("NewProjectDialog");
        NewProjectDialog->resize(393, 304);
        gridLayout = new QGridLayout(NewProjectDialog);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(NewProjectDialog);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 3, 1, 1, 1);

        widget = new QWidget(NewProjectDialog);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout->addWidget(pushButton_2);


        gridLayout->addWidget(widget, 5, 0, 1, 4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 2, 1, 1);

        label = new QLabel(NewProjectDialog);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 4, 2, 1, 1);

        label_pro_dir = new QLabel(NewProjectDialog);
        label_pro_dir->setObjectName("label_pro_dir");

        gridLayout->addWidget(label_pro_dir, 2, 1, 1, 1);

        lineEdit_path = new QLineEdit(NewProjectDialog);
        lineEdit_path->setObjectName("lineEdit_path");

        gridLayout->addWidget(lineEdit_path, 2, 2, 1, 1);

        pushButton_select_dir = new QPushButton(NewProjectDialog);
        pushButton_select_dir->setObjectName("pushButton_select_dir");

        gridLayout->addWidget(pushButton_select_dir, 2, 3, 1, 1);

        lineEdit_pro_name = new QLineEdit(NewProjectDialog);
        lineEdit_pro_name->setObjectName("lineEdit_pro_name");

        gridLayout->addWidget(lineEdit_pro_name, 1, 2, 1, 2);

        comboBox = new QComboBox(NewProjectDialog);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        gridLayout->addWidget(comboBox, 3, 2, 1, 2);


        retranslateUi(NewProjectDialog);

        QMetaObject::connectSlotsByName(NewProjectDialog);
    } // setupUi

    void retranslateUi(QDialog *NewProjectDialog)
    {
        NewProjectDialog->setWindowTitle(QCoreApplication::translate("NewProjectDialog", "Dialog", nullptr));
        label_2->setText(QCoreApplication::translate("NewProjectDialog", "\351\241\271\347\233\256\347\261\273\345\236\213", nullptr));
        pushButton->setText(QCoreApplication::translate("NewProjectDialog", "\347\241\256\345\256\232", nullptr));
        pushButton_2->setText(QCoreApplication::translate("NewProjectDialog", "\345\217\226\346\266\210", nullptr));
        label->setText(QCoreApplication::translate("NewProjectDialog", "\351\241\271\347\233\256\345\220\215\347\247\260", nullptr));
        label_pro_dir->setText(QCoreApplication::translate("NewProjectDialog", "\351\241\271\347\233\256\350\267\257\345\276\204", nullptr));
        lineEdit_path->setText(QCoreApplication::translate("NewProjectDialog", "./", nullptr));
        pushButton_select_dir->setText(QCoreApplication::translate("NewProjectDialog", "\351\200\211\346\213\251", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("NewProjectDialog", "c++", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("NewProjectDialog", "\347\251\272", nullptr));

    } // retranslateUi

};

namespace Ui {
    class NewProjectDialog: public Ui_NewProjectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWPROJECTDIALOG_H
