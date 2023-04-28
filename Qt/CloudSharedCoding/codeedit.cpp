#include "codeedit.h"
//#include "qpainter.h"
//#include "qscrollbar.h"
#include "ui_codeedit.h"
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QRegExp>
#include "package.h"
#include "mainwindow.h"
#include "useredittip.h"
#include <QSettings>

CodeEdit::CodeEdit(std::shared_ptr<FileInfo> fileptr, QWidget *parent) : QWidget(parent),
                                                                         ui(new Ui::CodeEdit)
{
    ui->setupUi(this);

    document = ui->textEdit->document();
    ui->textEdit->setFont(QFont("Consolas"));
    HighLighter *highLighter = new HighLighter(this, document);
    QHBoxLayout *layout = (QHBoxLayout*)this->layout();
    setLayout(layout);
    ((CodeDocEdit*)ui->textEdit)->setFile(fileptr);

    this->file = fileptr;

    if (MainWindow::loginState())
    {
        auto projPrivilege = MainWindow::userProjs->find(file->file_project)->pro_privilege_level;
        switch (projPrivilege)
        {
        case 1:
        {
            if (file->file_privilege <= 1)
                ui->textEdit->setReadOnly(true);
            break;
        }
        case 2:
        {
            if (file->file_privilege == 1)
            {
                ui->textEdit->setReadOnly(true);
            }
            break;
        }
        case 4:
        {
            ui->textEdit->setReadOnly(true);
            break;
        }
        default:
            break;
        }
    }



    const int tabstop = 4;
    QFontMetrics m(ui->textEdit->font());
    ui->textEdit->setTabStopDistance(tabstop * m.horizontalAdvance(" "));

    if (file->file_project>=0)
        connect(document, SIGNAL(contentsChange(int, int, int)), this, SLOT(docChange(int, int, int)));
    else
        connect(document,&QTextDocument::contentsChange,this,[=](){
            ((CodeDocEdit*)(ui->textEdit))->showAssociateWidget();
        });
}

CodeEdit::~CodeEdit()
{
    delete ui;
}

void CodeEdit::gotoline(int line)
{
    auto block = document->findBlockByLineNumber(line-1);
    QTextCursor cursor(block);
    ui->textEdit->setTextCursor(cursor);
}

void CodeEdit::docChange(int pos, int charRemoved, int charAdded)
{
    ((CodeDocEdit*)(ui->textEdit))->showAssociateWidget();
    //*********************************
    QTextCursor cursor(document);
    cursor.setPosition(pos);
    cursor.setPosition(0, QTextCursor::KeepAnchor);
    QString str = cursor.selectedText();
    str.replace(QChar(8233), '\n');
    str.replace(QChar(8232), '\n');
    int size = str.toStdString().size();
    //*********************************

    QString data = QString::number(file->file_id) + "#" + QString::number(size) + "#" + QString::number(charRemoved) + "#" + file->file_path + "#" + MainWindow::userId + "#";
    for (int var = pos; var < pos + charAdded; ++var)
    {
        if (document->characterAt(var) == QChar(8233) || document->characterAt(var) == QChar(8232))
        {
            if (charRemoved == 1 && charAdded == 1)
            {
                return;
            }
            data += "\n";
        }
        else
            data += document->characterAt(var);
    }

    Package pck(data.toUtf8(), (int)Package::PackageType::TEXT_CHANGE);
    MainWindow::socket->write(pck.getPdata(), pck.getSize());
    MainWindow::socket->flush();
}

void CodeEdit::addText(const QString str)
{
    document->disconnect(SIGNAL(contentsChange(int, int, int)), this, SLOT(docChange(int, int, int)));
    ui->textEdit->insertPlainText(str);
    connect(document, SIGNAL(contentsChange(int, int, int)), this, SLOT(docChange(int, int, int)));
}

