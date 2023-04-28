
#include "codedocedit.h"
#include"mainwindow.h"

#include<QPainter>
#include<QTextBlock>

/*
 * 编辑器会在区域左侧的区域显示行号进行编辑。
 * 编辑器将突出显示包含光标的行。
 */
CodeDocEdit::CodeDocEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    //属性：blockCount  保存了文本块的数量
    connect(this,&CodeDocEdit::blockCountChanged,this,&CodeDocEdit::updateLineNumerAreaWidth);

    //这个信号，是QPlainTextEdit变更时发出的信息，就是为了给 另外的 行号、断点的小部件使用
    //光标闪烁时，就有updateRequest信号
    connect(this,&CodeDocEdit::updateRequest,this,&CodeDocEdit::updateLineNumberArea);

    //光标位置变更的信号
    connect(this,&CodeDocEdit::cursorPositionChanged,this,&CodeDocEdit::highlightCurrentLine);

    connect(lineNumberArea,SIGNAL(clicked(QMouseEvent*)),this,SLOT(setBreakPoint(QMouseEvent*)));

    updateLineNumerAreaWidth(0);
    highlightCurrentLine();
    resize(300,200);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //    qDebug() << this->viewport() << this->rect();

    // 鍒濆鍖栬仈鎯冲垪琛?
    setUpAssociateList();
    associateWidget = new AssociateListWidget(this,(CodeEdit*)this->parent());
    associateWidget->hide();
    associateWidget->setMaximumHeight(fontMetrics().height() * 5);
    associateState = AssociateState::Hide;
}


//绘制行号
void CodeDocEdit::lineNumberAreaPainterEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(),Qt::lightGray);

    //获取当前页面的第一条文本块的数量，非整个文档的第一条文本块
    QTextBlock block = firstVisibleBlock();

    //获取block的文本块序号
    int blockNumber = block.blockNumber();

    //QPlainTextEdit::blockBoundingGeometry(block)：获取文本块block的边界矩形框
    //QPlainTextEdit::contentOffset() 文本水平滚动，第一块部分滚出屏幕，会有偏移
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    //while 和 if  保证了文本的矩形在绘制矩形的区域内
    while(block.isValid() && top <= event->rect().bottom()) {
        if(block.isVisible() && bottom >= event->rect().top()){
            QString number = QString::number(blockNumber + 1);
            auto breakPointSet = MainWindow::debugInfo->value(file->file_project);
            auto list = breakPointSet->values(file->file_name);
            for(auto it:list)
            {
                if(it==blockNumber+1)
                {
                    painter.save();
                    painter.setPen(QColor("#FFDAB9"));
                    painter.setBrush(QBrush(QColor("#FFDAB9")));
                    painter.drawEllipse(0,top,fontMetrics().height(),fontMetrics().height());
                    painter.restore();
                    break;
                }
            }
            painter.drawText(0,top,lineNumberArea->width(),fontMetrics().height(),Qt::AlignRight,number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

//根据blockCount，计算所需要的空间的宽度
int CodeDocEdit::lineNumberAreaWidth()
{
    int digits = 1;
    //blockCount()  : 此属性保存文档中文本块的数量。默认情况下，在空文档中，此属性的值为1。
    int max = qMax(1,blockCount());
    while(max>=10){
        max /= 10;
        ++digits;
    }

    if(digits==1)
        digits=2;

    //字体的度量  QWidget::fontMetrics()
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return  space;
}

//在左侧设置行号区域
void CodeDocEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(),cr.top(),lineNumberAreaWidth(),cr.height()));
}

// 更新行号部分的宽度，并且设置视口部件的margin
void CodeDocEdit::updateLineNumerAreaWidth(int /*newBlockCount*/)
{
    // 设置视口部件的margin，left部分
    setViewportMargins(lineNumberAreaWidth(),0,0,0);
}


//高亮光标所在行，使用QPlainTextEdit::setExtraSelections
//设计富文本格式的一些知识
void CodeDocEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if(!isReadOnly()){
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor("#FFF5EE");

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection,true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

void CodeDocEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    // dy不为0，有上下滚动， lineNumberArea 跟着滚动
    // dy为0 ，有区域内的更新，lineNumberArea更新部分内容
    if(dy)
        lineNumberArea->scroll(0,dy);
    else
        lineNumberArea->update (0, rect.y(), lineNumberArea->width(), rect.height());

    // qDebug() << rect << viewport()->rect() << rect.contains(viewport()->rect());

    // 变动了整个area页面
    if(rect.contains(viewport()->rect()))
        updateLineNumerAreaWidth(0);
}

