#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include<QTextEdit>

namespace Ui {
class CodeEdit;
}

class CodeEdit : public QWidget
{
    Q_OBJECT

public:
    explicit CodeEdit(QWidget *parent = nullptr);
    ~CodeEdit();

private:
    Ui::CodeEdit *ui;

    bool eventFilter(QObject *obj, QEvent *e)override;

    QString buffer;

    void format();
};

#endif // CODEEDIT_H
