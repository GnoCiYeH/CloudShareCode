
#include "CodeworkThread.h"

RunThread::RunThread(QString path,QObject *parent)
    : QThread{parent}
{
    workPath = path;
    proName = path.mid(path.lastIndexOf("\\")+1);
}

RunThread::~RunThread()
{
    if(runProcess->state()==QProcess::ProcessState::Running)
    {
        runProcess->kill();
    }
}

void RunThread::run()
{
    cmakeProcess = new QProcess(this);
    makefileProcess = new QProcess(this);
    findExecProcess = new QProcess(this);
    runProcess = new QProcess(this);

    //Cmake
    cmakeProcess->setProgram("cmake");
    cmakeProcess->setNativeArguments("-B "+ workPath +"/build -G \"Unix Makefiles\"");
    cmakeProcess->start();
    cmakeProcess->waitForStarted();

    connect(cmakeProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = cmakeProcess->readAllStandardOutput();
        emit buildInfo(QString(data));
    });

    connect(cmakeProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = cmakeProcess->readAllStandardError();
        emit buildInfo(QString(data));
    });

    cmakeProcess->waitForFinished();

    //make makefile
    makefileProcess->setProgram("mingw32-make");
    makefileProcess->setNativeArguments("-C " + workPath + "/build");
    makefileProcess->start();
    makefileProcess->waitForStarted();

    connect(makefileProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = makefileProcess->readAllStandardOutput();
        emit buildInfo(QString(data));
    });

    connect(makefileProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = makefileProcess->readAllStandardError();
        emit buildInfo(QString(data));
    });

    makefileProcess->waitForFinished();

    //find file
    runProcess->setProgram(workPath+"\\build\\bin\\"+proName+".exe");
    runProcess->start();
    runProcess->waitForStarted();

    connect(runProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = runProcess->readAllStandardOutput();
        emit stdOut(QString(data));
    });

    connect(runProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = runProcess->readAllStandardError();
        emit stdOut(QString(data));
    });

    this->exec();
}

DebugThread::DebugThread(QString path,QObject *parent)
    : QThread{parent}
{
    workPath = path;
    proName = path.mid(path.lastIndexOf("\\")+1);
}

DebugThread::~DebugThread()
{
    if(debugProcess->state()==QProcess::ProcessState::Running)
    {
        debugProcess->kill();
    }
}

void DebugThread::run()
{
    cmakeProcess = new QProcess(this);
    makefileProcess = new QProcess(this);
    findExecProcess = new QProcess(this);
    debugProcess = new QProcess(this);

    //Cmake
    cmakeProcess->setProgram("cmake");
    cmakeProcess->setNativeArguments("-B "+ workPath +"/build -G \"Unix Makefiles\"");
    cmakeProcess->start();
    cmakeProcess->waitForStarted();

    connect(cmakeProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = cmakeProcess->readAllStandardOutput();
        emit buildInfo(QString(data));
    });

    connect(cmakeProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = cmakeProcess->readAllStandardError();
        emit buildInfo(QString(data));
    });

    cmakeProcess->waitForFinished();

    //make makefile
    makefileProcess->setProgram("mingw32-make");
    makefileProcess->setNativeArguments("-C " + workPath + "/build");
    makefileProcess->start();
    makefileProcess->waitForStarted();

    connect(makefileProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = makefileProcess->readAllStandardOutput();
        emit buildInfo(QString(data));
    });

    connect(makefileProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = makefileProcess->readAllStandardError();
        emit buildInfo(QString(data));
    });

    makefileProcess->waitForFinished();

    //run
    QString exePath = workPath+"\\build\\bin\\"+proName+".exe";
    debugProcess->setProgram("gdb");
    debugProcess->setNativeArguments(workPath+" -silent");
    debugProcess->start();
    debugProcess->waitForStarted();

    connect(debugProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = debugProcess->readAllStandardOutput();
        emit debugInfo(QString(data));
    });

    connect(debugProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = debugProcess->readAllStandardError();
        emit debugInfo(QString(data));
    });

    this->exec();
}

