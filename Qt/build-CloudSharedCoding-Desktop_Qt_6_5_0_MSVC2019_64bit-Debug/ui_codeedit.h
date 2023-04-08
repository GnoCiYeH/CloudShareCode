/********************************************************************************
** Form generated from reading UI file 'codeedit.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CODEEDIT_H
#define UI_CODEEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CodeEdit
{
public:
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *textEdit;

    void setupUi(QWidget *CodeEdit)
    {
        if (CodeEdit->objectName().isEmpty())
            CodeEdit->setObjectName("CodeEdit");
        CodeEdit->resize(580, 500);
        verticalLayout = new QVBoxLayout(CodeEdit);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        textEdit = new QPlainTextEdit(CodeEdit);
        textEdit->setObjectName("textEdit");

        verticalLayout->addWidget(textEdit);


        retranslateUi(CodeEdit);

        QMetaObject::connectSlotsByName(CodeEdit);
    } // setupUi

    void retranslateUi(QWidget *CodeEdit)
    {
        CodeEdit->setWindowTitle(QCoreApplication::translate("CodeEdit", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CodeEdit: public Ui_CodeEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CODEEDIT_H
