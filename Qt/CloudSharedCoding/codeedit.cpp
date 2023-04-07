#include "codeedit.h"
#include "ui_codeedit.h"
#include<QKeyEvent>
#include<QDebug>
#include<QTimer>
#include"mainwindow.h"
#include "package.h"

CodeEdit::CodeEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CodeEdit)
{
    ui->setupUi(this);

    document = ui->textEdit->document();
    //ui->textEdit->setFontFamily("Consolas");
    ui->textEdit->setFont(QFont("Consolas"));
    HighLighter* highLighter = new HighLighter(document);

    //初始化联想列表
    setUpAssociateList();
    associateWidget= new AssociateListWidget(ui->textEdit);
    associateWidget->hide();
    associateWidget->setMaximumHeight(fontMetrics().height()*5);
    associateState=AssociateState::Hide;

    const int tabstop = 4;
    QFontMetrics m(ui->textEdit->font());
    ui->textEdit->setTabStopDistance(tabstop*m.horizontalAdvance(" "));
//    ui->textEdit->insertPlainText("1\n");
//    document->begin().setVisible(false);

    //connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(textChange()));
    connect(document,SIGNAL(contentsChange(int,int,int)),this,SLOT(docChange(int,int,int)));

    thread = new EditWorkThread(this);
    thread->start();
}

CodeEdit::~CodeEdit()
{
    delete ui;
    thread->exit(0);
    delete thread;
}

void CodeEdit::docChange(int pos,int charRemoved,int charAdded)
{
    showAssociateWidget();
    qDebug()<<pos<<" "<<charRemoved<<" "<<charAdded;
    QString data = QString::number(file->file_id)+"#"+QString::number(pos)+"#"+QString::number(charRemoved)+"#"+file->file_path+"#";
    for (int var = pos; var < pos+charAdded; ++var) {
        if(document->characterAt(var)==QChar(8233))
            data+="\n";
        else
            data+=document->characterAt(var);
    }
    qDebug()<<data;
    Package pck(data.toUtf8(),(int)Package::PackageType::TEXT_CHANGE);
    MainWindow::socket->write(pck.getPdata(),pck.getSize());
    MainWindow::socket->flush();
}

void CodeEdit::addText(const QString str)
{
    document->disconnect(SIGNAL(contentsChange(int,int,int)));
    ui->textEdit->insertPlainText(str);
    connect(document,SIGNAL(contentsChange(int,int,int)),this,SLOT(docChange(int,int,int)));
}

void CodeEdit::changeText(int pos,int charRemoved,QString data)
{
    QTextCursor cursor(document);
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::MoveAnchor,pos);
    qDebug()<<ui->textEdit->toPlainText().size();
    cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor,charRemoved);
    document->disconnect(SIGNAL(contentsChange(int,int,int)));
    cursor.removeSelectedText();
    cursor.insertText(data);
    connect(document,SIGNAL(contentsChange(int,int,int)),this,SLOT(docChange(int,int,int)));
}

void CodeEdit::showAssociateWidget(){
    if(associateState==AssociateState::Ignore)return;//对于光标和文本变化不做任何相应，避免陷入死循环
    associateWidget->hide();
    associateState=AssociateState::Hide;
    QString word=this->getWordCursor();
    associateWidget->clear();
    if(!word.isEmpty()){
        int maxSize=0;
        QMap<QString,int>differenceRecord;
        vector<QString>itemVec;
        foreach(const QString &keyword,associateList){
            if(keyword.contains(word)){//如果当前输入字符属于联想表中的字符串
                itemVec.push_back(keyword);
                differenceRecord[keyword]=AssociateListWidget::letterDifference(keyword.toStdString(),word.toStdString());
                if(keyword.length()>maxSize)maxSize=keyword.length();//找到联想列表中最长的一个，好设置联想列表宽度
            }
        }

        if(itemVec.size()>0){//有匹配字符
            //按差异度从小到大排，最匹配的在最前面
            sort(itemVec.begin(),itemVec.end(),[&](const QString &s1,const QString &s2)->bool{return differenceRecord[s1]<differenceRecord[s2];});
            foreach(const QString& item,itemVec){
                associateWidget->addItem(new QListWidgetItem(item));
            }

            int x=this->getAssociateWidgetX();
            int y=ui->textEdit->cursorRect().y()+fontMetrics().height();

            associateWidget->move(x,y);//设置联想列表的位置
            //设置联想列表合适的大小
            if(associateWidget->count()>5)associateWidget->setFixedHeight(fontMetrics().height()*6);
            else associateWidget->setFixedHeight(fontMetrics().height()*(associateWidget->count()+1));
            associateWidget->setFixedHeight((fontMetrics().lineWidth()+6)*maxSize);
            associateWidget->show();
            associateState=AssociateState::Showing;
            associateWidget->setCurrentRow(0,QItemSelectionModel::Select);
        }
    }
}

