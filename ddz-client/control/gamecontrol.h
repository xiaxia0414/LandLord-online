#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "robot.h"
#include "user.h"
#include "QRandomGenerator"
#include "QTimer"
#include "playhand.h"
struct BetRecord
{
    BetRecord()
    {
        reset();
    }
    void reset()
    {
        player = nullptr;
        bet = 0;
        times = 0;
    }
    Player* player;
    int bet;
    int times;
};

class GameControl : public QObject
{
    Q_OBJECT
public:
    enum GameState{
        DispatchCard,
        CallingLord,
        PlayingHand
    };
    enum PlayerStatus
    {
        ThinkingForCallLord,
        ThinkingForPlayHand,
        Winning
    };

    explicit GameControl(QObject *parent = nullptr);

    void palyerInit();

    Robot* getLeftRobot();
    Robot* getRightRobot();
    User* getUserPlayer();

    void setCurrentPlayer(Player* player);
    Player* currentPlayer();
    //得到出牌玩家和打出的的牌
    Player* pendPlayer();
    Cards pendCards();

    void initAllCards();
    Card takeOneCard();
    Cards getSurplusCards();
    void resetCarData();

    void startLordCard();
    void becomeLord(Player* player,int bet);
    void clearPlayerScore();
    void onGarbBet(Player* player,int point);
    void onPlayHand(Player* player,Cards& card);

    int getPlayerMaxBet();

    void setCurrentPlayer(int index);


signals:

    void playerStausChaged(Player* player,PlayerStatus status);

    void notifyGrabLordBet(Player* player,int bet,bool flag);

    void gameSatusChanged(GameState state);

    void notifyPlayHand(Player* player,Cards& card);

    void pendingInfo(Player* player,Cards& card);


private:
    Robot* m_robotLeft;
    Robot* m_robotRight;
    User* m_user;
    Player* m_curPlayer;
    Player* m_pendPlayer;
    Cards m_pendCards;
    Cards m_allCards;
    BetRecord m_betRecord;
    int m_curBet;
signals:

};

#endif // GAMECONTROL_H
