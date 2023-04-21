#include "codeedit.h"
#include "qpainter.h"
#include "qscrollbar.h"
#include "ui_codeedit.h"
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QRegExp>
#include "mainwindow.h"
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

    connect(document, &QTextDocument::blockCountChanged, this, &CodeEdit::updateLineNumberAreaWidth);
    connect(ui->textEdit->verticalScrollBar(), &QScrollBar::valueChanged, this, &CodeEdit::updateLineNumberArea);
    connect(ui->textEdit, &QPlainTextEdit::cursorPositionChanged, this, &CodeEdit::highlightCurrentLine);

    updateLineNumberAreaWidth();

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

    // 鍒濆鍖栬仈鎯冲垪琛?
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
    qDebug() << pos << " " << charRemoved << " " << charAdded;
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

    // �Զ�����������
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

    qDebug() << data;
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
    qDebug() << ui->textEdit->toPlainText().size();
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
        return; // 瀵逛簬鍏夋爣鍜屾枃鏈彉鍖栦笉鍋氫换浣曠浉搴旓紝閬垮厤闄峰叆姝诲惊��??
    associateWidget->hide();
    associateState = AssociateState::Hide;
    QString word = this->getWordCursor();
    associateWidget->clear();
    if (!word.isEmpty())
    {
        int maxSize = 0;
        QMap<QString, int> differenceRecord;
        vector<QString> itemVec;
        foreach (const QString &keyword, associateList)
        {
            if (keyword.contains(word))
            { // 濡傛灉褰撳墠杈撳叆瀛楃灞炰簬鑱旀兂琛ㄤ腑鐨勫瓧绗︿覆
                itemVec.push_back(keyword);
                differenceRecord[keyword] = associateWidget->letterDifference(keyword.toStdString(), word.toStdString());
                if (keyword.length() > maxSize)
                    maxSize = keyword.length(); // 鎵惧埌鑱旀兂鍒楄〃涓渶闀跨殑涓€涓紝濂借缃仈鎯冲垪琛ㄥ搴?
            }
        }

        if (itemVec.size() > 0)
        { // 鏈夊尮閰嶅瓧��??
            // 鎸夊樊寮傚害浠庡皬鍒板ぇ鎺掞紝鏈€鍖归厤鐨勫湪鏈€鍓嶉潰
            sort(itemVec.begin(), itemVec.end(), [&](const QString &s1, const QString &s2) -> bool
                 { return differenceRecord[s1] < differenceRecord[s2]; });
            foreach (const QString &item, itemVec)
            {
                associateWidget->addItem(new QListWidgetItem(item));
            }

            int x = this->getAssociateWidgetX();
            int y = ui->textEdit->cursorRect().y() + fontMetrics().height();

            associateWidget->move(x, y); // 璁剧疆鑱旀兂鍒楄〃鐨勪綅��??
            // 璁剧疆鑱旀兂鍒楄〃鍚堥€傜殑澶у皬
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
            QApplication::sendEvent(associateWidget, newEvent); // 发送模拟事�?
            return;                                             // 不调用基类的函数，防止移动光�?
        }
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (associateState == AssociateState::Showing)
        {
            QKeyEvent *newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
            QApplication::sendEvent(associateWidget, newEvent); // 发送模拟事�?
            return;                                             // 不调用基类的函数，防止移动光�?
        }
    }
    else if (event->key() == Qt::Key_Up)
    {
        if (associateState == AssociateState::Showing)
        {
            QKeyEvent *newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
            QApplication::sendEvent(associateWidget, newEvent); // 发送模拟事�?
            return;                                             // 不调用基类的函数，防止移动光�?
        }
    }
}

void CodeEdit::updateLineNumberAreaWidth()
{
    int digits = 1;
    int max = 1;
    if (document->blockCount() > 1)
    {
        max = document->blockCount();
    }
    while (max >= 10)
    {
        max /= 10;
        digits++;
    }
    int width = 8 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    ui->lineNumberArea->setMinimumWidth(width);
    ui->lineNumberArea->setMaximumWidth(width);
}

