#ifndef ROBOTPLAYHAND_H
#define ROBOTPLAYHAND_H

#include <QThread>
#include <QRunnable>
#include <QObject>
#include "player.h"

class RobotPlayHand : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RobotPlayHand(Player* player,QObject *parent = nullptr);
protected:
    void run() override;
private:
    Player* m_player;
signals:
};

#endif // ROBOTPLAYHAND_H
