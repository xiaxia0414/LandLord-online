#include "gamecontrol.h"
#include "QDebug"
#include "datamanager.h"
GameControl::GameControl(QObject *parent)
    : QObject{parent}
{

}

void GameControl::palyerInit()
{
    m_robotLeft = new Robot("机器人A",this);
    m_robotRight = new Robot("机器人B",this);
    m_user = new User("我自己",this);

    m_robotLeft->setDirection(Player::Left);
    m_robotRight->setDirection(Player::Right);
    m_user->setDirection(Player::Right);

    Player::Sex sex;
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotLeft->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotRight->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_user->setSex(sex);

    m_user->setPrevPlayer(m_robotLeft);
    m_user->setNextPlayer(m_robotRight);

    m_robotLeft->setPrevPlayer(m_robotRight);
    m_robotLeft->setNextPlayer(m_user);

    m_robotRight->setPrevPlayer(m_user);
    m_robotRight->setNextPlayer(m_robotLeft);

    //m_curPlayer = m_user;

    connect(m_user,&Player::notifyGrabLordBet,this,&GameControl::onGarbBet);
    connect(m_robotLeft,&Player::notifyGrabLordBet,this,&GameControl::onGarbBet);
    connect(m_robotRight,&Player::notifyGrabLordBet,this,&GameControl::onGarbBet);

    connect(this,&GameControl::pendingInfo,m_robotLeft,&Robot::setPendingInfo);
    connect(this,&GameControl::pendingInfo,m_robotRight,&Robot::setPendingInfo);
    connect(this,&GameControl::pendingInfo,m_user,&User::setPendingInfo);

    connect(m_robotLeft,&Robot::notifyPalyHand,this,&GameControl::onPlayHand);
    connect(m_robotRight,&Robot::notifyPalyHand,this,&GameControl::onPlayHand);
    connect(m_user,&User::notifyPalyHand,this,&GameControl::onPlayHand);

}

Robot *GameControl::getLeftRobot()
{
    return m_robotLeft;
}

Robot *GameControl::getRightRobot()
{
    return m_robotRight;
}

User *GameControl::getUserPlayer()
{
    return m_user;
}

void GameControl::setCurrentPlayer(Player *player)
{
    m_curPlayer = player;
}

Player *GameControl::currentPlayer()
{
    return m_curPlayer;
}

Player *GameControl::pendPlayer()
{
    return m_pendPlayer;
}

Cards GameControl::pendCards()
{
    return m_pendCards;
}

void GameControl::initAllCards()
{
    if(DataManager::getInstance()->isNetworkMode())
    {
        m_allCards = DataManager::getInstance()->getCards();
        m_allCards.add(DataManager::getInstance()->getLast3Cards());
        return;
    }
    m_allCards.clear();
    for(int p = Card::Card_Begin+1;p<Card::Card_SJ;++p)
    {
        for(int s = Card::Suit_Begin+1;s<Card::Suit_end;++s)
        {
            Card c((Card::CardPoint)p,(Card::CardSuit)s);
            m_allCards.add(c);
        }
    }
    m_allCards.add(Card(Card::Card_SJ,Card::Suit_Begin));
    m_allCards.add(Card(Card::Card_BJ,Card::Suit_Begin));

}

Card GameControl::takeOneCard()
{
    return m_allCards.takeRandCard();
}

Cards GameControl::getSurplusCards()
{
    return m_allCards;
}

void GameControl::resetCarData()
{
    initAllCards();
    m_robotLeft->clearCards();
    m_robotRight->clearCards();
    m_user->clearCards();

    m_pendPlayer = nullptr;
    m_pendCards.clear();

}

void GameControl::startLordCard()
{
    m_curPlayer->prepareCallLord();
    emit  playerStausChaged(m_curPlayer,ThinkingForCallLord);
}

