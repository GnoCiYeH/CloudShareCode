
#ifndef CODEDOCEDIT_H
#define CODEDOCEDIT_H

#include <QPlainTextEdit>
#include<QTextBlock>
#include<QSet>
#include<QListWidget>
#include<string>
#include"InfoType.h"
using namespace std;

class AssociateListWidget;
static void setUpAssociateList();

class CodeDocEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeDocEdit(QWidget *parent = nullptr);

    void setFile(std::shared_ptr<FileInfo> file){this->file = file;}

    void lineNumberAreaPainterEvent(QPaintEvent *event);

    int lineNumberAreaWidth();// 更新行号部分的宽度，并且设置视口部件的margin

    QString getWordCursor();              // 鑾峰彇褰撳墠鍏夋爣鎵€鍦ㄤ綅缃殑瀛楃涓?

    void showAssociateWidget(); // 灞曠ず鑱旀兂鍒楄〃

protected:
    void resizeEvent(QResizeEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateLineNumerAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect,int dy);
    void setBreakPoint(QMouseEvent*);

private:
    QWidget *lineNumberArea;

    std::shared_ptr<FileInfo> file;

    int associateState;                   // 鑱旀兂鐘舵€?
    AssociateListWidget *associateWidget; // 鑱旀兂琛?
    int getAssociateWidgetX();
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

class AssociateListWidget : public QListWidget
{
public:
    AssociateListWidget(CodeDocEdit *edit, QWidget *parent = 0);
    static int letterDifference(const string source, const string target); // 涓や釜瀛楃涓茬殑宸紓搴?
    static int strToInt(string str);

private:
    QPlainTextEdit *p;
    QColor backgroundColor; // 鑱旀兂鍒楄〃鑳屾櫙鑹?
    QColor highlightColor;
    CodeDocEdit *edit;
};

enum AssociateState
{
    Ignore = 0,
    Showing = 1,
    Hide = 2
};

#endif // CODEDOCEDIT_H
