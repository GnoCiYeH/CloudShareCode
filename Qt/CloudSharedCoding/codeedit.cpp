#include "codeedit.h"
#include "qpainter.h"
#include "qscrollbar.h"
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

    // 初始化联想列�?
    setUpAssociateList();
    associateWidget = new AssociateListWidget(this, ui->textEdit);
    associateWidget->hide();
    associateWidget->setMaximumHeight(fontMetrics().height() * 5);
    associateState = AssociateState::Hide;

    const int tabstop = 4;
    QFontMetrics m(ui->textEdit->font());
    ui->textEdit->setTabStopDistance(tabstop * m.horizontalAdvance(" "));

    if (MainWindow::loginState())
        connect(document, SIGNAL(contentsChange(int, int, int)), this, SLOT(docChange(int, int, int)));
}

CodeEdit::~CodeEdit()
{
    delete ui;
}

void CodeEdit::docChange(int pos, int charRemoved, int charAdded)
{
    showAssociateWidget();
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

    // ?????????????
    QMap<QChar, QChar> map;
    map['('] = ')';
    map['['] = ']';
    map['{'] = '}';
    map['\"'] = '\"';
    map['<'] = '>';
    int preCharIndex = ui->textEdit->textCursor().position() - 1;
    QChar preChar = document->characterAt(preCharIndex);
    QString text = ui->textEdit->textCursor().block().text();
    if (preChar == '(' || preChar == '[' || preChar == '{' || preChar == '\"' || (text.contains("#include") && preChar == '<'))
    {
        ui->textEdit->insertPlainText(map[preChar]);
        data+=map[preChar];
        ui->textEdit->moveCursor(QTextCursor::PreviousCharacter);
        if (preChar == '{')
        {
            QTextCursor cursor = ui->textEdit->textCursor();
            int startPos = cursor.block().position();
            int spaceCount = 0;
            while (document->characterAt(startPos) == ' ')
            {
                spaceCount++;
                startPos++;
            }
            ui->textEdit->insertPlainText("\n");
            data+="\n";
            ui->textEdit->insertPlainText(QString(spaceCount + 4, ' '));
            data+=QString(spaceCount + 4, ' ');
            QTextBlock middleBlock = document->findBlockByLineNumber(ui->textEdit->textCursor().blockNumber());
            QTextCursor middleCursor(middleBlock);
            cursor.deleteChar();
            ui->textEdit->insertPlainText("\n");
            data+="\n";
            ui->textEdit->insertPlainText(QString(spaceCount, ' '));
            data+=QString(spaceCount + 4, ' ');
            ui->textEdit->insertPlainText("}");
            data+="}";
            middleCursor.setPosition(middleBlock.position() + spaceCount + 4);
            ui->textEdit->setTextCursor(middleCursor);
        }
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
        wind->move(ui->textEdit->cursorRect(cursor).center());
        wind->showTip();
    }
    else
    {
        UserEditTip *wind = new UserEditTip(userId, this);
        userWidget.insert(userId, wind);
        wind->move(ui->textEdit->cursorRect(cursor).center());
        wind->showTip();
    }
}

void CodeEdit::showAssociateWidget()
{
    if (associateState == AssociateState::Ignore)
        return; // 对于光标和文本变化不做任何相应，避免陷入死循????
    associateWidget->hide();
    associateState = AssociateState::Hide;
    QString word = this->getWordCursor();
    associateWidget->clear();
    if (!word.isEmpty())
    {
        int maxSize = 0;
        QMap<QString, int> differenceRecord;
        vector<QString> itemVec;
        foreach (const QString &keyword, *associateList)
        {
            if (keyword.contains(word))
            { // 如果当前输入字符属于联想表中的字符串
                itemVec.push_back(keyword);
                differenceRecord[keyword] = associateWidget->letterDifference(keyword.toStdString(), word.toStdString());
                if (keyword.length() > maxSize)
                    maxSize = keyword.length(); // 找到联想列表中最长的一个，好设置联想列表宽�?
            }
        }

        if (itemVec.size() > 0)
        { // 有匹配字????
            // 按差异度从小到大排，最匹配的在最前面
            sort(itemVec.begin(), itemVec.end(), [&](const QString &s1, const QString &s2) -> bool
                 { return differenceRecord[s1] < differenceRecord[s2]; });
            foreach (const QString &item, itemVec)
            {
                associateWidget->addItem(new QListWidgetItem(item));
            }

            int x = this->getAssociateWidgetX();
            int y = ui->textEdit->cursorRect().y() + fontMetrics().height();

            associateWidget->move(x, y); // 设置联想列表的位????
            // 设置联想列表合适的大小
            if (associateWidget->count() > 5)
                associateWidget->setFixedHeight(fontMetrics().height() * 6);
            else
                associateWidget->setFixedHeight(fontMetrics().height() * (associateWidget->count() + 1));
            associateWidget->setFixedHeight((fontMetrics().lineWidth() + 6) * maxSize);
            associateWidget->show();
            associateState = AssociateState::Showing;
            associateWidget->setCurrentRow(0, QItemSelectionModel::Select);
        }
        associateWidget->setFocus();
    }
}

