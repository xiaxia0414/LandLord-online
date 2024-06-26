#include "cardpanel.h"
#include "QMouseEvent"


CardPanel::CardPanel(QWidget *parent) :QWidget(parent)
{
    m_isfront = true;

}

void CardPanel::setImage(QPixmap &front, QPixmap &back)
{
    m_front = front;
    m_back = back;
    this->setFixedSize(back.width(),back.height());
}

QPixmap CardPanel::image()
{
    return m_front;
}

void CardPanel::setFrontSide(bool flag)
{
    m_isfront = flag;
}

bool CardPanel::isFrontSide()
{
    return m_isfront;
}

void CardPanel::setSelected(bool flag)
{
    m_isSelected = flag;
}

bool CardPanel::isSelected()
{
    return m_isSelected;
}

void CardPanel::setCart(Card &card)
{
    m_card = card;
}

Card CardPanel::card()
{
    return m_card;
}

void CardPanel::setOwner(Player *owner)
{
    m_owner=owner;
}

Player* CardPanel::owner()
{
    return m_owner;
}

void CardPanel::clicked()
{
    emit cardSelected(Qt::LeftButton);
}



void CardPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(m_isfront)
    {
        painter.drawPixmap(rect(),m_front);
    }
    else
    {
        painter.drawPixmap(rect(),m_back);
    }

}

void CardPanel::mousePressEvent(QMouseEvent *event)
{
    emit cardSelected(event->button());
}
