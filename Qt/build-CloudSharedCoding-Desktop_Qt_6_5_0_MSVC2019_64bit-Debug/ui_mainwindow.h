/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "welcomewind.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCloud_project;
    QAction *actionOpenLocal_project;
    QAction *actionNew_cloud_project;
    QAction *new_file_action;
    QAction *actionClose_project_2;
    QAction *actionClose;
    QAction *actioncopyAction;
    QAction *actionpasteAction;
    QAction *actioncutAction;
    QAction *actionCheckHelper;
    QAction *actionAbout;
    QAction *actionReportProblem;
    QAction *actionSuggestion;
    QAction *actionProblem;
    QAction *actionASCLL;
    QAction *actionUTF8;
    QAction *actionSwitching;
    QAction *actionSave;
    QAction *actionNewFileDirection;
    QAction *actionNewFile;
    QAction *actionNew_local_project;
    QAction *actionSetting;
    QAction *actionAdd_Cloud_File;
    QAction *actionAdd_Local_File;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    WelcomeWind *widget;
    QWidget *tab_2;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menuNew_project;
    QMenu *menuOpen_project;
    QMenu *menu_2;
    QMenu *Tool;
    QMenu *Setting;
    QMenu *menu_4;
    QMenu *menuSendFeedback;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *treeWidget;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1390, 785);
        actionCloud_project = new QAction(MainWindow);
        actionCloud_project->setObjectName("actionCloud_project");
        actionOpenLocal_project = new QAction(MainWindow);
        actionOpenLocal_project->setObjectName("actionOpenLocal_project");
        actionNew_cloud_project = new QAction(MainWindow);
        actionNew_cloud_project->setObjectName("actionNew_cloud_project");
        new_file_action = new QAction(MainWindow);
        new_file_action->setObjectName("new_file_action");
        actionClose_project_2 = new QAction(MainWindow);
        actionClose_project_2->setObjectName("actionClose_project_2");
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName("actionClose");
        actioncopyAction = new QAction(MainWindow);
        actioncopyAction->setObjectName("actioncopyAction");
        actionpasteAction = new QAction(MainWindow);
        actionpasteAction->setObjectName("actionpasteAction");
        actioncutAction = new QAction(MainWindow);
        actioncutAction->setObjectName("actioncutAction");
        actionCheckHelper = new QAction(MainWindow);
        actionCheckHelper->setObjectName("actionCheckHelper");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionReportProblem = new QAction(MainWindow);
        actionReportProblem->setObjectName("actionReportProblem");
        actionSuggestion = new QAction(MainWindow);
        actionSuggestion->setObjectName("actionSuggestion");
        actionProblem = new QAction(MainWindow);
        actionProblem->setObjectName("actionProblem");
        actionASCLL = new QAction(MainWindow);
        actionASCLL->setObjectName("actionASCLL");
        actionUTF8 = new QAction(MainWindow);
        actionUTF8->setObjectName("actionUTF8");
        actionSwitching = new QAction(MainWindow);
        actionSwitching->setObjectName("actionSwitching");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionNewFileDirection = new QAction(MainWindow);
        actionNewFileDirection->setObjectName("actionNewFileDirection");
        actionNewFile = new QAction(MainWindow);
        actionNewFile->setObjectName("actionNewFile");
        actionNew_local_project = new QAction(MainWindow);
        actionNew_local_project->setObjectName("actionNew_local_project");
        actionSetting = new QAction(MainWindow);
        actionSetting->setObjectName("actionSetting");
        actionAdd_Cloud_File = new QAction(MainWindow);
        actionAdd_Cloud_File->setObjectName("actionAdd_Cloud_File");
        actionAdd_Local_File = new QAction(MainWindow);
        actionAdd_Local_File->setObjectName("actionAdd_Local_File");
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName("widget_2");
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(widget_2);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(true);
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new WelcomeWind(tab);
        widget->setObjectName("widget");

        verticalLayout->addWidget(widget);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);


        gridLayout->addWidget(widget_2, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1390, 17));
        menu = new QMenu(menuBar);
        menu->setObjectName("menu");
        menuNew_project = new QMenu(menu);
        menuNew_project->setObjectName("menuNew_project");
        menuOpen_project = new QMenu(menu);
        menuOpen_project->setObjectName("menuOpen_project");
        menu_2 = new QMenu(menu);
        menu_2->setObjectName("menu_2");
        Tool = new QMenu(menuBar);
        Tool->setObjectName("Tool");
        Setting = new QMenu(Tool);
        Setting->setObjectName("Setting");
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName("menu_4");
        menuSendFeedback = new QMenu(menu_4);
        menuSendFeedback->setObjectName("menuSendFeedback");
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName("dockWidget");
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName("dockWidgetContents");
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(dockWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName("treeWidget");

        verticalLayout_3->addWidget(treeWidget);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(Tool->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menu->addAction(menuOpen_project->menuAction());
        menu->addAction(menuNew_project->menuAction());
        menu->addSeparator();
        menu->addAction(menu_2->menuAction());
        menu->addAction(new_file_action);
        menu->addSeparator();
        menu->addAction(actionClose_project_2);
        menu->addAction(actionClose);
        menu->addSeparator();
        menu->addAction(actionSave);
        menuNew_project->addAction(actionNew_cloud_project);
        menuNew_project->addAction(actionNew_local_project);
        menuOpen_project->addAction(actionCloud_project);
        menuOpen_project->addAction(actionOpenLocal_project);
        menu_2->addSeparator();
        menu_2->addAction(actionAdd_Cloud_File);
        menu_2->addAction(actionAdd_Local_File);
        Tool->addAction(Setting->menuAction());
        Setting->addAction(actionSwitching);
        Setting->addAction(actionSetting);
        menu_4->addAction(actionCheckHelper);
        menu_4->addAction(actionAbout);
        menu_4->addAction(menuSendFeedback->menuAction());
        menuSendFeedback->addAction(actionSuggestion);
        menuSendFeedback->addAction(actionProblem);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionCloud_project->setText(QCoreApplication::translate("MainWindow", "Cloud project", nullptr));
        actionOpenLocal_project->setText(QCoreApplication::translate("MainWindow", "Local project", nullptr));
        actionNew_cloud_project->setText(QCoreApplication::translate("MainWindow", "New cloud project", nullptr));
        new_file_action->setText(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272\346\226\207\344\273\266", nullptr));
        actionClose_project_2->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255\351\241\271\347\233\256", nullptr));
        actionClose->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
        actioncopyAction->setText(QCoreApplication::translate("MainWindow", "\345\244\215\345\210\266", nullptr));
        actionpasteAction->setText(QCoreApplication::translate("MainWindow", "\347\262\230\350\264\264", nullptr));
        actioncutAction->setText(QCoreApplication::translate("MainWindow", "\345\211\252\345\210\207", nullptr));
        actionCheckHelper->setText(QCoreApplication::translate("MainWindow", "\346\237\245\347\234\213\345\270\256\345\212\251", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216CloudShareCode", nullptr));
        actionReportProblem->setText(QCoreApplication::translate("MainWindow", "\346\212\245\345\221\212\351\227\256\351\242\230", nullptr));
        actionSuggestion->setText(QCoreApplication::translate("MainWindow", "\345\273\272\350\256\256\345\212\237\350\203\275", nullptr));
        actionProblem->setText(QCoreApplication::translate("MainWindow", "\345\217\215\351\246\210\351\227\256\351\242\230", nullptr));
        actionASCLL->setText(QCoreApplication::translate("MainWindow", "ASCII", nullptr));
        actionUTF8->setText(QCoreApplication::translate("MainWindow", "UTF-8", nullptr));
        actionSwitching->setText(QCoreApplication::translate("MainWindow", "\345\210\207\346\215\242\347\274\226\347\240\201\346\226\271\345\274\217", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "\346\234\254\345\234\260\344\277\235\345\255\230", nullptr));
        actionNewFileDirection->setText(QCoreApplication::translate("MainWindow", "NewFileDirection", nullptr));
        actionNewFile->setText(QCoreApplication::translate("MainWindow", "NewFile", nullptr));
        actionNew_local_project->setText(QCoreApplication::translate("MainWindow", "New local project", nullptr));
        actionSetting->setText(QCoreApplication::translate("MainWindow", "Setting", nullptr));
        actionAdd_Cloud_File->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\344\272\221\346\226\207\344\273\266", nullptr));
        actionAdd_Local_File->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\234\254\345\234\260\346\226\207\344\273\266", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Welcome", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menuNew_project->setTitle(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272\351\241\271\347\233\256", nullptr));
        menuOpen_project->setTitle(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\351\241\271\347\233\256", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\226\207\344\273\266", nullptr));
        Tool->setTitle(QCoreApplication::translate("MainWindow", "\345\267\245\345\205\267", nullptr));
        Setting->setTitle(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        menu_4->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
        menuSendFeedback->setTitle(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\345\217\215\351\246\210", nullptr));
        dockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "\351\241\271\347\233\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
