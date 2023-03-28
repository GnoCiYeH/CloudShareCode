#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void close();
    void openCloudProj();

private:
    Ui::MainWindow *ui;

    QString userId;
    QString userName;

    bool isLogin = false;

    QTcpSocket* socket;
    QTcpSocket* editSocket;
    QString serverIP = "127.0.0.1";
    quint16 mainPort = 9098;
    quint16 editPort = 9097;

private:
    bool Login();
};

#endif // MAINWINDOW_H
