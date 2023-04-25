#ifndef WELCOMEWIND_H
#define WELCOMEWIND_H

#include <QWidget>

namespace Ui {
class WelcomeWind;
}

class WelcomeWind : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWind(QWidget *parent = nullptr);
    ~WelcomeWind();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::WelcomeWind *ui;
};

#endif // WELCOMEWIND_H
