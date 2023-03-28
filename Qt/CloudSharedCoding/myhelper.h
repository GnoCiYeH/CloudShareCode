#ifndef MYHELPER_H
#define MYHELPER_H

#include <QtCore>
#include <QtGui>

#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))

#include <QtWidgets>

#endif

class myHelper : public QObject {

public:
    static void autoRunWithSystem(bool ifAutoRun, QString appName, QString appPath) {
        QSettings *reg = new QSettings(
                "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

        if (ifAutoRun) {
            reg->setValue(appName, appPath);
        } else {
            reg->setValue(appName, "");
        }
    }

    //设置编码为UTF8
    static void setTextCode(const QString sForName = "UTF-8") {
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
        QTextCodec *codec = QTextCodec::codecForName(sForName);
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
#endif
    }


    //设置指定样式
    static void setStyle(const QString &qssFile) {
        QFile file(qssFile);
        if (file.open(QFile::ReadOnly)) {
            QString qss = QLatin1String(file.readAll());
            qApp->setStyleSheet(qss);
            QString PaletteColor = qss.mid(20, 7);
            qApp->setPalette(QPalette(QColor(PaletteColor)));
            file.close();
        }
    }

    //加载中文字符
    static void setChinese() {
        QTranslator *translator = new QTranslator(qApp);
        translator->load(":/image/qt_zh_CN.qm");
        qApp->installTranslator(translator);
    }

    //判断是否是IP地址
    static bool isIP(const QString sIP) {
        QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
        return RegExp.exactMatch(sIP);
    }

    //延时
    static void sleep(int sec) {
        QTime dieTime = QTime::currentTime().addMSecs(sec);

        while (QTime::currentTime() < dieTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    //延时
    static int sleep1(int command, int sec, int *state) {
        int ret = 0;
        QTime dieTime = QTime::currentTime().addMSecs(sec);

        while (QTime::currentTime() < dieTime) {
            if (((0xC7 != command && 0xC1 != command) && (*state == 2)) ||
                ((0xC7 == command || 0xC1 == command) && (*state == 3))) {
                return 1;
            }
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        ret = 2;
        return ret;
    }

    //窗体居中显示
    static void moveFormToCenter(QWidget *frm) {
        int frmX = frm->width();
        int frmY = frm->height();

        QDesktopWidget dwt;

        int deskWidth = dwt.availableGeometry().width();
        int deskHeight = dwt.availableGeometry().height();

        QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
        frm->move(movePoint);
    }
};

#endif // MYHELPER_H
