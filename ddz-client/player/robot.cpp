#include "robot.h"
#include "robotgraplord.h"
#include "strategy.h"
#include "QDebug"
#include "robotplayhand.h"
#include <QThreadPool>
#include "datamanager.h"
#include "taskqueue.h"
Robot::Robot(QObject *parent)
    : Player{parent}
{
    m_type = Player::Robot;
}

void Robot::prepareCallLord()
{
    RobotGrapLord *task = new  RobotGrapLord(this);
    QThreadPool::globalInstance()->start(task);

    // RobotGrapLord* subThread = new RobotGrapLord(this);
    // connect(subThread,&RobotGrapLord::finished,this,[=](){
    //     qDebug()<<"RobotGrapLord 子线程对象析构..."<<",Robot name:"<<this->name();
    //     subThread->deleteLater();
    // });
    // subThread->start();
}

void Robot::preparePlayHand()
{

    RobotPlayHand *task = new  RobotPlayHand(this);
    QThreadPool::globalInstance()->start(task);
    // RobotPlayHand* subThread = new RobotPlayHand(this);
    // connect(subThread, &RobotGrapLord::finished, this, [=](){
    //     qDebug() << "RobotPlayHand 子线程对象析构..." << ", Robot name: " << this->name();
    //     subThread->deleteLater();
    // });
    // subThread->start();
}

void Robot::thinkCallLord()
{
    if(DataManager::getInstance()->isNetworkMode())
    {
        Task t = TaskQueue::getInstance()->take();
        grabLordBet(t.bet);
        return;
    }


    int weight = 0 ;
    Strategy st(this,m_cards);
    weight+= st.getRangeCards(Card::Card_SJ,Card::Card::Card_BJ).cardCount()*6;

    QVector<Cards> optSeq = st.pickOptimalSeqSingles();
    weight+=optSeq.size()*5;

    QVector<Cards> bombs = st.pickOptimalSeqSingles();
    weight+=optSeq.size()*5;

    weight +=m_cards.pointCount(Card::Card_2)*3;

    Cards tmp = m_cards;
    tmp.remove(optSeq);
    tmp.remove(bombs);
    Cards card2 = st.getRangeCards(Card::Card_2,Card::Card_2);
    tmp.remove(card2);

    QVector<Cards> triples = Strategy(this,tmp).findCardsByCount(3);
    weight += triples.size()*4;
    tmp.remove(triples);

    QVector<Cards> pairs = Strategy(this,tmp).findCardsByCount(3);
    weight += pairs.size()*1;
    tmp.remove(pairs);

    if(weight>=22)
    {
        grabLordBet(3);
    }
    else if(weight<22&&weight>=18)
    {
        grabLordBet(2);
    }
    else if(weight<18&&weight>=10)
    {
        grabLordBet(1);
    }
    else
    {
        grabLordBet(0);
    }

}

void Robot::thinkPlayHand()
{
    if(DataManager::getInstance()->isNetworkMode())
    {
        Task t = TaskQueue::getInstance()->take();
        playHand(t.cs);
        return;
    }
    Strategy st(this,m_cards);
    Cards cs = st.makeStrategy();
    playHand(cs);
}