QString CodeEdit::getWordCursor()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
    int start = cursor.position();
    QString res = "";
    QChar ch = document->characterAt(start);
    while (ch.isDigit() || ch.isLetter() || ch == '_' || ch == '#')
    {
        res.push_back(ch);
        ch = ui->textEdit->document()->characterAt(++start);
    }
    return res;
}

QString CodeEdit::getText()
{
    return ui->textEdit->toPlainText();
}

int CodeEdit::getAssociateWidgetX()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    int pos = cursor.position() - 1;
    int originalPos = pos + 1;
    QChar ch = ui->textEdit->document()->characterAt(pos);
    while ((ch.isDigit() || ch.isLetter() || ch == '_' || ch == '#') && pos > 0)
    {
        ch = ui->textEdit->document()->characterAt(pos--);
    }
    pos++;
    associateState = AssociateState::Ignore;
    cursor.setPosition(pos);
    ui->textEdit->setTextCursor(cursor);
    int x = ui->textEdit->cursorRect().x() + 2 * fontMetrics().lineWidth();
    cursor.setPosition(originalPos);
    ui->textEdit->setTextCursor(cursor);
    associateState = AssociateState::Hide;
    return x;
}

void CodeEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (associateState == AssociateState::Showing)
        {
            QKeyEvent *newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
            QApplication::sendEvent(associateWidget, newEvent); // ����ģ����???
            return;                                             // �����û���ĺ�������ֹ�ƶ���???
        }
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (associateState == AssociateState::Showing)
        {
            QKeyEvent *newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
            QApplication::sendEvent(associateWidget, newEvent); // ����ģ����???
            return;                                             // �����û���ĺ�������ֹ�ƶ���???
        }
    }
    else if (event->key() == Qt::Key_Up)
    {
        if (associateState == AssociateState::Showing)
        {
            QKeyEvent *newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
            QApplication::sendEvent(associateWidget, newEvent); // ����ģ����???
            return;                                             // �����û���ĺ�������ֹ�ƶ���???
        }
    }
}

void CodeEdit::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

int CodeEdit::lineNumberAreaWidth()
{
    int digits = 1;//?????????��??
    int max = qMax(1, ui->textEdit->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    if(digits<3)digits=3;

    int space = 3 + ui->textEdit->fontMetrics().averageCharWidth()*digits;

    return space;
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
    keyword_format.setForeground(QColor(settings.value("KEYWORD", "#00ffff").toString())); // 设置关键字前景颜????(blue)
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
    class_format.setForeground(QColor(settings.value("CLASS", "#00ffff").toString())); // 设置Qt类前景色(darkCyan)
    class_format.setFontWeight(QFont::Bold);                                           // 设置Qt类字体格????(Bold)
    QString class_pattern = "\\bQ[a-zA-z]+\\b";                                        // Qt类识别格式为两边有分隔符，且以Q开头的所有英文字符�???
    rule.pattern = QRegularExpression(class_pattern);
    rule.format = class_format;
    highlighterrules.push_back(rule);

    // 3.添加头文件高亮格????
    // 3.1 #开????
    headfile_format.setForeground(QColor(settings.value("HEADER", "#00ffff").toString()));
    headfile_format.setFontWeight(QFont::Bold);
    rule.format = headfile_format;
    rule.pattern = QRegularExpression("#.*");
    highlighterrules.push_back(rule);

    // 3.2 各头文件
    headfile_format.setForeground(QColor(settings.value("HEADER", "#00ffff").toString()));
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
    quotation_format.setForeground(QColor(settings.value("QUATATION", "#00ffff").toString())); // 引号内容颜�???(cyan)
    QString quotation_pattern = "\".*\"";
    rule.pattern = QRegularExpression(quotation_pattern);
    rule.format = quotation_format;
    highlighterrules.push_back(rule);

    // 6.添加函数高亮格式
    function_format.setForeground(QColor(settings.value("FUNCTION", "#00ffff").toString())); // 函数字体颜色设置为darkGreen
    function_format.setFontWeight(QFont::Bold);                                              // 函数字体格式设置为Bold
    QString function_pattern = "\\b[a-zA-Z0-9_]+(?=\\()";                                    // 函数名可以是大小写英文字符、数字、下划线，其中，(?=\\()表示后面必须跟着一个左括号，但是这个左括号不会被匹配到
    rule.pattern = QRegularExpression(function_pattern);
    rule.format = function_format;
    highlighterrules.push_back(rule);

    // 7.添加分支高亮格式
    branch_format.setForeground(QColor(settings.value("BRANCH", "#00ffff").toString()));
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
    cincout_format.setForeground(QColor(settings.value("STDIO", "#00ffff").toString()));
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
    singleLine_comment_format.setForeground(QColor(settings.value("SIGNLE_LINE_COMMENT", "#00ffff").toString()));
    singleLine_comment_format.setFontWeight(QFont::Bold);
    QString singleLine_comment_pattern = "//[^\n]*"; // 单行注释识别格式为跟�?//后，但不包括换行符，且不需要间隔符
    rule.pattern = QRegularExpression(singleLine_comment_pattern);
    rule.format = singleLine_comment_format;
    highlighterrules.push_back(rule);

    // 多行注释格�???
    multiLine_comment_format.setForeground(QColor(settings.value("MULITLINE_COMMENT", "#00ffff").toString()));
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

void HighLighter::highlightError(const QString &error){
    QRegularExpression errorRegex("(.*):(\\d+):(\\d+):\\s+(error|warning):(.*)");
    int errorOffset=0;
    QTextCursor cursor=edit->ui->textEdit->textCursor();
    while(errorOffset!=-1){
        auto match=errorRegex.match(error,errorOffset);
        if(match.hasMatch()){
            int lineNumber=match.captured(2).toInt();
            int columnNumber = match.captured(3).toInt()-1;
            QString errorData = match.captured(5);
            int left=0;
            int right=0;
            for(int i=0;i<errorData.length();i++){
                if(errorData[i]=='\''){
                    left=i+1;
                    break;
                }
            }
            for(int i=left;i<errorData.length();i++){
                if(errorData[i]=='\''){
                    right=i-1;
                    break;
                }
            }
            QString errorChar;
            for(int i=left;i<=right;i++){
                errorChar.push_back(errorData[i]);
            }
            QTextBlock block=edit->ui->textEdit->document()->findBlockByLineNumber(lineNumber);
            QTextCursor cursor=QTextCursor(block);
            cursor.setPosition(block.position());
            cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,columnNumber);
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, errorChar.length()-1);
            QTextCharFormat error_format;
            error_format.setUnderlineColor(Qt::red);
            error_format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
            cursor.mergeCharFormat(error_format);
        }
    }
}

