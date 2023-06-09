#ifndef CODEEDIT_H
#define CODEEDIT_H

#include "ui_codeedit.h"
#include <QWidget>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QThread>
#include <QMutex>
#include <QTcpSocket>
#include <QObject>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QApplication>
#include <vector>
#include <string>
#include <cmath>
#include <QHash>
#include "InfoType.h"
#include "useredittip.h"
using std::min;
using std::string;
using std::vector;

namespace Ui
{
    class CodeEdit;
}
class HighLighter;

static QStringList* associateList;

class CodeEdit : public QWidget
{
    Q_OBJECT

public:
    friend class EditWorkThread;
    friend class HighLighter;
    friend class AssociateListWidget;

public:
    explicit CodeEdit(std::shared_ptr<FileInfo> fileptr = std::shared_ptr<FileInfo>(new FileInfo()), QWidget *parent = nullptr);
    ~CodeEdit() override;

    void gotoline(int lineNum);

    void initLastBlock()
    {
        lastBlock = ui->textEdit->blockCount();
    }

    void highlightError(const QString &error);

    void addText(const QString str);

    std::shared_ptr<FileInfo> getFile() { return file; }

    void changeText(int lineNum, int charRemoved,int posInBlock, QString userId, QString data);

    int tcnum = 0;
    int ctnum = 0;
    QString getText();

signals:
    void deleteInfo(int, int);

public slots:
    void docChange(int,int,int);
protected:
    void resizeEvent(QResizeEvent *event)override;
private:
    Ui::CodeEdit *ui;

    int lastBlock = 0;
    QString buffer;
    QTextDocument *document;
    HighLighter *highLighter;

    bool isChanged = false;

    std::shared_ptr<FileInfo> file;

    QMutex mutex;

    QHash<QString, UserEditTip *> userWidget;
};

class HighLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    friend class CodeEdit;
    HighLighter(CodeEdit *edit, QTextDocument *text = nullptr); // 构造函数需要先传一个QTextDocument对象给父类，因为要先构造父�?
    //void highlightError(const QString &text);

protected:
    void highlightBlock(const QString &text) override; // 重写父类QSyntaxHighlighter的highlightBlock函数，使多行注释高亮

private:
    struct HighLighterRule
    { // 封装高亮规则为结构体
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighLighterRule> highlighterrules; // 规则集合，允许有多条高亮规则

    QTextCharFormat keyword_format;            // 关键字高亮格�?
    QTextCharFormat class_format;              // Qt类高亮格�?
    QTextCharFormat singleLine_comment_format; // 单行注释高亮格式
    QTextCharFormat multiLine_comment_format;  // 多行注释高亮格式
    QTextCharFormat quotation_format;          // 引号高亮格式
    QTextCharFormat function_format;           // 函数高亮格式
    QTextCharFormat headfile_format;           // 头文件高亮格�?
    QTextCharFormat cincout_format;            // 输入输出高亮格式
    QTextCharFormat branch_format;             // 分支高亮格式
    // 用于匹配注释
    QRegularExpression comment_start;
    QRegularExpression comment_end;

    CodeEdit *edit;
};
#endif // CODEEDIT_H