void CodeDocEdit::setBreakPoint(QMouseEvent* event)
{
    //获取当前页面的第一条文本块的数量，非整个文档的第一条文本块
    QTextBlock block = firstVisibleBlock();

    //获取block的文本块序号
    int blockNumber = block.blockNumber();

    //QPlainTextEdit::blockBoundingGeometry(block)：获取文本块block的边界矩形框
    //QPlainTextEdit::contentOffset() 文本水平滚动，第一块部分滚出屏幕，会有偏移
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    //while 和 if  保证了文本的矩形在绘制矩形的区域内
    while(block.isValid() && top <= event->pos().y()) {
        if(block.isVisible() && bottom >= event->pos().y()){
            auto breakPointSet = MainWindow::debugInfo->value(file->file_project);
            auto list = breakPointSet->values(file->file_name);
            bool flag = true;
            for(auto it:list)
            {
                if(it == blockNumber+1)
                {
                    breakPointSet->remove(file->file_name,blockNumber+1);
                    flag = false;
                    break;
                }
            }
            if(flag)
            {
                breakPointSet->insert(file->file_name,blockNumber+1);
            }
            break;
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
    this->updateGeometry();
}

void LineNumberArea::mouseReleaseEvent(QMouseEvent * event)
{
    emit clicked(event);
    //return QWidget::mouseReleaseEvent(event);
}

void LineNumberArea::mousePressEvent(QMouseEvent * event)
{

}

void LineNumberArea::mouseMoveEvent(QMouseEvent * event)
{

}

void CodeDocEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (associateState == AssociateState::Showing)
        {
            associateWidget->setFocus();
            QKeyEvent *newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
            //QApplication::sendEvent(associateWidget, newEvent); // 发送模拟事???
            // 获取当前选中的项
            QListWidgetItem *item = associateWidget->currentItem();
            if (item)
            {
                QString text = associateWidget->currentItem()->text();
                QString word = this->getWordCursor();
                for (int i = 0; i < word.length(); i++)
                {
                    this->textCursor().deletePreviousChar();
                }
                this->insertPlainText(text);
            }
            associateWidget->hide();
            associateState = AssociateState::Hide;
            this->setFocus();
            return;                                             // 不调用基类的函数，防止移动光???
        }
        else{
            int preCharIndex = this->textCursor().position() - 1;
            QChar preChar = this->document()->characterAt(preCharIndex);
            if (preChar == '{')
            {
                QTextCursor cursor = this->textCursor();
                int startPos = cursor.block().position();
                int spaceCount = 0;
                while (this->document()->characterAt(startPos) == ' ')
                {
                    spaceCount++;
                    startPos++;
                }
                this->insertPlainText("\n");
                this->insertPlainText(QString(spaceCount + 4, ' '));
                this->insertPlainText("\n");
                this->insertPlainText(QString(spaceCount,' '));
                this->moveCursor(QTextCursor::Up);
                this->moveCursor(QTextCursor::EndOfLine);
            }else{
                QTextCursor cursor = this->textCursor();
                int startPos = cursor.block().position();
                int spaceCount = 0;
                while (this->document()->characterAt(startPos) == ' ')
                {
                    spaceCount++;
                    startPos++;
                }
                this->insertPlainText("\n");
                this->insertPlainText(QString(spaceCount, ' '));
            }
            return;
        }
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (associateState == AssociateState::Showing)
        {
            associateWidget->setFocus();
            QKeyEvent *newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
            //QApplication::sendEvent(associateWidget, newEvent); // 发送模拟事???

            if (associateWidget->currentRow() < associateWidget->count() - 1)
            {
                associateWidget->setCurrentRow(associateWidget->currentRow() + 1);
            }

            this->setFocus();
            return;                                             // 不调用基类的函数，防止移动光???
        }
    }
    else if (event->key() == Qt::Key_Up)
    {
        if (associateState == AssociateState::Showing)
        {
            associateWidget->setFocus();
            QKeyEvent *newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
            //QApplication::sendEvent(associateWidget, newEvent); // 发送模拟事???
            if (associateWidget->currentRow() > 0)
            {
                associateWidget->setCurrentRow(associateWidget->currentRow() - 1);
            }
            this->setFocus();
            return;                                             // 不调用基类的函数，防止移动光???
        }
    }
    else if(event->key()==Qt::Key_ParenLeft){
        this->insertPlainText("()");
        this->moveCursor(QTextCursor::PreviousCharacter);
        return;
    }
    else if(event->key() == Qt::Key_BracketLeft){
        this->insertPlainText("[]");
        this->moveCursor(QTextCursor::PreviousCharacter);
        return;
    }
    else if(event->key() == Qt::Key_BraceLeft){
        this->insertPlainText("{}");
        this->moveCursor(QTextCursor::PreviousCharacter);
        return;
    }
    else if(event->key()==Qt::Key_QuoteDbl){
        this->insertPlainText("\"\"");
        this->moveCursor(QTextCursor::PreviousCharacter);
        return;
    }
    else{
        //this->setFocus();
    }
    return QPlainTextEdit::keyPressEvent(event);
}

