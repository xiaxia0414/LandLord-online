#include "robotgraplord.h"

RobotGrapLord::RobotGrapLord(Player* player,QObject *parent)
    : QObject(parent),QRunnable()
{
    m_player = player;
    this->setAutoDelete(true);
}

void RobotGrapLord::run()
{
    QThread::msleep(2000);
    m_player->thinkCallLord();
}
