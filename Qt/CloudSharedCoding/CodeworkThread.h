
#ifndef RUNTHREAD_H
#define RUNTHREAD_H

#include <QThread>
#include<QProcess>


class RunThread : public QThread
{
    Q_OBJECT
public:
    explicit RunThread(QString path,QObject *parent = nullptr);
    ~RunThread();

signals:
    void buildInfo(QString);
    void stdOut(QString);
private:
    QString workPath;
    QString proName;
protected:
    void run()override;
    QProcess* cmakeProcess;
    QProcess* makefileProcess;
    QProcess* findExecProcess;
    QProcess* runProcess;
};

class DebugThread : public QThread
{
    Q_OBJECT
public:
    explicit DebugThread(QString path,QObject *parent = nullptr);
    ~DebugThread();

signals:
    void buildInfo(QString);
    void debugInfo(QString);
private:
    QString workPath;
    QString proName;
protected:
    void run()override;
    QProcess* cmakeProcess;
    QProcess* makefileProcess;
    QProcess* findExecProcess;
    QProcess* debugProcess;
};

#endif // RUNTHREAD_H
