#ifndef CLOUDSHAREDCODING_H
#define CLOUDSHAREDCODING_H

#include <QWidget>
#include<QMouseEvent>


#define PADDING 2
enum Direction { UP=0, DOWN=1, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE };
namespace Ui {
class CloudSharedCoding;
}

class CloudSharedCoding : public QWidget
{
    Q_OBJECT

public:
    explicit CloudSharedCoding(QWidget *parent = nullptr);
    ~CloudSharedCoding();

    void region(const QPoint &currentGlobalPoint);

private:
    Ui::CloudSharedCoding *ui;


protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void on_toolButton_3_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_clicked();

private:
    bool isMax = true;
    QPoint m_movePoint;  //鼠标的位置
    bool isLeftPressDown;  // 判断左键是否按下
    Direction dir;        // 窗口大小改变时，记录改变方向
};

#endif // CLOUDSHAREDCODING_H