void GameControl::becomeLord(Player *player,int bet)
{
    m_curBet = bet;
    player->setRole(Player::Lord);
    player->prevPlayer()->setRole(Player::Farmar);
    player->nextPlayer()->setRole(Player::Farmar);

    qDebug()<<player->name()<<player->prevPlayer()->name()<<player->nextPlayer()->name();
    qDebug()<<player->role()<<player->prevPlayer()->role()<<player->nextPlayer()->role();

    m_curPlayer = player;
    m_pendPlayer = nullptr;
    player->storeDispatchCards(m_allCards);

    QTimer::singleShot(1000,this,[=](){
        emit gameSatusChanged(PlayingHand);
        emit playerStausChaged(player,ThinkingForPlayHand);
        m_curPlayer->preparePlayHand();
        //qDebug()<<m_curPlayer->name()<<"pphd";
    });

}

void GameControl::clearPlayerScore()
{
    m_robotLeft->setScore(0);
    m_robotRight->setScore(0);
    m_user->setScore(0);
}

void GameControl::onGarbBet(Player *player, int point)
{
    qDebug()<<m_betRecord.bet<<m_betRecord.player<<m_betRecord.times<<m_curPlayer<<m_curPlayer->nextPlayer();
    if(point == 0||m_betRecord.bet>=point)
    {
        emit notifyGrabLordBet(player,0,false);
        qDebug()<<"case1"<<point;
    }
    else if(point>0&&m_betRecord.bet==0)
    {
        emit notifyGrabLordBet(player,point,true);
         qDebug()<<"case2"<<point;
    }
    else
    {
        emit notifyGrabLordBet(player,point,false);
         qDebug()<<"case3"<<point;
    }
    if(point == 3)
    {
        becomeLord(player,point);
        m_betRecord.reset();
        return;
    }
    if(m_betRecord.bet<point)
    {
        m_betRecord.bet = point;
        m_betRecord.player = player;
    }
    m_betRecord.times++;
    if(m_betRecord.times==3)
    {
        if(m_betRecord.bet==0)
        {
            emit gameSatusChanged(DispatchCard);
        }
        else
        {
            becomeLord(m_betRecord.player,m_betRecord.bet);
            qDebug()<<"onGrabBet"<<m_betRecord.player->name();
        }
        m_betRecord.reset();
        return;
    }

    m_curPlayer = player->nextPlayer();
    emit playerStausChaged(m_curPlayer,ThinkingForCallLord);
    m_curPlayer->prepareCallLord();
}

void GameControl::onPlayHand(Player *player, Cards &card)
{
    emit notifyPlayHand(player,card);
    qDebug()<<m_curPlayer->name();
    if(!card.isEmpty())
    {
        m_pendCards = card;
        m_pendPlayer = player;
        emit pendingInfo(player,card);
    }
    Cards myCards = const_cast<Cards&>(card);
    PlayHand::HandType type = PlayHand(myCards).handType();
    if(type == PlayHand::Hand_Bomb||type == PlayHand::Hand_Bomb_Jokers)
    {
        m_curBet = m_curBet*2;
    }
    if(player->cards().isEmpty())
    {
        Player* prev = player->prevPlayer();
        Player* next = player->nextPlayer();
        if(player->role()==Player::Lord)
        {
            player->setScore(player->score()+m_curBet*2);
            prev->setScore(prev->score()-m_curBet);
            next->setScore(next->score()-m_curBet);
            player->setWin(true);
            prev->setWin(false);
            prev->setWin(false);
        }
        else
        {
            player->setWin(true);
            player->setScore(player->score()+m_curBet);
            if(prev->role()==Player::Lord)
            {
                prev->setScore(prev->score()-2*m_curBet);
                next->setScore(next->score()+m_curBet);
                prev->setWin(false);
                next->setWin(true);
            }
            else
            {
                prev->setScore(prev->score()+m_curBet);
                next->setScore(next->score()-2*m_curBet);
                prev->setWin(true);
                next->setWin(false);
            }
        }
        emit playerStausChaged(player,GameControl::Winning);
        return;

    }
    m_curPlayer = player->nextPlayer();
    m_curPlayer->preparePlayHand();
    emit playerStausChaged(m_curPlayer,GameControl::ThinkingForPlayHand);

}

int GameControl::getPlayerMaxBet()
{
    return m_betRecord.bet;
}

void GameControl::setCurrentPlayer(int index)
{
    if(index == 1)
    {
        m_curPlayer = m_user;
    }
    else if(index == 2)
    {
        m_curPlayer = m_robotRight;
    }
    else if(index == 3)
    {
        m_curPlayer = m_robotLeft;
    }
}