void CodeEdit::changeText(int pos, int charRemoved, QString userId, QString data)
{
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, charRemoved);
    document->disconnect(SIGNAL(contentsChange(int, int, int)), this, SLOT(docChange(int, int, int)));
    cursor.removeSelectedText();
    cursor.insertText(data);
    connect(document, SIGNAL(contentsChange(int, int, int)), this, SLOT(docChange(int, int, int)));

    if (userWidget.contains(userId))
    {
        UserEditTip *wind = userWidget.find(userId).value();
        wind->move(ui->textEdit->cursorRect(cursor).center().x()+20,ui->textEdit->cursorRect(cursor).center().y());
        wind->showTip();
    }
    else
    {
        UserEditTip *wind = new UserEditTip(userId, this);
        userWidget.insert(userId, wind);
        wind->move(ui->textEdit->cursorRect(cursor).center().x()+20,ui->textEdit->cursorRect(cursor).center().y());
        wind->showTip();
    }
}

void CodeEdit::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

QString CodeEdit::getText()
{
    return ui->textEdit->toPlainText();
}

HighLighter::HighLighter(CodeEdit *edit, QTextDocument *text) : QSyntaxHighlighter(text),
                                                                edit(edit)
{
    QSettings settings("./configs/configs.ini", QSettings::IniFormat, this);
    settings.beginGroup("CODETHEME");

    // 制定高亮规�???
    HighLighterRule rule;
    this->edit = edit;

    // 1.添加关键字高亮规????
    keyword_format.setForeground(QColor(settings.value("KEYWORD", "#37bb2d").toString())); // 设置关键字前景颜????(blue)
    keyword_format.setFontWeight(QFont::Bold);                                             // 设置关键字的字体格式(Bold)
    QVector<QString> keyword_pattern = {                                                   // \b在表示单词字符边界，防止例如intVal也被识别为int导致高亮
                                        "\\bchar\\b", "\\bclass\\b", "\\bconst\\b", "\\bdouble\\b", "\\benum\\b", "\\bexplicit\\b",
                                        "\\bfriend\\b", "\\binline\\b", "\\bint\\b", "\\blong\\b", "\\bnamespace\\b", "\\boperator\\b",
                                        "\\bprivate\\b", "\\bprotected\\b", "\\bpublic\\b", "\\bshort\\b", "\\bsignals\\b", "\\bsigned\\b",
                                        "\\bslots\\b", "\\bstatic\\b", "\\bstruct\\b", "\\btemplate\\b", "\\btypedef\\b", "\\btypename\\b",
                                        "\\bunion\\b", "\\bunsigned\\b", "\\bvirtual\\b", "\\bvoid\\b", "\\bvolatile\\b", "\\bbool\\b",
                                        "\\busing\\b", "\\bvector\\b", "\\breturn\\b", "\\btrue\\b", "\\bfalse\\b"}; // 关键字集�?
    // 遍历关键字集合，通过正则表达式识别字符串。并设定为rule的pattern，代表当前关键字的标识符；再设定rule的格式，最终加入规则集合中
    for (auto &keyword : keyword_pattern)
    {
        rule.pattern = QRegularExpression(keyword);
        rule.format = keyword_format;
        highlighterrules.push_back(rule);
    } // 规则集合中存储着keyword_pattern中所有关键字的标识符和格????(蓝�??? 粗�???)

    // 2.添加Qt类高亮规????
    class_format.setForeground(QColor(settings.value("CLASS", "#bcc069").toString())); // 设置Qt类前景色(darkCyan)
    class_format.setFontWeight(QFont::Bold);                                           // 设置Qt类字体格????(Bold)
    QString class_pattern = "\\bQ[a-zA-z]+\\b";                                        // Qt类识别格式为两边有分隔符，且以Q开头的所有英文字符�???
    rule.pattern = QRegularExpression(class_pattern);
    rule.format = class_format;
    highlighterrules.push_back(rule);

    // 3.添加头文件高亮格????
    // 3.1 #开????
    headfile_format.setForeground(QColor(settings.value("HEADER", "#a65579").toString()));
    headfile_format.setFontWeight(QFont::Bold);
    rule.format = headfile_format;
    rule.pattern = QRegularExpression("#.*");
    highlighterrules.push_back(rule);

    // 3.2 各头文件
    headfile_format.setForeground(QColor(settings.value("HEADER", "#a65579").toString()));
    headfile_format.setFontWeight(QFont::Bold);
    QVector<QString> headfile_pattern = {
        "<algorithm>", "<bitset>", "<cctype>", "<cerrno>", "<cerrno>", "<cerrno>",
        "\\b<?complex>?\\b", "<cstdio>", "<cstdlib>", "\\b<?cstring>?\\b", "<ctime>", "\\b<?deque>?\\b",
        "\\b<?exception>?\\b", "\\b<?fstream>?\\b", "\\b<?limits>?\\b", "\\b<?list>?\\b", "\\b<?map>?\\b", "<iomanip>", "<ios>",
        "<iosfwd>", "<iostream>", "\\b<?istream>?\\b", "\\b<?ostream>?\\b", "\\b<?queue>?\\b", "\\b<?set>?\\b",
        "\\b<?sstream>?\\b", "\\b<?stack>?\\b", "<stdexcept>", "<streambuf>", "<string>", "<utility>",
        "\\b<?vector>?\\b", "\\b<?cwchar>?\\b", "\\b<?cwctype>?\\b"};
    rule.format = headfile_format;
    for (auto &pattern : headfile_pattern)
    {
        rule.pattern = QRegularExpression(pattern);
        highlighterrules.push_back(rule);
    }

    // 4.添加多行注释高亮规�???
    // 多行注释的匹配正则表达式
    QString comment_start_pattern = "/\\*"; // 开始位�???*，因为正则表达式????*需要用\*表达，�???*需要字符串用\\*表�???
    comment_start = QRegularExpression(comment_start_pattern);
    QString comment_end_pattern = "\\*/"; // 结束位置
    comment_end = QRegularExpression(comment_end_pattern);

    // 5.添加引号高亮规则
    quotation_format.setForeground(QColor(settings.value("QUATATION", "#dbcf2c").toString())); // 引号内容颜�???(cyan)
    QString quotation_pattern = "\".*\"";
    rule.pattern = QRegularExpression(quotation_pattern);
    rule.format = quotation_format;
    highlighterrules.push_back(rule);

    // 6.添加函数高亮格式
    function_format.setForeground(QColor(settings.value("FUNCTION", "#ce9191").toString())); // 函数字体颜色设置为darkGreen
    function_format.setFontWeight(QFont::Bold);                                              // 函数字体格式设置为Bold
    QString function_pattern = "\\b[a-zA-Z0-9_]+(?=\\()";                                    // 函数名可以是大小写英文字符、数字、下划线，其中，(?=\\()表示后面必须跟着一个左括号，但是这个左括号不会被匹配到
    rule.pattern = QRegularExpression(function_pattern);
    rule.format = function_format;
    highlighterrules.push_back(rule);

    // 7.添加分支高亮格式
    branch_format.setForeground(QColor(settings.value("BRANCH", "#ff7637").toString()));
    branch_format.setFontWeight(QFont::Bold);
    QVector<QString> branch_pattern = {
        "if", "else", "switch", "case", "while", "for"};
    rule.format = branch_format;
    for (QString &pattern : branch_pattern)
    {
        rule.pattern = QRegularExpression(pattern);
        highlighterrules.push_back(rule);
    }

    // 8.添加输入输出高亮格�???
    cincout_format.setForeground(QColor(settings.value("STDIO", "#8962c1").toString()));
    cincout_format.setFontWeight(QFont::Bold);
    QVector<QString> cincout_pattern = {
        "cin", "cout", "std", "endl", "<<", ">>"};
    rule.format = cincout_format;
    for (auto &pattern : cincout_pattern)
    {
        rule.pattern = QRegularExpression(pattern);
        highlighterrules.push_back(rule);
    }

    // 9.添加单行注释高亮规�???
    singleLine_comment_format.setForeground(QColor(settings.value("SIGNLE_LINE_COMMENT", "#6e888f").toString()));
    singleLine_comment_format.setFontWeight(QFont::Bold);
    QString singleLine_comment_pattern = "//[^\n]*"; // 单行注释识别格式为跟�?//后，但不包括换行符，且不需要间隔符
    rule.pattern = QRegularExpression(singleLine_comment_pattern);
    rule.format = singleLine_comment_format;
    highlighterrules.push_back(rule);

    // 多行注释格�???
    multiLine_comment_format.setForeground(QColor(settings.value("MULITLINE_COMMENT", "#82887f").toString()));
    multiLine_comment_format.setFontWeight(QFont::Bold);

    settings.endGroup();
}

