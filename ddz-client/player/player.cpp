#include "player.h"
#include "QDebug"
Player::Player(QObject *parent)
    : QObject{parent}
{
    m_score = 0;
    m_isWin = false;
}

Player::Player(QString name, QObject *parent):Player(parent)
{
    m_name = name;
}

void Player::setScore(int score)
{
    m_score = score;
}

int Player::score()
{
    return m_score;
}

void Player::setName(QString name)
{
    m_name=name;
}

QString Player::name()
{
    return m_name;
}

void Player::setRole(Role role)
{
    m_role = role;
}

Player::Role Player::role()
{
    return m_role;
}

void Player::setSex(Sex sex)
{
    m_sex = sex;
}

Player::Sex Player::sex()
{
    return m_sex;
}

void Player::setDirection(Direction direction)
{
    m_direction = direction;
}

Player::Direction Player::direction()
{
    return m_direction;
}

void Player::setType(Type type)
{
    m_type = type;
}

Player::Type Player::type()
{
    return m_type;

}

void Player::setWin(bool isWin)
{
    m_isWin = isWin;
}

bool Player::isWin()
{
    return m_isWin;
}

void Player::setPrevPlayer(Player *player)
{
    m_prev = player;
}

void Player::setNextPlayer(Player *player)
{
    m_next = player;
}

Player *Player::prevPlayer()
{
    return m_prev;
}

Player *Player::nextPlayer()
{
    return m_next;
}

void Player::grabLordBet(int point)
{
    emit notifyGrabLordBet(this,point);
}

void Player::storeDispatchCard(Card &card)
{
    m_cards.add(card);
    Cards cs;
    cs.add(card);
    emit notifyPickCards(this,cs);
}

void Player::storeDispatchCards(Cards &cards)
{
    m_cards.add(cards);
    emit notifyPickCards(this,cards);
}

Cards Player::cards()
{
    return m_cards;
}

void Player::clearCards()
{
    m_cards.clear();
}

void Player::playHand(Cards &cards)
{
    m_cards.remove(cards);
    qDebug()<<this->name()<<"hasPlayHand";
    emit notifyPalyHand(this,cards);

}

void Player::setPendingInfo(Player *player, Cards cards)
{
    m_pendPlayer=player;
    m_pendCards = cards;
}

Player *Player::pendPlayer()
{
    return m_pendPlayer;
}

Cards Player::pendCards()
{
    return m_pendCards;
}

void Player::prepareCallLord()
{

}

void Player::preparePlayHand()
{

}
void Player::thinkPlayHand()
{

}
void Player::thinkCallLord()
{

}
