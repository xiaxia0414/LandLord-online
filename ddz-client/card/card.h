#ifndef CARD_H
#define CARD_H

#include "QVector"

#include <QDataStream>
class Card
{
public:


    enum CardSuit
    {
        Suit_Begin,
        Diamond,
        Club,
        Heart,
        Spade,
        Suit_end
    };
    enum CardPoint
    {
        Card_Begin,
        Card_3,
        Card_4,
        Card_5,
        Card_6,
        Card_7,
        Card_8,
        Card_9,
        Card_10,
        Card_J,
        Card_Q,
        Card_K,
        Card_A,
        Card_2,
        Card_SJ,
        Card_BJ,
        Card_End
    };
public:
    Card();
    Card(CardPoint point,CardSuit suit);
    void setSuit(CardSuit suit);
    void setPoint(CardPoint point);
    CardSuit suit() const;
    CardPoint point() const;

    friend QDataStream& operator<<(QDataStream& in, const Card& card);
    friend QDataStream& operator>>(QDataStream& out, Card& card);

private:
    CardPoint m_point;
    CardSuit m_suit;
};

using CardList = QVector<Card>;
bool lessSort(const Card &c1,const Card &c2);
bool greaterSort(const Card &c1,const Card &c2);
bool operator <(const Card& c1, const Card& c2);

bool operator ==(const Card& left,const Card& rigth);
uint qHash(const Card& card);

#endif // CARD_H