void CodeEdit::updateLineNumberArea(const int)
{
    // �����к�����Ĵ�ֱ����λ��
    ui->lineNumberArea->scroll(0, ui->textEdit->verticalScrollBar()->value());
    // �ػ��к�����
    ui->lineNumberArea->update();
}

void CodeEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!ui->textEdit->isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::darkGray).lighter(10);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = ui->textEdit->textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    ui->textEdit->setExtraSelections(extraSelections);
}

void CodeEdit::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

HighLighter::HighLighter(CodeEdit *edit, QTextDocument *text) : QSyntaxHighlighter(text),
                                                                edit(edit)
{
    QSettings settings("./configs/configs.ini", QSettings::IniFormat, this);
    settings.beginGroup("CODETHEME");

    // 鍒跺畾楂樹寒瑙勫��?
    HighLighterRule rule;
    this->edit = edit;

    // 1.娣诲姞鍏抽敭瀛楅珮浜��??
    keyword_format.setForeground(QColor(settings.value("KEYWORD", "#00ffff").toString())); // 璁剧疆鍏抽敭瀛楀墠鏅��??(blue)
    keyword_format.setFontWeight(QFont::Bold);                                             // 璁剧疆鍏抽敭瀛楃殑瀛椾綋鏍煎紡(Bold)
    QVector<QString> keyword_pattern = {                                                   // \b鍦ㄨ〃绀哄崟璇嶅瓧绗﹁竟鐣岋紝闃叉渚嬪intVal涔熻璇嗗埆涓篿nt瀵艰嚧楂樹寒
                                        "\\bchar\\b", "\\bclass\\b", "\\bconst\\b", "\\bdouble\\b", "\\benum\\b", "\\bexplicit\\b",
                                        "\\bfriend\\b", "\\binline\\b", "\\bint\\b", "\\blong\\b", "\\bnamespace\\b", "\\boperator\\b",
                                        "\\bprivate\\b", "\\bprotected\\b", "\\bpublic\\b", "\\bshort\\b", "\\bsignals\\b", "\\bsigned\\b",
                                        "\\bslots\\b", "\\bstatic\\b", "\\bstruct\\b", "\\btemplate\\b", "\\btypedef\\b", "\\btypename\\b",
                                        "\\bunion\\b", "\\bunsigned\\b", "\\bvirtual\\b", "\\bvoid\\b", "\\bvolatile\\b", "\\bbool\\b",
                                        "\\busing\\b", "\\bvector\\b", "\\breturn\\b", "\\btrue\\b", "\\bfalse\\b"}; // 鍏抽敭瀛楅泦鍚?
    // 閬嶅巻鍏抽敭瀛楅泦鍚堬紝閫氳繃姝ｅ垯琛ㄨ揪寮忚瘑鍒瓧绗︿覆銆傚苟璁惧畾涓簉ule鐨刾attern锛屼唬琛ㄥ綋鍓嶅叧閿瓧鐨勬爣璇嗙锛涘啀璁惧畾rule鐨勬牸寮忥紝鏈€缁堝姞鍏ヨ鍒欓泦鍚堜腑
    for (auto &keyword : keyword_pattern)
    {
        rule.pattern = QRegularExpression(keyword);
        rule.format = keyword_format;
        highlighterrules.push_back(rule);
    } // 瑙勫垯闆嗗悎涓瓨鍌ㄧ潃keyword_pattern涓墍鏈夊叧閿瓧鐨勬爣璇嗙鍜屾牸��??(钃濊��? 绮椾��?)

    // 2.娣诲姞Qt绫婚珮浜��??
    class_format.setForeground(QColor(settings.value("CLASS", "#00ffff").toString())); // 璁剧疆Qt绫诲墠鏅壊(darkCyan)
    class_format.setFontWeight(QFont::Bold);                                           // 璁剧疆Qt绫诲瓧浣撴牸��??(Bold)
    QString class_pattern = "\\bQ[a-zA-z]+\\b";                                        // Qt绫昏瘑鍒牸寮忎负涓よ竟鏈夊垎闅旂锛屼笖浠寮€澶寸殑鎵€鏈夎嫳鏂囧瓧绗︿��?
    rule.pattern = QRegularExpression(class_pattern);
    rule.format = class_format;
    highlighterrules.push_back(rule);

    // 3.娣诲姞澶存枃浠堕珮浜牸��??
    // 3.1 #寮€��??
    headfile_format.setForeground(QColor(settings.value("HEADER", "#00ffff").toString()));
    headfile_format.setFontWeight(QFont::Bold);
    rule.format = headfile_format;
    rule.pattern = QRegularExpression("#.*");
    highlighterrules.push_back(rule);

    // 3.2 鍚勫ご鏂囦欢
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

    // 4.娣诲姞澶氳娉ㄩ噴楂樹寒瑙勫��?
    // 澶氳娉ㄩ噴鐨勫尮閰嶆鍒欒〃杈惧紡
    QString comment_start_pattern = "/\\*"; // 寮€濮嬩綅缃��?*锛屽洜涓烘鍒欒〃杈惧紡��??*闇€瑕佺敤\*琛ㄨ揪锛岃€��?*闇€瑕佸瓧绗︿覆鐢╘\*琛ㄨ��?
    comment_start = QRegularExpression(comment_start_pattern);
    QString comment_end_pattern = "\\*/"; // 缁撴潫浣嶇疆
    comment_end = QRegularExpression(comment_end_pattern);

    // 5.娣诲姞寮曞彿楂樹寒瑙勫垯
    quotation_format.setForeground(QColor(settings.value("QUATATION", "#00ffff").toString())); // 寮曞彿鍐呭棰滆��?(cyan)
    QString quotation_pattern = "\".*\"";
    rule.pattern = QRegularExpression(quotation_pattern);
    rule.format = quotation_format;
    highlighterrules.push_back(rule);

    // 6.娣诲姞鍑芥暟楂樹寒鏍煎紡
    function_format.setForeground(QColor(settings.value("FUNCTION", "#00ffff").toString())); // 鍑芥暟瀛椾綋棰滆壊璁剧疆涓篸arkGreen
    function_format.setFontWeight(QFont::Bold);                                              // 鍑芥暟瀛椾綋鏍煎紡璁剧疆涓築old
    QString function_pattern = "\\b[a-zA-Z0-9_]+(?=\\()";                                    // 鍑芥暟鍚嶅彲浠ユ槸澶у皬鍐欒嫳鏂囧瓧绗︺€佹暟瀛椼€佷笅鍒掔嚎锛屽叾涓紝(?=\\()琛ㄧず鍚庨潰蹇呴』璺熺潃涓€涓乏鎷彿锛屼絾鏄繖涓乏鎷彿涓嶄細琚尮閰嶅埌
    rule.pattern = QRegularExpression(function_pattern);
    rule.format = function_format;
    highlighterrules.push_back(rule);

    // 7.娣诲姞鍒嗘敮楂樹寒鏍煎紡
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

    // 8.娣诲姞杈撳叆杈撳嚭楂樹寒鏍煎��?
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

    // 9.娣诲姞鍗曡娉ㄩ噴楂樹寒瑙勫��?
    singleLine_comment_format.setForeground(QColor(settings.value("SIGNLE_LINE_COMMENT", "#00ffff").toString()));
    singleLine_comment_format.setFontWeight(QFont::Bold);
    QString singleLine_comment_pattern = "//[^\n]*"; // 鍗曡娉ㄩ噴璇嗗埆鏍煎紡涓鸿窡鍦?//鍚庯紝浣嗕笉鍖呮嫭鎹㈣绗︼紝涓斾笉闇€瑕侀棿闅旂
    rule.pattern = QRegularExpression(singleLine_comment_pattern);
    rule.format = singleLine_comment_format;
    highlighterrules.push_back(rule);

    // 澶氳娉ㄩ噴鏍煎��?
    multiLine_comment_format.setForeground(QColor(settings.value("MULITLINE_COMMENT", "#00ffff").toString()));
    multiLine_comment_format.setFontWeight(QFont::Bold);

    settings.endGroup();
}

