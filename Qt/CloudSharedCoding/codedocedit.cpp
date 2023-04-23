
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
        QColor lineColor = QColor(Qt::yellow).lighter(160);

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