void CodeEdit::setUpAssociateList(){
    associateList<< "char" << "class" << "const"
                 << "double" << "enum" << "explicit"
                 << "friend" << "inline" << "int"
                 << "long" << "namespace" << "operator"
                 << "private" << "protected" << "public"
                 << "short" << "signals" << "signed"
                 << "slots" << "static" << "struct"
                 << "template" << "typedef" << "typename"
                 << "union" << "unsigned" << "virtual"
                 << "void" << "volatile" << "bool"<<"using"<<"constexpr"
                 <<"sizeof"<<"if"<<"for"<<"foreach"<<"while"<<"do"<<"case"
                 <<"break"<<"continue"<<"template"<<"delete"<<"new"
                 <<"default"<<"try"<<"return"<<"throw"<<"catch"<<"goto"<<"else"
                 <<"extren"<<"this"<<"switch"<<"#include <>"<<"#include \"\""<<"#define"<<"iostream";
}

QString CodeEdit::getWordCursor(){
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
    int start = cursor.position();
    QString res = "";
    QChar ch = document->characterAt(start);
    while(ch.isDigit()||ch.isLetter()||ch=='_'||ch=='#'){
        res.push_back(ch);
        ch=ui->textEdit->document()->characterAt(++start);
    }
    return res;
}

int CodeEdit::getAssociateWidgetX(){
    QTextCursor cursor=ui->textEdit->textCursor();
    int pos=cursor.position()-1;
    int originalPos=pos+1;
    QChar ch=ui->textEdit->document()->characterAt(pos);
    while((ch.isDigit()||ch.isLetter()||ch=='_'||ch=='#')&&pos>0){
        ch=ui->textEdit->document()->characterAt(pos--);
    }
    pos++;
    associateState=AssociateState::Ignore;
    cursor.setPosition(pos);
    ui->textEdit->setTextCursor(cursor);
    int x=ui->textEdit->cursorRect().x()+2*fontMetrics().lineWidth();
    cursor.setPosition(originalPos);
    ui->textEdit->setTextCursor(cursor);
    associateState=AssociateState::Hide;
    return x;
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
    QTimer subTimer;
    connect(&subTimer,&QTimer::timeout,this,[=](){
            if(codeEdit->isChanged)
            {
                char* data = codeEdit->document->toPlainText().toUtf8().data();
                int offset = 0;
            }
        },Qt::DirectConnection);
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
      "\\busing\\b","vector","return"
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

    //3.添加头文件高亮格式
    //3.1 #开头
    headfile_format.setForeground(Qt::darkGray);
    headfile_format.setFontWeight(QFont::Bold);
    rule.format=headfile_format;
    rule.pattern=QRegularExpression("#.*");
    highlighterrules.push_back(rule);

    //3.2 各头文件
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

    //9.添加单行注释高亮规则
    singleLine_comment_format.setForeground(QColor(211, 211 ,211));//注释颜色为green
    QString singleLine_comment_pattern="//[^\n]*";//单行注释识别格式为跟在//后，但不包括换行符，且不需要间隔符
    rule.pattern=QRegularExpression(singleLine_comment_pattern);
    rule.format=singleLine_comment_format;
    highlighterrules.push_back(rule);
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

AssociateListWidget::AssociateListWidget(QWidget*parent):QListWidget(parent){
    p=(QPlainTextEdit*)parent;
    backgroundColor=Qt::lightGray;
    highlightColor.setRgb(22,165,248);
    QPalette palette=this->palette();
    palette.setColor(QPalette::Active,QPalette::Highlight,highlightColor);
    palette.setColor(QPalette::Inactive,QPalette::Highlight,highlightColor);
    palette.setColor(QPalette::Active, QPalette::Base,backgroundColor);
    palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
    palette.setColor(QPalette::Text,Qt::white);
    this->setPalette(palette);
}

void AssociateListWidget::keyPressEvent(QKeyEvent *event){
    if(event->key()==16777235||event->key()==16777237){//如果用户按下回车或ESC键
        QString str = this->currentItem()->text();
    }else{
        QApplication::sendEvent(p,event);
        p->setFocus();
    }
    return QListWidget::keyPressEvent(event);
}

int AssociateListWidget::letterDifference(const std::string source, const std::string target){
    int n = source.length();
    int m = target.length();
    if (m == 0) return n;
    if (n == 0) return m;
    vector< vector<int> >dMatrix(n + 1,vector<int>(m+1));//dMatrix[i][j]：source[0,i-1]和target[0,j-1]字符串的最小差异度(有几个字符不同);
    for (int i = 1; i <= n; i++) dMatrix[i][0] = i;
    for (int j = 1; j <= m; j++) dMatrix[0][j] = j;

    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){
            if(source[i-1]==target[j-1]){
                dMatrix[i][j]=dMatrix[i-1][j-1];
            }else{
                dMatrix[i][j]=min(dMatrix[i-1][j],dMatrix[i][j-1])+1;//不取source[i-1]或不取target[j-1]
            }
        }
    }
    return dMatrix[n][m];
}
