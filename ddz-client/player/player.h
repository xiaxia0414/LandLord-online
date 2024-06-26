#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "cards.h"
class Player : public QObject
{
    Q_OBJECT
public:
    enum Role{Lord=1,Farmar=2};
    enum Sex{Man,Woman};
    enum Direction{Left,Right};
    enum Type{Robot,User,Unkonw};

    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name ,QObject *parent = nullptr);

    void setScore(int score);
    int  score();

    void setName(QString name);
    QString name();

    void setRole(Role role);
    Role role();

    void setSex(Sex sex);
    Sex sex();

    void setDirection(Direction direction);
    Direction direction();

    void setType(Type type);
    Type type();

    void setWin(bool isWin);
    bool isWin();

    //上家，下家
    void setPrevPlayer(Player *player);
    void setNextPlayer(Player *player);
    Player* prevPlayer();
    Player* nextPlayer();

    //叫地主，抢地主
    void grabLordBet(int point);
    //存扑克牌（发牌时）
    void storeDispatchCard(Card &card);
    void storeDispatchCards(Cards &cards);


    Cards cards();

    void clearCards();

    void playHand(Cards& cards);

    //设置出牌的玩家已经待处理的扑克牌

    void setPendingInfo(Player* player, Cards cards);
    Player* pendPlayer();
    Cards pendCards();

    virtual void prepareCallLord();
    virtual void preparePlayHand();
    virtual void thinkCallLord();
    virtual void thinkPlayHand();





protected:
    int m_score;
    QString m_name;
    Role m_role = Lord;
    Sex m_sex;
    Direction m_direction;
    Type m_type;
    bool m_isWin;
    Player* m_next;
    Player* m_prev;
    Cards m_cards;

    Cards m_pendCards=Cards();
    Player* m_pendPlayer=nullptr;
signals:
   void notifyGrabLordBet(Player* player,int point);
   void notifyPalyHand(Player* player,Cards &cards);
   void notifyPickCards(Player* player,Cards &cards);

};

#endif // PLAYER_H