void setUpAssociateList()
{
    associateList=MainWindow::fileName;
    *associateList << "char"
                  << "class"
                  << "const"
                  << "double"
                  << "enum"
                  << "explicit"
                  << "friend"
                  << "inline"
                  << "int"
                  << "long"
                  << "namespace"
                  << "operator"
                  << "private"
                  << "protected"
                  << "public"
                  << "short"
                  << "signals"
                  << "signed"
                  << "slots"
                  << "static"
                  << "struct"
                  << "template"
                  << "typedef"
                  << "typename"
                  << "union"
                  << "unsigned"
                  << "virtual"
                  << "void"
                  << "volatile"
                  << "bool"
                  << "using"
                  << "constexpr"
                  << "sizeof"
                  << "if"
                  << "for"
                  << "foreach"
                  << "while"
                  << "do"
                  << "case"
                  << "break"
                  << "continue"
                  << "template"
                  << "delete"
                  << "new"
                  << "default"
                  << "try"
                  << "return"
                  << "throw"
                  << "catch"
                  << "goto"
                  << "else"
                  << "extren"
                  << "this"
                  << "switch"
                  << "include <>"
                  << "include \""
                  << "define"
                  << "iostream";
}

AssociateListWidget::AssociateListWidget(CodeEdit *edit, QWidget *parent) : QListWidget(parent),
                                                                            edit(edit)
{
    p = (QPlainTextEdit *)parent;
    backgroundColor = Qt::lightGray;
    highlightColor.setRgb(22, 165, 248);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Active, QPalette::Highlight, highlightColor);
    palette.setColor(QPalette::Inactive, QPalette::Highlight, highlightColor);
    palette.setColor(QPalette::Active, QPalette::Base, backgroundColor);
    palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
    palette.setColor(QPalette::Text, Qt::white);
    this->setPalette(palette);
}

int AssociateListWidget::letterDifference(const std::string source, const std::string text)
{
    int difference = 0;
    for (int i = 0; i < source.length(); i++)
    {
        if (i < text.length())
            difference += abs(text[i] - source[i]);
        else
        {
            if (difference == 0)
            {
                return strToInt(source.substr(i, source.length() - i));
            }
            difference += source[i];
        }
    }
    return difference;
}

int AssociateListWidget::strToInt(string str)
{
    int res = 0;
    for (int i = 0; i < str.length(); i++)
    {
        res += str[i] - 'a';
    }
    return res;
}

void AssociateListWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        // ��ȡ��ǰѡ�е���
        QListWidgetItem *item = currentItem();
        if (item)
        {
            QString text = this->currentItem()->text();
            QString word = edit->getWordCursor();
            for (int i = 0; i < word.length(); i++)
            {
                edit->ui->textEdit->textCursor().deletePreviousChar();
            }
            edit->ui->textEdit->insertPlainText(text);
        }
        this->hide();
        edit->ui->textEdit->setFocus();
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (this->currentRow() < this->count() - 1)
        {
            this->setCurrentRow(this->currentRow() + 1);
        }
    }
    else if (event->key() == Qt::Key_Up)
    {
        if (this->currentRow() > 0)
        {
            this->setCurrentRow(this->currentRow() - 1);
        }
    }
    else
    {
        event->ignore();
    }
}
