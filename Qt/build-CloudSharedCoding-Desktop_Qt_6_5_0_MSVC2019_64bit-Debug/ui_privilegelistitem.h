/********************************************************************************
** Form generated from reading UI file 'privilegelistitem.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVILEGELISTITEM_H
#define UI_PRIVILEGELISTITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrivilegeListItem
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *userId;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox;

    void setupUi(QWidget *PrivilegeListItem)
    {
        if (PrivilegeListItem->objectName().isEmpty())
            PrivilegeListItem->setObjectName("PrivilegeListItem");
        PrivilegeListItem->resize(436, 42);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PrivilegeListItem->sizePolicy().hasHeightForWidth());
        PrivilegeListItem->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(PrivilegeListItem);
        horizontalLayout->setObjectName("horizontalLayout");
        userId = new QLabel(PrivilegeListItem);
        userId->setObjectName("userId");

        horizontalLayout->addWidget(userId);

        horizontalSpacer = new QSpacerItem(278, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        comboBox = new QComboBox(PrivilegeListItem);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        horizontalLayout->addWidget(comboBox);


        retranslateUi(PrivilegeListItem);

        QMetaObject::connectSlotsByName(PrivilegeListItem);
    } // setupUi

    void retranslateUi(QWidget *PrivilegeListItem)
    {
        PrivilegeListItem->setWindowTitle(QCoreApplication::translate("PrivilegeListItem", "Form", nullptr));
        userId->setText(QCoreApplication::translate("PrivilegeListItem", "TextLabel", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("PrivilegeListItem", "public", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("PrivilegeListItem", "default", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("PrivilegeListItem", "privite", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("PrivilegeListItem", "protected", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("PrivilegeListItem", "root", nullptr));
        comboBox->setItemText(5, QString());

    } // retranslateUi

};

namespace Ui {
    class PrivilegeListItem: public Ui_PrivilegeListItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVILEGELISTITEM_H
