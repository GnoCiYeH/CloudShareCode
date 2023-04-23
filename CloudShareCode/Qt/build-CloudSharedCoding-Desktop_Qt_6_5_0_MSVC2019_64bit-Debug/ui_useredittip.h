/********************************************************************************
** Form generated from reading UI file 'useredittip.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USEREDITTIP_H
#define UI_USEREDITTIP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserEditTip
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;

    void setupUi(QWidget *UserEditTip)
    {
        if (UserEditTip->objectName().isEmpty())
            UserEditTip->setObjectName("UserEditTip");
        UserEditTip->resize(53, 16);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(UserEditTip->sizePolicy().hasHeightForWidth());
        UserEditTip->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(UserEditTip);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(5, 0, 0, 0);
        label = new QLabel(UserEditTip);
        label->setObjectName("label");
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(label);


        retranslateUi(UserEditTip);

        QMetaObject::connectSlotsByName(UserEditTip);
    } // setupUi

    void retranslateUi(QWidget *UserEditTip)
    {
        UserEditTip->setWindowTitle(QCoreApplication::translate("UserEditTip", "Form", nullptr));
        label->setText(QCoreApplication::translate("UserEditTip", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserEditTip: public Ui_UserEditTip {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USEREDITTIP_H
