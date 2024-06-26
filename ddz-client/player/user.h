#ifndef USER_H
#define USER_H

#include "player.h"
#include <QObject>

class User : public Player
{
    Q_OBJECT
public:
    using Player::Player;
    explicit User(QObject *parent = nullptr);
    void prepareCallLord() override;
    void preparePlayHand() override;
signals:
     void startCountDown();
};

#endif // USER_H
