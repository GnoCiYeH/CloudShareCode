#include "codeedit.h"
#include "ui_codeedit.h"
#include<QKeyEvent>
#include<QDebug>
#include<QTimer>

QStringList HighLighter::keyWords = {
    "\\bint\\b","\\bdouble\\b","\\bchar\\b","\\bstring\\b"
};
CodeEdit::CodeEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CodeEdit)
{
    ui->setupUi(this);

    document = ui->textEdit->document();
    ui->textEdit->setFontFamily("Consolas");
    HighLighter* highLighter = new HighLighter(document);

    connect(document,SIGNAL(contentsChange(int, int, int)),this,SLOT(docChange(int,int,int)));

    thread = new EditWorkThread(this);
    thread->start();
}

CodeEdit::~CodeEdit()
{
    delete ui;
    thread->deleteLater();
    delete thread;
}

void CodeEdit::docChange(int p, int charsRemoved, int charsAdded)
{
    int position = ui->textEdit->textCursor().position()-(charsAdded-charsRemoved);
    qDebug()<<position<<" "<<charsRemoved<<" "<<charsAdded;
    if(position==lastPosition);
    if(charsRemoved>0)
    {
        if(charsAdded==0)
            emit deleteInfo(position,charsRemoved);
        else
        {
            for (int i = position;i<charsAdded+position-charsRemoved;i++) {
                buffer.append(document->characterAt(i));
            }
        }
    }
    else{
        for (int i = position;i<charsAdded+position;i++) {
            buffer.append(document->characterAt(i));
        }
    }
}

EditWorkThread::EditWorkThread(CodeEdit* codeE)
{
    this->codeEdit = codeE;
}

void EditWorkThread::deleteInfo(int position,int charsRemoved)
{
    qDebug()<<"在"<<position<<"删除"<<charsRemoved<<"个字符";
}

void EditWorkThread::run()
{
    connect(codeEdit,SIGNAL(deleteInfo(int,int)),this,SLOT(deleteInfo(int,int)),Qt::DirectConnection);
    QTimer timer;
    connect(&timer,&QTimer::timeout,this,[=](){
        if(!codeEdit->buffer.isEmpty())
        {
            qDebug()<<"buffer:"<<codeEdit->buffer;
            codeEdit->buffer = "";
        }
    },Qt::DirectConnection);
    timer.start(500);
    this->exec();
}

HighLighter::HighLighter(QTextDocument* parent):QSyntaxHighlighter (parent)
{
    HighLightRule rule;
    keyWordFormat.setForeground(QColor(127, 255, 212));
    keyWordFormat.setFontWeight(QFont::Bold);
    for(auto i : keyWords)
    {
        rule.exp = QRegularExpression(i);
        rule.format = keyWordFormat;
        rules.append(rule);
    }
}

void HighLighter::highlightBlock(const QString &text)
{
    for(auto i : rules)
    {
        QRegularExpressionMatchIterator it = i.exp.globalMatch(text);
        while(it.hasNext())
        {
            QRegularExpressionMatch match = it.next();
            setFormat(match.capturedStart(),match.capturedLength(),i.format);
        }
    }
}

