
#ifndef RUNTHREAD_H
#define RUNTHREAD_H

#include <QThread>
#include<QProcess>


class RunThread : public QObject
{
    Q_OBJECT
public:
    explicit RunThread(QString path,QObject *parent = nullptr);
    ~RunThread();
    void run();
    void writeStdin(QString);
signals:
    void buildInfo(QString);
    void buildFinish();
    void runFinish(int);
    void stdOut(QString);
private:
    QString workPath;
    QString proName;

    void make();
    void exe();
protected:
    QProcess* cmakeProcess;
    QProcess* makefileProcess;
    QProcess* runProcess;
};

class DebugThread : public QObject
{
    Q_OBJECT
public:
    explicit DebugThread(QString path,QObject *parent = nullptr);
    ~DebugThread();
    void run();
    void writeGdbOrStdin(QString);
signals:
    void buildInfo(QString);
    void buildFinish();
    void debugInfo(QString);
    void debugFinish(int);
private:
    QString workPath;
    QString proName;
    void make();
    void exe();
protected:
    QProcess* cmakeProcess;
    QProcess* makefileProcess;
    QProcess* debugProcess;
};

#endif // RUNTHREAD_H
