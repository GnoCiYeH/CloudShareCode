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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include "codedocedit.h"

QT_BEGIN_NAMESPACE

class Ui_CodeEdit
{
public:
    QHBoxLayout *horizontalLayout;
    CodeDocEdit *textEdit;

    void setupUi(QWidget *CodeEdit)
    {
        if (CodeEdit->objectName().isEmpty())
            CodeEdit->setObjectName("CodeEdit");
        CodeEdit->resize(580, 500);
        horizontalLayout = new QHBoxLayout(CodeEdit);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        textEdit = new CodeDocEdit(CodeEdit);
        textEdit->setObjectName("textEdit");

        horizontalLayout->addWidget(textEdit);


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
