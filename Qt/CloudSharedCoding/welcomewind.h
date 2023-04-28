#ifndef WELCOMEWIND_H
#define WELCOMEWIND_H

#include <QListWidgetItem>
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

    void update(QQueue<QString> historyQueue);

    void addHistoryListItem(QString historyInfo);
signals:
    void historyProjectemit(QString path);

    void openProj();

    void newProj();

    void registerRequest();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::WelcomeWind *ui;
};

#endif // WELCOMEWIND_H
