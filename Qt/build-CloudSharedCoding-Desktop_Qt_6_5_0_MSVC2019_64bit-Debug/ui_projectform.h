/********************************************************************************
** Form generated from reading UI file 'projectform.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTFORM_H
#define UI_PROJECTFORM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectForm
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_3;
    QVBoxLayout *verticalLayout_6;
    QListWidget *listWidget;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_5;
    QScrollArea *scrollArea_4;
    QWidget *scrollAreaWidgetContents_4;
    QVBoxLayout *verticalLayout_7;
    QListWidget *listWidget_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_4;

    void setupUi(QWidget *ProjectForm)
    {
        if (ProjectForm->objectName().isEmpty())
            ProjectForm->setObjectName("ProjectForm");
        ProjectForm->resize(815, 530);
        verticalLayout = new QVBoxLayout(ProjectForm);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, -1, 0, 0);
        widget = new QWidget(ProjectForm);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        toolButton = new QToolButton(widget);
        toolButton->setObjectName("toolButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/filebrowser/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);
        toolButton->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(toolButton);

        toolButton_2 = new QToolButton(widget);
        toolButton_2->setObjectName("toolButton_2");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/files/file_folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_2->setIcon(icon1);
        toolButton_2->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(toolButton_2);

        toolButton_3 = new QToolButton(widget);
        toolButton_3->setObjectName("toolButton_3");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/svg/cloud.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_3->setIcon(icon2);
        toolButton_3->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(toolButton_3);


        verticalLayout->addWidget(widget);

        tabWidget = new QTabWidget(ProjectForm);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 791, 439));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_4->setObjectName("verticalLayout_4");
        scrollArea_3 = new QScrollArea(scrollAreaWidgetContents);
        scrollArea_3->setObjectName("scrollArea_3");
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName("scrollAreaWidgetContents_3");
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 771, 419));
        verticalLayout_6 = new QVBoxLayout(scrollAreaWidgetContents_3);
        verticalLayout_6->setObjectName("verticalLayout_6");
        listWidget = new QListWidget(scrollAreaWidgetContents_3);
        listWidget->setObjectName("listWidget");

        verticalLayout_6->addWidget(listWidget);

        widget_2 = new QWidget(scrollAreaWidgetContents_3);
        widget_2->setObjectName("widget_2");
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        pushButton = new QPushButton(widget_2);
        pushButton->setObjectName("pushButton");

        horizontalLayout_2->addWidget(pushButton);

        pushButton_3 = new QPushButton(widget_2);
        pushButton_3->setObjectName("pushButton_3");

        horizontalLayout_2->addWidget(pushButton_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(widget_2);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout_6->addWidget(widget_2);

        scrollArea_3->setWidget(scrollAreaWidgetContents_3);

        verticalLayout_4->addWidget(scrollArea_3);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName("verticalLayout_3");
        scrollArea_2 = new QScrollArea(tab_2);
        scrollArea_2->setObjectName("scrollArea_2");
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName("scrollAreaWidgetContents_2");
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 791, 439));
        verticalLayout_5 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_5->setObjectName("verticalLayout_5");
        scrollArea_4 = new QScrollArea(scrollAreaWidgetContents_2);
        scrollArea_4->setObjectName("scrollArea_4");
        scrollArea_4->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName("scrollAreaWidgetContents_4");
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 771, 419));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents_4);
        verticalLayout_7->setObjectName("verticalLayout_7");
        listWidget_2 = new QListWidget(scrollAreaWidgetContents_4);
        listWidget_2->setObjectName("listWidget_2");

        verticalLayout_7->addWidget(listWidget_2);

        widget_3 = new QWidget(scrollAreaWidgetContents_4);
        widget_3->setObjectName("widget_3");
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        pushButton_5 = new QPushButton(widget_3);
        pushButton_5->setObjectName("pushButton_5");

        horizontalLayout_3->addWidget(pushButton_5);

        pushButton_6 = new QPushButton(widget_3);
        pushButton_6->setObjectName("pushButton_6");

        horizontalLayout_3->addWidget(pushButton_6);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        pushButton_4 = new QPushButton(widget_3);
        pushButton_4->setObjectName("pushButton_4");

        horizontalLayout_3->addWidget(pushButton_4);


        verticalLayout_7->addWidget(widget_3);

        scrollArea_4->setWidget(scrollAreaWidgetContents_4);

        verticalLayout_5->addWidget(scrollArea_4);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_3->addWidget(scrollArea_2);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(ProjectForm);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ProjectForm);
    } // setupUi

    void retranslateUi(QWidget *ProjectForm)
    {
        ProjectForm->setWindowTitle(QCoreApplication::translate("ProjectForm", "Form", nullptr));
        label->setText(QCoreApplication::translate("ProjectForm", "    \344\272\221\347\253\257\351\241\271\347\233\256", nullptr));
#if QT_CONFIG(whatsthis)
        toolButton->setWhatsThis(QCoreApplication::translate("ProjectForm", "\346\226\260\345\273\272\351\241\271\347\233\256", nullptr));
#endif // QT_CONFIG(whatsthis)
        toolButton->setText(QCoreApplication::translate("ProjectForm", "...", nullptr));
#if QT_CONFIG(whatsthis)
        toolButton_2->setWhatsThis(QCoreApplication::translate("ProjectForm", "\345\257\274\345\205\245\345\210\260\344\272\221", nullptr));
#endif // QT_CONFIG(whatsthis)
        toolButton_2->setText(QCoreApplication::translate("ProjectForm", "...", nullptr));
#if QT_CONFIG(whatsthis)
        toolButton_3->setWhatsThis(QCoreApplication::translate("ProjectForm", "\345\217\202\344\270\216\344\272\221\347\253\257\351\241\271\347\233\256", nullptr));
#endif // QT_CONFIG(whatsthis)
        toolButton_3->setText(QCoreApplication::translate("ProjectForm", "...", nullptr));
        pushButton->setText(QCoreApplication::translate("ProjectForm", "\345\210\206\344\272\253\351\241\271\347\233\256", nullptr));
        pushButton_3->setText(QCoreApplication::translate("ProjectForm", "\345\210\240\351\231\244\351\241\271\347\233\256", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ProjectForm", "\346\211\223\345\274\200\351\241\271\347\233\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("ProjectForm", "\346\210\221\347\232\204\351\241\271\347\233\256", nullptr));
        pushButton_5->setText(QCoreApplication::translate("ProjectForm", "\351\241\271\347\233\256\347\273\223\346\236\204", nullptr));
        pushButton_6->setText(QCoreApplication::translate("ProjectForm", "\351\200\200\345\207\272\351\241\271\347\233\256", nullptr));
        pushButton_4->setText(QCoreApplication::translate("ProjectForm", "\346\211\223\345\274\200\351\241\271\347\233\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("ProjectForm", "\346\210\221\347\232\204\345\217\202\344\270\216", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjectForm: public Ui_ProjectForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTFORM_H