void HighLighter::highlightBlock(const QString &text)
{ // 搴旂敤楂樹寒瑙勫��?
    foreach (const HighLighterRule &rule, highlighterrules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text); // 鍦ㄦ暣涓猼ext鏂囨湰涓尮閰嶅綋鍓峳ule鐨刾attern
        while (matchIterator.hasNext())
        { // 楂樹寒鏁翠釜鏂囨湰涓尮閰嶅埌鐨勫瓧��??
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format); //(鍖归厤鍒扮殑璧峰浣嶇疆锛屾枃鏈潡闀垮害锛岄珮浜鍒欐牸寮?)
        }
    }

    // 澶勭悊澶氳娉ㄩ噴锛岀敱浜庡琛屾敞閲婁紭鍏堢骇鏈€楂橈紝鎵€浠ユ渶鍚庡��??
    setCurrentBlockState(0);
    int start = 0;
    if (previousBlockState() != 1)
    {                                        // 涓婁竴涓枃鏈潡涓嶆槸澶氳娉ㄩ噴鐨勬枃鏈唴瀹癸紝濡傛灉鏄紝鏂囨湰鐘舵€佸簲璁剧疆��??1
        start = text.indexOf(comment_start); // 鍏堝畾浣嶅埌绗竴涓琛屾敞閲婄殑璧峰瀛楃锛屾壘鍒拌繑鍥炰綅缃紝娌℃壘鍒拌繑鍥?-1
    }
    while (start >= 0)
    {
        QRegularExpressionMatch match = comment_end.match(text, start); // 浠庡綋鍓嶈捣濮嬪瓧绗﹀尮閰嶇涓€涓粨鏉熷瓧绗︼紝鍗冲綋鍓嶅琛屾敞閲婄殑鎴瀛楃��?
        int end = match.capturedStart();                                // match瀵瑰簲comment_end锛屾鏃舵壘鐨勬槸浠庡綋鍓峴tart(澶氳娉ㄩ噴璧峰瀛楃��?)寮€濮嬪尮閰嶇殑绗竴涓粨鏉熷瓧绗?
        int length = 0;
        if (end == -1)
        { // 鎵句笉鍒扮粨鏉熷瓧绗︼紝璇存槑鏄渶鍚庝竴涓琛屾敞閲婏紝涓轰簡璁╀笅涓€涓枃鏈潡鐭ラ亾涔嬪墠鏄琛屾敞閲婃ā鍧楋紝鎵€浠ヨ鐘舵€佷负1锛岀劧鍚庣粨鏉熶綅缃笌鏂囨湰缁撴潫浣嶇疆鐩稿悓
            setCurrentBlockState(1);
            length = text.length() - start;
        }
        else
        {
            length = end - start + match.capturedLength(); // 闇€瑕侀珮浜殑鏂囨湰闀垮害��?? 缁撴潫瀛楃涓嬫爣-璧峰瀛楃涓嬫爣+鍖归厤鍒扮殑鏂囨��?(*/)鐨勯暱搴?
        }
        setFormat(start, length, multiLine_comment_format);
        start = text.indexOf(comment_start, start + length); // 浠庡綋鍓峴tart+length寮€濮嬪尮閰嶄笅涓€涓猻tart
    }
}

void setUpAssociateList()
{
    associateList << "char"
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
                  << "include \"\""
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
        // 获取当前选中的项
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
