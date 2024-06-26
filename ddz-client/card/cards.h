#ifndef CARDS_H
#define CARDS_H

#include <QSet>
#include "card.h"
#include "QRandomGenerator"
class Cards
{
public:
    enum SortType{Asc,Desc,NoSort};
    Cards();
    Cards(const Card& card);

    void add(const Card& card);
    void add(const Cards& cards);
    void add(const QVector<Cards>& cards);

    void remove(const Card& card);
    void remove(const Cards& cards);
    void remove(const QVector<Cards>& cards);

    Cards& operator <<(const Card& card);
    Cards& operator <<(const Cards& cards);

    int cardCount();
    bool isEmpty();
    bool isEmpty() const;
    void clear();

    Card::CardPoint maxPoint();
    Card::CardPoint minPoint();

    int pointCount(Card::CardPoint point);

    bool contains(const Card& card);
    bool contains(const Cards& cards);

    Card takeRandCard();

    CardList toCardList(SortType type = Desc);

private:
    QList<Card> m_cards;
};

#endif // CARDS_H
