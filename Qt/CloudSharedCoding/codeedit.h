#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include<QTextEdit>
#include<QSyntaxHighlighter>
#include<QTextCharFormat>
#include<QRegularExpression>
#include<QThread>
#include<QMutex>

namespace Ui {
class CodeEdit;
}

class EditWorkThread;
class CodeEdit : public QWidget
{
    Q_OBJECT

public: friend class EditWorkThread;
public:
    explicit CodeEdit(QWidget *parent = nullptr);
    ~CodeEdit() override;

signals:
    void deleteInfo(int,int);

public slots:
    void docChange(int,int,int);

private:
    Ui::CodeEdit *ui;

    int lastPosition;
    QString buffer;
    QTextDocument * document;
    EditWorkThread* thread;

    QMutex mutex;
};

class EditWorkThread : public QThread
{
    Q_OBJECT

public:
    explicit EditWorkThread(CodeEdit*);
    ~EditWorkThread(){}

private slots:
    void deleteInfo(int,int);

private:
    CodeEdit * codeEdit;
protected:
    void run() override;
};

class HighLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit HighLighter(QTextDocument* parent = nullptr);

private:
    static QStringList keyWords;

    struct HighLightRule{
        QTextCharFormat format;
        QRegularExpression exp;
    };
    QVector<HighLightRule> rules;
    QTextCharFormat keyWordFormat;

protected:
    void highlightBlock(const QString &text) override;
};

#endif // CODEEDIT_H
