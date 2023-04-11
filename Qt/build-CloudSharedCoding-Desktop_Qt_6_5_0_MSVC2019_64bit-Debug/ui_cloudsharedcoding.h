/********************************************************************************
** Form generated from reading UI file 'cloudsharedcoding.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOUDSHAREDCODING_H
#define UI_CLOUDSHAREDCODING_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE

class Ui_CloudSharedCoding
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    MainWindow *widget;

    void setupUi(QWidget *CloudSharedCoding)
    {
        if (CloudSharedCoding->objectName().isEmpty())
            CloudSharedCoding->setObjectName("CloudSharedCoding");
        CloudSharedCoding->resize(1439, 784);
        verticalLayout = new QVBoxLayout(CloudSharedCoding);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(CloudSharedCoding);
        widget_2->setObjectName("widget_2");
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget_2);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        toolButton = new QToolButton(widget_2);
        toolButton->setObjectName("toolButton");
        toolButton->setMaximumSize(QSize(16, 16));
        toolButton->setIconSize(QSize(16, 16));

        horizontalLayout->addWidget(toolButton);

        toolButton_2 = new QToolButton(widget_2);
        toolButton_2->setObjectName("toolButton_2");
        toolButton_2->setMaximumSize(QSize(16, 16));

        horizontalLayout->addWidget(toolButton_2);

        toolButton_3 = new QToolButton(widget_2);
        toolButton_3->setObjectName("toolButton_3");
        toolButton_3->setMaximumSize(QSize(16, 16));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/remove-gray@2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/images/remove-red.png"), QSize(), QIcon::Selected, QIcon::On);
        toolButton_3->setIcon(icon);
        toolButton_3->setIconSize(QSize(32, 33));
        toolButton_3->setCheckable(false);

        horizontalLayout->addWidget(toolButton_3);


        verticalLayout->addWidget(widget_2);

        widget = new MainWindow(CloudSharedCoding);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(widget);


        retranslateUi(CloudSharedCoding);

        QMetaObject::connectSlotsByName(CloudSharedCoding);
    } // setupUi

    void retranslateUi(QWidget *CloudSharedCoding)
    {
        CloudSharedCoding->setWindowTitle(QCoreApplication::translate("CloudSharedCoding", "Form", nullptr));
        label->setText(QCoreApplication::translate("CloudSharedCoding", "TextLabel", nullptr));
        toolButton->setText(QCoreApplication::translate("CloudSharedCoding", "...", nullptr));
        toolButton_2->setText(QCoreApplication::translate("CloudSharedCoding", "...", nullptr));
        toolButton_3->setText(QCoreApplication::translate("CloudSharedCoding", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CloudSharedCoding: public Ui_CloudSharedCoding {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOUDSHAREDCODING_H
