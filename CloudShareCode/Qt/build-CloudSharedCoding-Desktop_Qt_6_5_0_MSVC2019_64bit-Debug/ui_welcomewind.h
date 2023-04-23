/********************************************************************************
** Form generated from reading UI file 'welcomewind.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOMEWIND_H
#define UI_WELCOMEWIND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WelcomeWind
{
public:
    QGridLayout *gridLayout_2;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton;
    QPushButton *pushButton_4;
    QListWidget *listWidget;
    QLabel *label_2;
    QLabel *label;

    void setupUi(QWidget *WelcomeWind)
    {
        if (WelcomeWind->objectName().isEmpty())
            WelcomeWind->setObjectName("WelcomeWind");
        WelcomeWind->resize(1021, 619);
        gridLayout_2 = new QGridLayout(WelcomeWind);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(WelcomeWind);
        widget->setObjectName("widget");
        QFont font;
        font.setFamilies({QString::fromUtf8("Matura MT Script Capitals")});
        font.setPointSize(20);
        widget->setFont(font);
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Microsoft JhengHei UI")});
        font1.setPointSize(16);
        font1.setBold(true);
        label_3->setFont(font1);

        gridLayout->addWidget(label_3, 3, 0, 2, 2);

        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_3 = new QPushButton(widget_2);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setMinimumSize(QSize(200, 50));
        pushButton_3->setMaximumSize(QSize(200, 50));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Microsoft JhengHei UI")});
        font2.setPointSize(20);
        font2.setBold(true);
        pushButton_3->setFont(font2);

        verticalLayout->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(widget_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setMinimumSize(QSize(200, 50));
        pushButton_2->setMaximumSize(QSize(200, 50));
        pushButton_2->setFont(font2);

        verticalLayout->addWidget(pushButton_2);

        verticalSpacer_2 = new QSpacerItem(20, 400, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        pushButton = new QPushButton(widget_2);
        pushButton->setObjectName("pushButton");
        pushButton->setMinimumSize(QSize(200, 50));
        pushButton->setMaximumSize(QSize(200, 50));
        pushButton->setFont(font2);
        pushButton->setIconSize(QSize(16, 16));

        verticalLayout->addWidget(pushButton);

        pushButton_4 = new QPushButton(widget_2);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setMinimumSize(QSize(200, 50));
        pushButton_4->setMaximumSize(QSize(200, 50));
        pushButton_4->setFont(font2);
        pushButton_4->setIconSize(QSize(16, 16));

        verticalLayout->addWidget(pushButton_4);


        gridLayout->addWidget(widget_2, 5, 0, 3, 1);

        listWidget = new QListWidget(widget);
        listWidget->setObjectName("listWidget");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);

        gridLayout->addWidget(listWidget, 5, 1, 3, 1);

        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Microsoft JhengHei UI")});
        font3.setPointSize(20);
        label_2->setFont(font3);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label = new QLabel(widget);
        label->setObjectName("label");
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Matura MT Script Capitals")});
        font4.setPointSize(28);
        label->setFont(font4);

        gridLayout->addWidget(label, 1, 0, 1, 2);


        gridLayout_2->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(WelcomeWind);

        QMetaObject::connectSlotsByName(WelcomeWind);
    } // setupUi

    void retranslateUi(QWidget *WelcomeWind)
    {
        WelcomeWind->setWindowTitle(QCoreApplication::translate("WelcomeWind", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("WelcomeWind", "                                            \346\234\200\350\277\221\350\256\277\351\227\256", nullptr));
        pushButton_3->setText(QCoreApplication::translate("WelcomeWind", "\351\241\271\347\233\256", nullptr));
        pushButton_2->setText(QCoreApplication::translate("WelcomeWind", "\346\226\207\344\273\266", nullptr));
        pushButton->setText(QCoreApplication::translate("WelcomeWind", "\347\231\273\345\275\225", nullptr));
        pushButton_4->setText(QCoreApplication::translate("WelcomeWind", "\346\263\250\345\206\214", nullptr));
        label_2->setText(QCoreApplication::translate("WelcomeWind", "   Welcome to", nullptr));
        label->setText(QCoreApplication::translate("WelcomeWind", "        CloudSharedCoding", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WelcomeWind: public Ui_WelcomeWind {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOMEWIND_H
