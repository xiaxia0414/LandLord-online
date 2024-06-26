#include "card.h"

Card::Card()
{


}

Card::Card(CardPoint point, CardSuit suit)
{
    m_point=point;
    m_suit=suit;
}

void Card::setSuit(CardSuit suit)
{
    m_suit= suit;
}

void Card::setPoint(CardPoint point)
{
    m_point = point;
}

Card::CardSuit Card::suit() const
{
    return m_suit;
}

Card::CardPoint Card::point() const
{
    return m_point;
}

bool lessSort(const Card &c1, const Card &c2)
{
    if(c1.point()== c2.point())
    {
        return c1.suit()<c2.suit();
    }
    else
    {
        return c1.point()<c2.point();
    }
}

bool greaterSort(const Card &c1, const Card &c2)
{
    if(c1.point()== c2.point())
    {
        return c1.suit()>c2.suit();
    }
    else
    {
        return c1.point()>c2.point();
    }
}
bool operator ==(const Card& left,const Card& rigth)
{
    return (left.point()==rigth.point()&&left.suit()==rigth.suit());
}


uint qHash(const Card &card)
{
    return card.point()*100+card.suit();
}
bool operator <(const Card& c1, const Card& c2)
{
    return lessSort(c1, c2);
}
QDataStream& operator<<(QDataStream& in, const Card& card)
{
    int suit = static_cast<int>(card.suit());
    int point = static_cast<int>(card.point());
    in << suit << point;
    return in;
}
QDataStream& operator>>(QDataStream& out, Card& card)
{
    int suit, point;
    out >> suit >> point;
    card.setSuit(static_cast<Card::CardSuit>(suit));
    card.setPoint(static_cast<Card::CardPoint>(point));
    return out;
}
