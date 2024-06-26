#ifndef ROBOTGRAPLORD_H
#define ROBOTGRAPLORD_H

#include <QThread>
#include <QRunnable>
#include <QObject>
#include "player.h"

class RobotGrapLord : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RobotGrapLord(Player* player,QObject *parent = nullptr);
protected:
    void run() override;
private:
    Player *m_player;
signals:
};

#endif // ROBOTGRAPLORD_H
