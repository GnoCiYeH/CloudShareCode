/********************************************************************************
** Form generated from reading UI file 'privilegemanager.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVILEGEMANAGER_H
#define UI_PRIVILEGEMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrivilegeManager
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_pro_name;
    QLabel *label;
    QLabel *label_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label_people_count;
    QListWidget *listWidget;

    void setupUi(QWidget *PrivilegeManager)
    {
        if (PrivilegeManager->objectName().isEmpty())
            PrivilegeManager->setObjectName("PrivilegeManager");
        PrivilegeManager->resize(807, 468);
        gridLayout = new QGridLayout(PrivilegeManager);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        label_pro_name = new QLabel(PrivilegeManager);
        label_pro_name->setObjectName("label_pro_name");

        gridLayout->addWidget(label_pro_name, 0, 1, 1, 1);

        label = new QLabel(PrivilegeManager);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(PrivilegeManager);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        widget = new QWidget(PrivilegeManager);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 9);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout->addWidget(pushButton_2);


        gridLayout->addWidget(widget, 3, 0, 1, 3);

        label_people_count = new QLabel(PrivilegeManager);
        label_people_count->setObjectName("label_people_count");

        gridLayout->addWidget(label_people_count, 1, 1, 1, 1);

        listWidget = new QListWidget(PrivilegeManager);
        listWidget->setObjectName("listWidget");

        gridLayout->addWidget(listWidget, 2, 0, 1, 3);


        retranslateUi(PrivilegeManager);

        QMetaObject::connectSlotsByName(PrivilegeManager);
    } // setupUi

    void retranslateUi(QWidget *PrivilegeManager)
    {
        PrivilegeManager->setWindowTitle(QCoreApplication::translate("PrivilegeManager", "Form", nullptr));
        label_pro_name->setText(QCoreApplication::translate("PrivilegeManager", "TextLabel", nullptr));
        label->setText(QCoreApplication::translate("PrivilegeManager", "\351\241\271\347\233\256", nullptr));
        label_3->setText(QCoreApplication::translate("PrivilegeManager", "\351\241\271\347\233\256\345\275\223\345\211\215\345\217\202\344\270\216\344\272\272\346\225\260", nullptr));
        pushButton->setText(QCoreApplication::translate("PrivilegeManager", "\347\241\256\345\256\232", nullptr));
        pushButton_2->setText(QCoreApplication::translate("PrivilegeManager", "\345\217\226\346\266\210", nullptr));
        label_people_count->setText(QCoreApplication::translate("PrivilegeManager", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrivilegeManager: public Ui_PrivilegeManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVILEGEMANAGER_H
