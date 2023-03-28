#include "codeedit.h"
#include "ui_codeedit.h"
#include<QKeyEvent>

CodeEdit::CodeEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CodeEdit)
{
    ui->setupUi(this);

    ui->textEdit->setFontFamily("Consolas");
    HighLighter* highLighter = new HighLighter(ui->textEdit->document());
}

CodeEdit::~CodeEdit()
{
    delete ui;
}

HighLighter::HighLighter(QTextDocument* parent):QSyntaxHighlighter (parent)
{
    HighLightRule rule;
    keyWordFormat.setForeground(QColor(127, 255, 212));
    keyWordFormat.setFontWeight(QFont::Bold);
    QStringList keyWords = {"\\bint\\b","\\bdouble\\b","\\bchar\\b","\\bstring\\b"};
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