void HighLighter::highlightBlock(const QString &text)
{ // 应用高亮规�???
    foreach (const HighLighterRule &rule, highlighterrules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text); // 在整个text文本中匹配当前rule的pattern
        while (matchIterator.hasNext())
        { // 高亮整个文本中匹配到的字????
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format); //(匹配到的起始位置，文本块长度，高亮规则格�?)
        }
    }

    // 处理多行注释，由于多行注释优先级最高，所以最后处????
    setCurrentBlockState(0);
    int start = 0;
    if (previousBlockState() != 1)
    {                                        // 上一个文本块不是多行注释的文本内容，如果是，文本状态应设置????1
        start = text.indexOf(comment_start); // 先定位到第一个多行注释的起始字符，找到返回位置，没找到返�?-1
    }
    while (start >= 0)
    {
        QRegularExpressionMatch match = comment_end.match(text, start); // 从当前起始字符匹配第一个结束字符，即当前多行注释的截止字�???
        int end = match.capturedStart();                                // match对应comment_end，此时找的是从当前start(多行注释起始字�???)开始匹配的第一个结束字�?
        int length = 0;
        if (end == -1)
        { // 找不到结束字符，说明是最后一个多行注释，为了让下一个文本块知道之前是多行注释模块，所以设状态为1，然后结束位置与文本结束位置相同
            setCurrentBlockState(1);
            length = text.length() - start;
        }
        else
        {
            length = end - start + match.capturedLength(); // 需要高亮的文本长度???? 结束字符下标-起始字符下标+匹配到的文�???(*/)的长�?
        }
        setFormat(start, length, multiLine_comment_format);
        start = text.indexOf(comment_start, start + length); // 从当前start+length开始匹配下一个start
    }
}

