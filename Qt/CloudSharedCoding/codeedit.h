#ifndef CODEEDIT_H
#define CODEEDIT_H

#include "ui_codeedit.h"
#include <QWidget>
#include<QTextEdit>
#include<QSyntaxHighlighter>
#include<QTextCharFormat>
#include<QRegularExpression>
#include<QThread>
#include<QMutex>
#include<QTcpSocket>
#include<QObject>
#include<QListWidget>
#include<QPlainTextEdit>
#include<QApplication>
#include<vector>
#include<string>
#include<cmath>
#include<QHash>
#include"InfoType.h"
#include"useredittip.h"
using std::string;
using std::vector;
using std::min;

namespace Ui {
class CodeEdit;
}
class AssociateListWidget;
class HighLighter;
static void setUpAssociateList();
static QStringList associateList;

class CodeEdit : public QWidget
{
    Q_OBJECT

public: friend class EditWorkThread;
    friend class HighLighter;
public:
    explicit CodeEdit(std::shared_ptr<FileInfo>fileptr=nullptr,QWidget *parent = nullptr);
    ~CodeEdit() override;

    void addText(const QString str);
    QString getText();

    std::shared_ptr<FileInfo> getFile(){return file;}

    void changeText(int pos,int charRemoved,QString userId,QString data);

    int tcnum = 0;
    int ctnum = 0;

signals:
    void deleteInfo(int,int);

public slots:
    void docChange(int,int,int);

protected:
    //void keyReleaseEvent(QKeyEvent* event)override;

private:
    Ui::CodeEdit *ui;

    int lastBlock = 0;
    QString buffer;
    QTextDocument * document;
    HighLighter *highLighter;

    bool isChanged = false;

    std::shared_ptr<FileInfo> file;

    QMutex mutex;
    int associateState;//联想状态
    AssociateListWidget *associateWidget;//联想表
    QString getWordCursor();//获取当前光标所在位置的字符串
    int getAssociateWidgetX();

    QHash<QString,UserEditTip*> userWidget;

private slots:
    void showAssociateWidget();//展示联想列表
};

class HighLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    friend class CodeEdit;
    HighLighter(CodeEdit*edit,QTextDocument*text=nullptr);//构造函数需要先传一个QTextDocument对象给父类，因为要先构造父类
    int showMistake();

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

    CodeEdit *edit;
};

enum AssociateState{
    Ignore=0,
    Showing=1,
    Hide=2
};

class AssociateListWidget:public QListWidget{
public:
    AssociateListWidget(QWidget*parent=0);
    static int letterDifference(const string source,const string target);//两个字符串的差异度
    static int strToInt(string str);
private:
    QPlainTextEdit* p;
    QColor backgroundColor;//联想列表背景色
    QColor highlightColor;
};

#endif // CODEEDIT_H
