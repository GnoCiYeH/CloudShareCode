#include "codeedit.h"
#include "ui_codeedit.h"
#include<QKeyEvent>
#include<QDebug>
#include<QTimer>

CodeEdit::CodeEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CodeEdit)
{
    ui->setupUi(this);

    document = ui->textEdit->document();
    ui->textEdit->setFontFamily("Consolas");
    HighLighter* highLighter = new HighLighter(document);

    const int tabstop = 4;
    QFontMetrics m(ui->textEdit->font());
    ui->textEdit->setTabStopDistance(tabstop*m.horizontalAdvance(" "));

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

void CodeEdit::addText(const QString str)
{
    ui->textEdit->insertPlainText(str);
}

void CodeEdit::docChange(int p, int charsRemoved, int charsAdded)
{
    int position = ui->textEdit->textCursor().position()-(charsAdded-charsRemoved);
    if(position<0)position=0;
    qDebug()<<position<<" "<<charsRemoved<<" "<<charsAdded;
    if(charsRemoved>0)
    {
        if(charsAdded==0)
            buffer.append("\b");
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

    socket = new QTcpSocket(this);
    socket->connectToHost("192.168.239.129",9897);
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
            qDebug()<<codeEdit->buffer;
            codeEdit->buffer = "";
        }
    },Qt::DirectConnection);
    timer.start(500);
    this->exec();
}

HighLighter::HighLighter(QTextDocument* text):QSyntaxHighlighter (text)
{
    //制定高亮规则
    HighLighterRule rule;

    //1.添加关键字高亮规则
    keyword_format.setForeground(QColor(118, 238, 198));//设置关键字前景颜色(blue)
    keyword_format.setFontWeight(QFont::Bold);//设置关键字的字体格式(Bold)
    QVector<QString>keyword_pattern={// \b在表示单词字符边界，防止例如intVal也被识别为int导致高亮
      "\\bchar\\b",  "\\bclass\\b","\\bconst\\b","\\bdouble\\b","\\benum\\b","\\bexplicit\\b",
      "\\bfriend\\b","\\binline\\b","\\bint\\b","\\blong\\b","\\bnamespace\\b","\\boperator\\b",
      "\\bprivate\\b","\\bprotected\\b","\\bpublic\\b","\\bshort\\b","\\bsignals\\b","\\bsigned\\b",
      "\\bslots\\b","\\bstatic\\b","\\bstruct\\b","\\btemplate\\b","\\btypedef\\b","\\btypename\\b",
      "\\bunion\\b","\\bunsigned\\b","\\bvirtual\\b","\\bvoid\\b","\\bvolatile\\b","\\bbool\\b",
      "\\busing\\b","vector"
    };//关键字集合
    //遍历关键字集合，通过正则表达式识别字符串。并设定为rule的pattern，代表当前关键字的标识符；再设定rule的格式，最终加入规则集合中
    for(auto& keyword:keyword_pattern){
        rule.pattern=QRegularExpression(keyword);
        rule.format=keyword_format;
        highlighterrules.push_back(rule);
    }//规则集合中存储着keyword_pattern中所有关键字的标识符和格式(蓝色 粗体)

    //2.添加Qt类高亮规则
    class_format.setForeground(Qt::darkCyan);//设置Qt类前景色(darkCyan)
    class_format.setFontWeight(QFont::Bold);//设置Qt类字体格式(Bold)
    QString class_pattern="\\bQ[a-zA-z]+\\b";//Qt类识别格式为两边有分隔符，且以Q开头的所有英文字符串
    rule.pattern=QRegularExpression(class_pattern);
    rule.format=class_format;
    highlighterrules.push_back(rule);

    //3.添加单行注释高亮规则
    singleLine_comment_format.setForeground(QColor(211, 211 ,211));//注释颜色为green
    QString singleLine_comment_pattern="//[^\n]*";//单行注释识别格式为跟在//后，但不包括换行符，且不需要间隔符
    rule.pattern=QRegularExpression(singleLine_comment_pattern);
    rule.format=singleLine_comment_format;
    highlighterrules.push_back(rule);

    //4.添加多行注释高亮规则
    //多行注释的匹配正则表达式
    QString comment_start_pattern="/\\*";//开始位置\*，因为正则表达式的*需要用\*表达，而\*需要字符串用\\*表达
    comment_start=QRegularExpression(comment_start_pattern);
    QString comment_end_pattern="\\*/";//结束位置
    comment_end=QRegularExpression(comment_end_pattern);


    //5.添加引号高亮规则
    quotation_format.setForeground(Qt::cyan);//引号内容颜色(cyan)
    QString quotation_pattern="\".*\"";
    rule.pattern=QRegularExpression(quotation_pattern);
    rule.format=quotation_format;
    highlighterrules.push_back(rule);
    multiLine_comment_format.setForeground(Qt::green);//多行注释颜色为green

    //6.添加函数高亮格式
    function_format.setForeground(QColor(238, 180, 180));//函数字体颜色设置为darkGreen
    function_format.setFontWeight(QFont::Bold);//函数字体格式设置为Bold
    QString function_pattern="\\b[a-zA-Z0-9_]+(?=\\()";//函数名可以是大小写英文字符、数字、下划线，其中，(?=\\()表示后面必须跟着一个左括号，但是这个左括号不会被匹配到
    rule.pattern=QRegularExpression(function_pattern);
    rule.format=function_format;
    highlighterrules.push_back(rule);

    //7.添加分支高亮格式
    branch_format.setForeground(Qt::red);
    branch_format.setFontWeight(QFont::Bold);
    QVector<QString>branch_pattern={
      "if","else","switch","case","while","for"
    };
    rule.format=branch_format;
    for(QString&pattern:branch_pattern){
        rule.pattern=QRegularExpression(pattern);
        highlighterrules.push_back(rule);
    }

    //8.添加输入输出高亮格式
    cincout_format.setForeground(Qt::darkGray);
    cincout_format.setFontWeight(QFont::Bold);
    QVector<QString>cincout_pattern={
      "cin","cout","std","endl","<<",">>"
    };
    rule.format=cincout_format;
    for(auto&pattern :cincout_pattern){
        rule.pattern=QRegularExpression(pattern);
        highlighterrules.push_back(rule);
    }
    //9.添加头文件高亮格式
    //9.1 #开头
    headfile_format.setForeground(Qt::darkGray);
    headfile_format.setFontWeight(QFont::Bold);
    rule.format=headfile_format;
    rule.pattern=QRegularExpression("#.*");
    highlighterrules.push_back(rule);

    //9.2 各头文件
    headfile_format.setForeground(Qt::yellow);
    headfile_format.setFontWeight(QFont::Bold);
    QVector<QString>headfile_pattern={
      "<algorithm>","<bitset>","<cctype>","<cerrno>","<cerrno>","<cerrno>",
      "\\b<?complex>?\\b","<cstdio>","<cstdlib>","\\b<?cstring>?\\b","<ctime>","\\b<?deque>?\\b",
      "\\b<?exception>?\\b","\\b<?fstream>?\\b","\\b<?limits>?\\b","\\b<?list>?\\b","\\b<?map>?\\b","<iomanip>","<ios>",
      "<iosfwd>","<iostream>","\\b<?istream>?\\b","\\b<?ostream>?\\b","\\b<?queue>?\\b","\\b<?set>?\\b",
      "\\b<?sstream>?\\b","\\b<?stack>?\\b","<stdexcept>","<streambuf>","<string>","<utility>",
      "\\b<?vector>?\\b","\\b<?cwchar>?\\b","\\b<?cwctype>?\\b"
    };
    rule.format=headfile_format;
    for(auto& pattern:headfile_pattern){
        rule.pattern=QRegularExpression(pattern);
        highlighterrules.push_back(rule);
    }
}