void CodeEdit::highlightError(const QString &error){
    QRegularExpression errorRegex("(.*):(\\d+):(\\d+):\\s+(error|warning):(.*)");
    int errorOffset=0;
    //QTextCursor cursor=this->document().
    while(errorOffset!=-1){
        auto match=errorRegex.match(error,errorOffset);
        if(match.hasMatch()){
            int lineNumber=match.captured(2).toInt();
            int columnNumber = match.captured(3).toInt()-1;
            QString errorData = match.captured(5);
            int left=0;
            int right=0;
            for(int i=0;i<errorData.length();i++){
                if(errorData[i]==QChar(8216)){
                    left=i+1;
                    break;
                }
            }
            for(int i=left;i<errorData.length();i++){
                if(errorData[i]==QChar(8217)){
                    right=i-1;
                    break;
                }
            }
            QString errorChar;
            for(int i=left;i<=right;i++){
                errorChar.push_back(errorData[i]);
            }
            QTextBlock block=ui->textEdit->document()->findBlockByLineNumber(lineNumber-1);
            QTextCursor cursor(block);
//            cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::MoveAnchor,columnNumber-block.length());
//            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, errorChar.length());
            cursor.select(QTextCursor::SelectionType::BlockUnderCursor);
            QTextCharFormat error_format;
            error_format.setUnderlineColor(Qt::red);
            error_format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
            cursor.mergeCharFormat(error_format);
        }
        errorOffset=match.capturedEnd();
    }
}
