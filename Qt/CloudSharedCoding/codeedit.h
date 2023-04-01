#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include<QTextEdit>
#include<QSyntaxHighlighter>
#include<QTextCharFormat>
#include<QRegularExpression>
#include<QThread>
#include<QMutex>

namespace Ui {
class CodeEdit;
}

class EditWorkThread;
class CodeEdit : public QWidget
{
    Q_OBJECT

public: friend class EditWorkThread;
public:
    explicit CodeEdit(QWidget *parent = nullptr);
    ~CodeEdit() override;

signals:
    void deleteInfo(int,int);

public slots:
    void docChange(int,int,int);

private:
    Ui::CodeEdit *ui;

    int lastPosition;
    QString buffer;
    QTextDocument * document;
    EditWorkThread* thread;

    QMutex mutex;
};

class EditWorkThread : public QThread
{
    Q_OBJECT

public:
    explicit EditWorkThread(CodeEdit*);
    ~EditWorkThread(){}

private slots:
    void deleteInfo(int,int);

private:
    CodeEdit * codeEdit;
protected:
    void run() override;
};

class HighLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    HighLighter(QTextDocument*text=nullptr);//构造函数需要先传一个QTextDocument对象给父类，因为要先构造父类

protected:
    void highlightBlock(const QString &text) override; //重写父类QSyntaxHighlighter的highlightBlock函数，使多行注释高亮

private:
    struct HighLighterRule{//封装高亮规则为结构体
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighLighterRule>highlighterrules;//规则集合，允许有多条高亮规则

    QTextCharFormat keyword_format;//关键字高亮格式
    QTextCharFormat class_format;//Qt类高亮格式
    QTextCharFormat singleLine_comment_format;//单行注释高亮格式
    QTextCharFormat multiLine_comment_format;//多行注释高亮格式
    QTextCharFormat quotation_format;//引号高亮格式
    QTextCharFormat function_format;//函数高亮格式
    QTextCharFormat headfile_format;//头文件高亮格式
    QTextCharFormat cincout_format;//输入输出高亮格式
    QTextCharFormat branch_format;//分支高亮格式

    //用于匹配注释
    QRegularExpression comment_start;
    QRegularExpression comment_end;
};

#endif // CODEEDIT_H
