#include "robotplayhand.h"

RobotPlayHand::RobotPlayHand(Player* player,QObject *parent)
    : QObject(parent) ,QRunnable()
{
    m_player= player;
    this->setAutoDelete(true);
}
void RobotPlayHand::run()
{
    QThread::msleep(2000);
    m_player->thinkPlayHand();
}
