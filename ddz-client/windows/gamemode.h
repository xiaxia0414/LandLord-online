#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <QWidget>

namespace Ui {
class GameMode;
}

class GameMode : public QWidget
{
    Q_OBJECT

public:
    explicit GameMode(QWidget *parent = nullptr);
    ~GameMode();

private:
    Ui::GameMode *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // GAMEMODE_H
