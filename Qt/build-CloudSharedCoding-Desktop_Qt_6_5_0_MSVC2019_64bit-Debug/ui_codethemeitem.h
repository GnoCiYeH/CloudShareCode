/********************************************************************************
** Form generated from reading UI file 'codethemeitem.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CODETHEMEITEM_H
#define UI_CODETHEMEITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CodeThemeItem
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_color;
    QPushButton *pushButton;

    void setupUi(QWidget *CodeThemeItem)
    {
        if (CodeThemeItem->objectName().isEmpty())
            CodeThemeItem->setObjectName("CodeThemeItem");
        CodeThemeItem->resize(352, 24);
        horizontalLayout = new QHBoxLayout(CodeThemeItem);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(CodeThemeItem);
        label->setObjectName("label");
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        widget = new QWidget(CodeThemeItem);
        widget->setObjectName("widget");
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_color = new QLabel(widget);
        label_color->setObjectName("label_color");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_color->sizePolicy().hasHeightForWidth());
        label_color->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(label_color);


        horizontalLayout->addWidget(widget);

        pushButton = new QPushButton(CodeThemeItem);
        pushButton->setObjectName("pushButton");
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(pushButton);


        retranslateUi(CodeThemeItem);

        QMetaObject::connectSlotsByName(CodeThemeItem);
    } // setupUi

    void retranslateUi(QWidget *CodeThemeItem)
    {
        CodeThemeItem->setWindowTitle(QCoreApplication::translate("CodeThemeItem", "Form", nullptr));
        label->setText(QCoreApplication::translate("CodeThemeItem", "TextLabel", nullptr));
        label_color->setText(QCoreApplication::translate("CodeThemeItem", "TextLabel", nullptr));
        pushButton->setText(QCoreApplication::translate("CodeThemeItem", "\351\200\211\346\213\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CodeThemeItem: public Ui_CodeThemeItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CODETHEMEITEM_H
