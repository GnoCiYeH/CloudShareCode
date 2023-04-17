#ifndef CODETHEMEITEM_H
#define CODETHEMEITEM_H

#include <QWidget>
#include"InfoType.h"

namespace Ui {
class CodeThemeItem;
}

class CodeThemeItem : public QWidget
{
    Q_OBJECT

public:
    explicit CodeThemeItem(QString title,QColor color,HightlightType type,QWidget *parent = nullptr);
    ~CodeThemeItem();

    void setColor(QColor color);

signals:
    void colorChanged(HightlightType,QColor);

private slots:
    void on_pushButton_clicked();

private:
    Ui::CodeThemeItem *ui;

    QColor color;
    HightlightType type;
};

#endif // CODETHEMEITEM_H
