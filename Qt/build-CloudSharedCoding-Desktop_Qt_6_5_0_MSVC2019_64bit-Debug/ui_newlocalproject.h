/********************************************************************************
** Form generated from reading UI file 'newlocalproject.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWLOCALPROJECT_H
#define UI_NEWLOCALPROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewLocalProject
{
public:
    QLabel *label;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;
    QLabel *label_3;
    QLineEdit *lineEdit_location;
    QLineEdit *lineEdit_name;
    QPushButton *pushButton_select;
    QWidget *widget_2;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_cancel;
    QPushButton *pushButton_new;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *NewLocalProject)
    {
        if (NewLocalProject->objectName().isEmpty())
            NewLocalProject->setObjectName("NewLocalProject");
        NewLocalProject->resize(400, 250);
        label = new QLabel(NewLocalProject);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 161, 31));
        QFont font;
        font.setPointSize(16);
        label->setFont(font);
        widget = new QWidget(NewLocalProject);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 53, 391, 111));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName("gridLayout");
        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        QFont font1;
        font1.setPointSize(12);
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");
        label_3->setFont(font1);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        lineEdit_location = new QLineEdit(widget);
        lineEdit_location->setObjectName("lineEdit_location");

        gridLayout->addWidget(lineEdit_location, 2, 1, 1, 1);

        lineEdit_name = new QLineEdit(widget);
        lineEdit_name->setObjectName("lineEdit_name");

        gridLayout->addWidget(lineEdit_name, 0, 1, 1, 2);

        pushButton_select = new QPushButton(widget);
        pushButton_select->setObjectName("pushButton_select");
        pushButton_select->setMinimumSize(QSize(40, 22));
        pushButton_select->setFont(font1);

        gridLayout->addWidget(pushButton_select, 2, 2, 1, 1);

        widget_2 = new QWidget(NewLocalProject);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(80, 190, 251, 51));
        gridLayout_2 = new QGridLayout(widget_2);
        gridLayout_2->setObjectName("gridLayout_2");
        pushButton_cancel = new QPushButton(widget_2);
        pushButton_cancel->setObjectName("pushButton_cancel");
        QFont font2;
        font2.setPointSize(15);
        pushButton_cancel->setFont(font2);

        gridLayout_2->addWidget(pushButton_cancel, 0, 2, 1, 1);

        pushButton_new = new QPushButton(widget_2);
        pushButton_new->setObjectName("pushButton_new");
        pushButton_new->setFont(font2);

        gridLayout_2->addWidget(pushButton_new, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 1, 1, 1);


        retranslateUi(NewLocalProject);

        QMetaObject::connectSlotsByName(NewLocalProject);
    } // setupUi

    void retranslateUi(QDialog *NewLocalProject)
    {
        NewLocalProject->setWindowTitle(QCoreApplication::translate("NewLocalProject", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("NewLocalProject", "\346\226\260\345\273\272\346\234\254\345\234\260\351\241\271\347\233\256", nullptr));
        label_2->setText(QCoreApplication::translate("NewLocalProject", "\351\241\271\347\233\256\345\220\215\347\247\260\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("NewLocalProject", "\351\241\271\347\233\256\344\275\215\347\275\256\357\274\232", nullptr));
        pushButton_select->setText(QCoreApplication::translate("NewLocalProject", "\351\200\211\346\213\251", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("NewLocalProject", "\345\217\226\346\266\210", nullptr));
        pushButton_new->setText(QCoreApplication::translate("NewLocalProject", "\346\226\260\345\273\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewLocalProject: public Ui_NewLocalProject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWLOCALPROJECT_H
