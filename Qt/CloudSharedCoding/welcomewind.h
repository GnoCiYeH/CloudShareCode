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

private:
    Ui::WelcomeWind *ui;
};

#endif // WELCOMEWIND_H