QString CodeDocEdit::getWordCursor()
{
    QTextCursor cursor = this->textCursor();
    cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
    int start = cursor.position();
    QString res = "";
    QChar ch = this->document()->characterAt(start);
    while (ch.isDigit() || ch.isLetter() || ch == '_' || ch == '#')
    {
        res.push_back(ch);
        ch = this->document()->characterAt(++start);
    }
    return res;
}

void CodeDocEdit::showAssociateWidget()
{
    if (associateState == AssociateState::Ignore)
        return; // 瀵逛簬鍏夋爣鍜屾枃鏈彉鍖栦笉鍋氫换浣曠浉搴旓紝閬垮厤闄峰叆姝诲惊????
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
            { // 濡傛灉褰撳墠杈撳叆瀛楃灞炰簬鑱旀兂琛ㄤ腑鐨勫瓧绗︿覆
                itemVec.push_back(keyword);
                differenceRecord[keyword] = associateWidget->letterDifference(keyword.toStdString(), word.toStdString());
                if (keyword.length() > maxSize)
                    maxSize = keyword.length(); // 鎵惧埌鑱旀兂鍒楄〃涓渶闀跨殑涓€涓紝濂借缃仈鎯冲垪琛ㄥ搴?
            }
        }

        if (itemVec.size() > 0)
        { // 鏈夊尮閰嶅瓧????
            // 鎸夊樊寮傚害浠庡皬鍒板ぇ鎺掞紝鏈€鍖归厤鐨勫湪鏈€鍓嶉潰
            sort(itemVec.begin(), itemVec.end(), [&](const QString &s1, const QString &s2) -> bool
                 { return differenceRecord[s1] < differenceRecord[s2]; });
            foreach (const QString &item, itemVec)
            {
                associateWidget->addItem(new QListWidgetItem(item));
            }

            int x = this->getAssociateWidgetX();
            int y = this->cursorRect().y() + fontMetrics().height();

            associateWidget->move(x, y); // 璁剧疆鑱旀兂鍒楄〃鐨勪綅????
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
    }
    this->setFocus();
}

int CodeDocEdit::getAssociateWidgetX()
{
    QTextCursor cursor = this->textCursor();
    int pos = cursor.position() - 1;
    int originalPos = pos + 1;
    QChar ch = this->document()->characterAt(pos);
    while ((ch.isDigit() || ch.isLetter() || ch == '_' || ch == '#') && pos > 0)
    {
        ch = this->document()->characterAt(pos--);
    }
    pos++;
    associateState = AssociateState::Ignore;
    cursor.setPosition(pos);
    this->setTextCursor(cursor);
    int x = this->cursorRect().x() + 2 * fontMetrics().lineWidth();
    cursor.setPosition(originalPos);
    this->setTextCursor(cursor);
    associateState = AssociateState::Hide;
    return x;
}

AssociateListWidget::AssociateListWidget(CodeDocEdit *edit, QWidget *parent) : QListWidget(parent),
    edit(edit)
{
    this->setWindowFlags(Qt::CustomizeWindowHint);
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
