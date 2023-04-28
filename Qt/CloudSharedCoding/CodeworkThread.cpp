
#include "CodeworkThread.h"
#include<QDebug>
RunThread::RunThread(QString path,QObject *parent)
    : QObject{parent}
{
    workPath = path;
    proName = path.mid(path.lastIndexOf("/")+1);
    cmakeProcess = new QProcess(this);
    makefileProcess = new QProcess(this);
    runProcess = new QProcess(this);

    connect(cmakeProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = cmakeProcess->readAllStandardOutput();
        emit buildInfo(QString(data));
    });

    connect(cmakeProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = cmakeProcess->readAllStandardError();
        emit buildInfo(QString(data));
    });

    connect(cmakeProcess,&QProcess::finished,this,[=](){
        make();
    });

    connect(makefileProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = makefileProcess->readAllStandardOutput();
        emit buildInfo(QString(data));
    });

    connect(makefileProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = makefileProcess->readAllStandardError();
        emit buildInfo(QString(data));
    });

    connect(makefileProcess,&QProcess::finished,this,[=](){
        emit buildFinish();
        exe();
    });

    connect(runProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = runProcess->readAllStandardOutput();
        emit stdOut(QString(data));
    });

    connect(runProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = runProcess->readAllStandardError();
        emit stdOut(QString(data));
    });

    connect(runProcess,&QProcess::finished,this,[=](int exitCode){
        emit runFinish(exitCode);
    });
}

RunThread::~RunThread()
{
    if(runProcess->state()==QProcess::ProcessState::Running)
    {
        runProcess->kill();
    }

    cmakeProcess->deleteLater();
    makefileProcess->deleteLater();
    runProcess->deleteLater();
}

void RunThread::make()
{
    //make makefile
    makefileProcess->setProgram("mingw32-make");
    makefileProcess->setNativeArguments("-C " + workPath + "/build");
    makefileProcess->start();
}

void RunThread::exe()
{
    //run file
    runProcess->setProgram(workPath+"\\build\\bin\\"+proName+".exe");
    runProcess->start();
    runProcess->waitForStarted();
    if(runProcess->state()==QProcess::NotRunning)
    {
        emit runFinish(-1);
    }
}

void RunThread::run()
{

    //Cmake
    cmakeProcess->setProgram("cmake");
    cmakeProcess->setNativeArguments(workPath+" -B "+ workPath +"/build -G \"Unix Makefiles\"");
    cmakeProcess->start();
}

void RunThread::writeStdin(QString data)
{
    this->runProcess->write(data.toUtf8());
}

DebugThread::DebugThread(QString path,QObject *parent)
    : QObject{parent}
{
    workPath = path;
    proName = path.mid(path.lastIndexOf("/")+1);
    cmakeProcess = new QProcess(this);
    makefileProcess = new QProcess(this);
    debugProcess = new QProcess(this);

    connect(cmakeProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = cmakeProcess->readAllStandardOutput();
        emit buildInfo(QString(data));
    });

    connect(cmakeProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = cmakeProcess->readAllStandardError();
        emit buildInfo(QString(data));
    });

    connect(cmakeProcess,&QProcess::finished,this,[=](){
        make();
    });

    connect(makefileProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = makefileProcess->readAllStandardOutput();
        emit buildInfo(QString(data));
    });

    connect(makefileProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = makefileProcess->readAllStandardError();
        emit buildInfo(QString(data));
    });

    connect(makefileProcess,&QProcess::finished,this,[=](){
        exe();
    });

    connect(debugProcess,&QProcess::readyReadStandardOutput,this,[=](){
        QByteArray data = debugProcess->readAllStandardOutput();
        emit debugInfo(QString(data));
    });

    connect(debugProcess,&QProcess::readyReadStandardError,this,[=](){
        QByteArray data = debugProcess->readAllStandardError();
        emit debugInfo(QString(data));
    });

    connect(debugProcess,&QProcess::finished,this,[=](int exitCode){
        emit debugFinish(exitCode);
    });
}

DebugThread::~DebugThread()
{
    if(debugProcess->state()==QProcess::ProcessState::Running)
    {
        debugProcess->kill();
    }

    cmakeProcess->deleteLater();
    makefileProcess->deleteLater();
    debugProcess->deleteLater();
}

void DebugThread::make()
{
    //make makefile
    makefileProcess->setProgram("mingw32-make");
    makefileProcess->setNativeArguments("-C " + workPath + "/build");
    makefileProcess->start();
}

void DebugThread::exe()
{
    //run
    debugProcess->setProgram("gdb");
    debugProcess->setNativeArguments(" -silent");
    debugProcess->start();
    debugProcess->waitForStarted();
    emit buildFinish();
}

void DebugThread::run()
{
    //Cmake
    cmakeProcess->setProgram("cmake");
    cmakeProcess->setNativeArguments(workPath +" -B "+ workPath +"/build -G \"Unix Makefiles\"");
    cmakeProcess->start();
}

void DebugThread::writeGdbOrStdin(QString data)
{
    this->debugProcess->write(data.toUtf8());
}

