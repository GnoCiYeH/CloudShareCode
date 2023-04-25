/********************************************************************************
** Form generated from reading UI file 'newlocalfile.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWLOCALFILE_H
#define UI_NEWLOCALFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_NewLocalFile
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton_cancel;
    QPushButton *pushButton_confirm;
    QLabel *label_2;
    QLineEdit *lineEdit_path;
    QPushButton *pushButton_select;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QLineEdit *lineEdit_name;

    void setupUi(QDialog *NewLocalFile)
    {
        if (NewLocalFile->objectName().isEmpty())
            NewLocalFile->setObjectName("NewLocalFile");
        NewLocalFile->resize(489, 162);
        gridLayout = new QGridLayout(NewLocalFile);
        gridLayout->setObjectName("gridLayout");
        pushButton_cancel = new QPushButton(NewLocalFile);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setMinimumSize(QSize(80, 20));
        QFont font;
        font.setPointSize(12);
        pushButton_cancel->setFont(font);

        gridLayout->addWidget(pushButton_cancel, 2, 3, 1, 1);

        pushButton_confirm = new QPushButton(NewLocalFile);
        pushButton_confirm->setObjectName("pushButton_confirm");
        pushButton_confirm->setMinimumSize(QSize(80, 20));
        pushButton_confirm->setFont(font);

        gridLayout->addWidget(pushButton_confirm, 2, 1, 1, 1);

        label_2 = new QLabel(NewLocalFile);
        label_2->setObjectName("label_2");
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_path = new QLineEdit(NewLocalFile);
        lineEdit_path->setObjectName("lineEdit_path");

        gridLayout->addWidget(lineEdit_path, 1, 1, 1, 3);

        pushButton_select = new QPushButton(NewLocalFile);
        pushButton_select->setObjectName("pushButton_select");

        gridLayout->addWidget(pushButton_select, 1, 4, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 1);

        label = new QLabel(NewLocalFile);
        label->setObjectName("label");
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit_name = new QLineEdit(NewLocalFile);
        lineEdit_name->setObjectName("lineEdit_name");

        gridLayout->addWidget(lineEdit_name, 0, 1, 1, 4);


        retranslateUi(NewLocalFile);

        QMetaObject::connectSlotsByName(NewLocalFile);
    } // setupUi

    void retranslateUi(QDialog *NewLocalFile)
    {
        NewLocalFile->setWindowTitle(QCoreApplication::translate("NewLocalFile", "Dialog", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("NewLocalFile", "\345\217\226\346\266\210", nullptr));
        pushButton_confirm->setText(QCoreApplication::translate("NewLocalFile", "\347\241\256\345\256\232", nullptr));
        label_2->setText(QCoreApplication::translate("NewLocalFile", "\351\241\271\347\233\256\350\267\257\345\276\204\357\274\232", nullptr));
        pushButton_select->setText(QCoreApplication::translate("NewLocalFile", "\351\200\211\346\213\251", nullptr));
        label->setText(QCoreApplication::translate("NewLocalFile", "\351\241\271\347\233\256\345\220\215\347\247\260\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewLocalFile: public Ui_NewLocalFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWLOCALFILE_H
