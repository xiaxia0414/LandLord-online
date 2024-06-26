#include "user.h"

User::User(QObject *parent)
    : Player{parent}
{
    m_type = Player::User;
}

void User::prepareCallLord()
{

}

void User::preparePlayHand()
{
     emit startCountDown();
}