void HighLighter::highlightBlock(const QString &text){//应用高亮规则
    foreach(const HighLighterRule& rule,highlighterrules){
        QRegularExpressionMatchIterator matchIterator=rule.pattern.globalMatch(text);//在整个text文本中匹配当前rule的pattern
        while(matchIterator.hasNext()){//高亮整个文本中匹配到的字符
            QRegularExpressionMatch match=matchIterator.next();
            setFormat(match.capturedStart(),match.capturedLength(),rule.format);//(匹配到的起始位置，文本块长度，高亮规则格式)
        }
    }

    //处理多行注释，由于多行注释优先级最高，所以最后处理
    setCurrentBlockState(0);
    int start=0;
    if(previousBlockState()!=1){//上一个文本块不是多行注释的文本内容，如果是，文本状态应设置成1
        start=text.indexOf(comment_start);//先定位到第一个多行注释的起始字符，找到返回位置，没找到返回-1
    }
    while(start>=0){
        QRegularExpressionMatch match=comment_end.match(text,start);//从当前起始字符匹配第一个结束字符，即当前多行注释的截止字符
        int end=match.capturedStart();//match对应comment_end，此时找的是从当前start(多行注释起始字符)开始匹配的第一个结束字符
        int length=0;
        if(end==-1){//找不到结束字符，说明是最后一个多行注释，为了让下一个文本块知道之前是多行注释模块，所以设状态为1，然后结束位置与文本结束位置相同
            setCurrentBlockState(1);
            length=text.length()-start;
        }else{
            length=end-start+match.capturedLength();//需要高亮的文本长度为 结束字符下标-起始字符下标+匹配到的文本(*/)的长度
        }
        setFormat(start,length,multiLine_comment_format);
        start=text.indexOf(comment_start,start+length);//从当前start+length开始匹配下一个start
    }
}

