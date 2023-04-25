/********************************************************************************
** Form generated from reading UI file 'addlocalfile.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDLOCALFILE_H
#define UI_ADDLOCALFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddLocalFile
{
public:
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEdit_name;
    QLabel *label_4;
    QComboBox *comboBox;
    QSpacerItem *verticalSpacer;
    QLabel *label_3;
    QLabel *label_2;
    QLineEdit *lineEdit_path;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton_change;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QPushButton *pushButton_add;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_cancel;
    QSpacerItem *horizontalSpacer_6;

    void setupUi(QDialog *AddLocalFile)
    {
        if (AddLocalFile->objectName().isEmpty())
            AddLocalFile->setObjectName("AddLocalFile");
        AddLocalFile->resize(400, 171);
        QFont font;
        font.setPointSize(10);
        AddLocalFile->setFont(font);
        widget = new QWidget(AddLocalFile);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, 10, 321, 111));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName("gridLayout_2");
        lineEdit_name = new QLineEdit(widget);
        lineEdit_name->setObjectName("lineEdit_name");

        gridLayout_2->addWidget(lineEdit_name, 2, 1, 1, 1);

        label_4 = new QLabel(widget);
        label_4->setObjectName("label_4");
        QFont font1;
        font1.setPointSize(12);
        label_4->setFont(font1);

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        comboBox = new QComboBox(widget);
        comboBox->setObjectName("comboBox");

        gridLayout_2->addWidget(comboBox, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 1, 1, 1);

        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");
        label_3->setFont(font1);

        gridLayout_2->addWidget(label_3, 4, 0, 1, 1);

        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        label_2->setFont(font1);

        gridLayout_2->addWidget(label_2, 2, 0, 1, 1);

        lineEdit_path = new QLineEdit(widget);
        lineEdit_path->setObjectName("lineEdit_path");

        gridLayout_2->addWidget(lineEdit_path, 4, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 3, 1, 1, 1);

        pushButton_change = new QPushButton(AddLocalFile);
        pushButton_change->setObjectName("pushButton_change");
        pushButton_change->setGeometry(QRect(340, 90, 51, 20));
        widget_2 = new QWidget(AddLocalFile);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(130, 120, 161, 51));
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setObjectName("gridLayout");
        pushButton_add = new QPushButton(widget_2);
        pushButton_add->setObjectName("pushButton_add");
        QFont font2;
        font2.setPointSize(15);
        pushButton_add->setFont(font2);

        gridLayout->addWidget(pushButton_add, 0, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 0, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 4, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 5, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 6, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 3, 1, 1);

        pushButton_cancel = new QPushButton(widget_2);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setFont(font2);

        gridLayout->addWidget(pushButton_cancel, 0, 7, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 0, 1, 1, 1);


        retranslateUi(AddLocalFile);

        QMetaObject::connectSlotsByName(AddLocalFile);
    } // setupUi

    void retranslateUi(QDialog *AddLocalFile)
    {
        AddLocalFile->setWindowTitle(QCoreApplication::translate("AddLocalFile", "Dialog", nullptr));
        label_4->setText(QCoreApplication::translate("AddLocalFile", "\346\226\207\344\273\266\347\261\273\345\236\213\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("AddLocalFile", "\346\211\200\345\234\250\344\275\215\347\275\256\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("AddLocalFile", "\346\226\207\344\273\266\345\220\215\347\247\260\357\274\232", nullptr));
        pushButton_change->setText(QCoreApplication::translate("AddLocalFile", "\346\233\264\346\224\271", nullptr));
        pushButton_add->setText(QCoreApplication::translate("AddLocalFile", "\346\267\273\345\212\240", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("AddLocalFile", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddLocalFile: public Ui_AddLocalFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDLOCALFILE_H
