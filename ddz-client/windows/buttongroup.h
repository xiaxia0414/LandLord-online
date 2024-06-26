#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    enum Panel{Start,PlayCard,passOrPlay,CallLord,nullPage};
    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();

    void initButton();
    //处理page的切换
    void selectPanel(Panel type,int bet=0);
signals:
    void startGame();
    void playHand();
    void pass();
    void betPoint(int bet);
private slots:
    void on_oneScore_clicked();

    void on_giveUp_clicked();

    void on_twoScore_clicked();

    void on_threeScore_clicked();

    void on_pass_clicked();

    void on_play_clicked();

    void on_playCard_clicked();

    void on_start_clicked();

private:
    Ui::ButtonGroup *ui;
    bool isSended=false;
};

#endif // BUTTONGROUP_H
