
#ifndef CODEDOCEDIT_H
#define CODEDOCEDIT_H

#include <QPlainTextEdit>
#include<QTextBlock>
#include<QSet>
#include"InfoType.h"



class CodeDocEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeDocEdit(QWidget *parent = nullptr);

    void setFile(std::shared_ptr<FileInfo> file){this->file = file;}

    void lineNumberAreaPainterEvent(QPaintEvent *event);

    int lineNumberAreaWidth();// 更新行号部分的宽度，并且设置视口部件的margin

protected:
    void resizeEvent(QResizeEvent *e) override;

private slots:
    void updateLineNumerAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect,int dy);
    void setBreakPoint(QMouseEvent*);

private:
    QWidget *lineNumberArea;

    std::shared_ptr<FileInfo> file;
};

class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    LineNumberArea(CodeDocEdit *editor) : QWidget(editor),codeEditor(editor){}

signals:
    void clicked(QMouseEvent * event);
protected:
    // 控件的重绘工作，放到了codeEditor里面
    // 主要是因为需要取得文本块的序号和视口的大小，写在codeEditor里，参数调用会更加方便
    void paintEvent(QPaintEvent *event) override{
        codeEditor->lineNumberAreaPainterEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent * event)override;
    void mousePressEvent(QMouseEvent * event)override;
    void mouseMoveEvent(QMouseEvent * event)override;

private:
    CodeDocEdit *codeEditor;
};

#endif // CODEDOCEDIT_H
