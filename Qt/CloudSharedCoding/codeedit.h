#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include<QTextEdit>
#include<QSyntaxHighlighter>
#include<QTextCharFormat>
#include<QRegularExpression>

namespace Ui {
class CodeEdit;
}

class CodeEdit : public QWidget
{
    Q_OBJECT

public:
    explicit CodeEdit(QWidget *parent = nullptr);
    ~CodeEdit();

private:
    Ui::CodeEdit *ui;

    QString buffer;
    QTextDocument * document;
};

class HighLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit HighLighter(QTextDocument* parent = nullptr);

private:
    static QStringList keyWords;

    struct HighLightRule{
        QTextCharFormat format;
        QRegularExpression exp;
    };
    QVector<HighLightRule> rules;
    QTextCharFormat keyWordFormat;

protected:
    void highlightBlock(const QString &text) override;
};

#endif // CODEEDIT_H
