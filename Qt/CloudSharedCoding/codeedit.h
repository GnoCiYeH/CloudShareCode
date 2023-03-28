#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>

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
};

#endif // CODEEDIT_H
