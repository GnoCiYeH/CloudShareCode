#include "codeedit.h"
#include "ui_codeedit.h"
#include<QKeyEvent>

CodeEdit::CodeEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CodeEdit)
{
    ui->setupUi(this);
}

CodeEdit::~CodeEdit()
{
    delete ui;
}

bool CodeEdit::eventFilter(QObject *obj, QEvent *e)
{
    if(obj==ui->textEdit&&e->type()==QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent *>(e);
        int keyVal = keyEvent->key();
        if(keyVal==Qt::Key_Space)
        {
            format();
        }
        else if((keyVal>=Qt::Key_0&&keyVal<=Qt::Key_9)||(keyVal>=Qt::Key_A&&keyVal<=Qt::Key_Z)){

        }
    }
}

