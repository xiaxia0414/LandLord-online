#include "strategy.h"
#include <QMap>
#include <functional>
#include <QDebug>
Strategy::Strategy()
{

}

Strategy::Strategy(Player *player, const Cards &cards)
{
    m_player = player;
    m_cards = cards;
}

Cards Strategy::makeStrategy()
{
    Player* pendPlayer = m_player->pendPlayer();
    Cards pendCards = m_player->pendCards();
    //qDebug()<<"makeStrategy:"<<m_player->pendPlayer();
    if(pendPlayer==m_player||pendPlayer==nullptr)
    {
        return firstPlay();
    }
    else
    {
        PlayHand type(pendCards);
        Cards beatCards = getGreaterCards(type);

        bool shouldBeat = whetherToBeat(beatCards);
        if(shouldBeat)
        {
            return beatCards;
        }
        else
        {
            return Cards();
        }
    }
    return Cards();
}

Cards Strategy::firstPlay()
{
    PlayHand hand(m_cards);
    if(hand.handType()!= PlayHand::Hand_Unknown)//只剩单一牌型
    {
        return m_cards;
    }

    QVector<Cards> optimalSeq = pickOptimalSeqSingles();

    if(!optimalSeq.isEmpty())
    {
        int baseNum = findCardsByCount(1).size();

        Cards save = m_cards;
        save.remove(optimalSeq);
        int lastNum = Strategy(m_player,save).findCardsByCount(1).size();
        if(baseNum>lastNum)
        {
            return optimalSeq[0];
        }
    }

    bool hasPlane,hasTriple,hasPair;
    hasPair = hasPlane = hasTriple=false;
    Cards backup=m_cards;

    QVector<Cards> bombArray = findCardType(PlayHand(PlayHand::Hand_Bomb,Card::Card_Begin,0),false);
    backup.remove(bombArray);

    QVector<Cards> planeArray = Strategy(m_player,backup).findCardType(PlayHand(PlayHand::Hand_Plane, Card::Card_Begin, 0),false);
    if(!planeArray.isEmpty())
    {
        hasPlane =true;
        backup.remove(planeArray);
    }
    QVector<Cards> seqTripleArray = Strategy(m_player,backup).findCardType(PlayHand(PlayHand::Hand_Plane, Card::Card_Begin, 0),false);
    if(!planeArray.isEmpty())
    {
        hasTriple =true;
        backup.remove(seqTripleArray);
    }
    QVector<Cards> seqPairArray = Strategy(m_player,backup).findCardType(PlayHand(PlayHand::Hand_Plane, Card::Card_Begin, 0),false);
    if(!planeArray.isEmpty())
    {
        hasPair =true;
        backup.remove(seqPairArray);
    }
    if(hasPair)
    {
        Cards maxPair;
        for(int i=0 ;i<seqPairArray.size();i++)
        {
            if(seqPairArray[i].cardCount()>maxPair.cardCount())
            {
                maxPair = seqPairArray[i];
            }
        }
        return maxPair;
    }
    if(hasPlane)
    {
        bool twoPairFond = false;
        QVector<Cards> pairArray;
        for(Card::CardPoint point = Card::Card_3;point <=Card::Card_10;point=(Card::CardPoint)(point+1))
        {
            Cards pair = Strategy(m_player,backup).findSamePointCards(point,2);
            if(!pair.isEmpty())
            {
                pairArray.push_back(pair);
                if(pairArray.size()==2)
                {
                    twoPairFond = true;
                    break;
                }
            }
        }
        if(twoPairFond)
        {
            Cards tmp = planeArray[0];
            tmp.add(pairArray);
            return tmp;
        }
        else
        {
            bool twoSingleFond = false;
            QVector<Cards> SingleArray;
            for(Card::CardPoint point = Card::Card_3;point <=Card::Card_10;point=(Card::CardPoint)(point+1))
            {
                if(backup.pointCount(point)==1)
                {
                    Cards single = Strategy(m_player,backup).findSamePointCards(point,1);
                    if(!single.isEmpty())
                    {
                        pairArray.push_back(single);
                        if(pairArray.size()==2)
                        {
                            twoSingleFond = true;
                            break;
                        }
                    }
                }
            }
            if(twoSingleFond)
            {
                Cards tmp = planeArray[0];
                tmp.add(SingleArray);
                return tmp;
            }
            else
            {
                return planeArray[0];
            }

        }

    }

    if(hasTriple)
    {
        if(PlayHand(seqTripleArray[0]).cardPoint()<Card::Card_4)
        {
            for(Card::CardPoint point = Card::Card_3;point <=Card::Card_A;point=(Card::CardPoint)(point+1))
            {
                int pointCount= backup.pointCount(point);
                if(pointCount==1)
                {
                    Cards single = Strategy(m_player,backup).findSamePointCards(point,1);
                    Cards tmp = seqTripleArray[0];
                    tmp.add(single);
                    return tmp;
                }
                else if(pointCount == 2)
                {
                    Cards pair = Strategy(m_player,backup).findSamePointCards(point,2);
                    Cards tmp = seqTripleArray[0];
                    tmp.add(pair);
                    return tmp;
                }
            }
        }

        return seqTripleArray[0];
    }

    Player *nextPlayer = m_player->nextPlayer();
    if(nextPlayer->cards().cardCount()==1&&m_player->role()!=nextPlayer->role())
    {
        for(Card::CardPoint point = Card::CardPoint(Card::Card_End-1);
             point >= Card::Card_3; point = Card::CardPoint(point-1))
        {
            int pointCount = backup.pointCount(point);
            if(pointCount == 1)
            {
                Cards single = Strategy(m_player, backup).findSamePointCards(point, 1);
                return single;
            }
            else if(pointCount == 2)
            {
                Cards pair = Strategy(m_player, backup).findSamePointCards(point, 2);
                return pair;
            }
        }
    }
    else
    {
        for(Card::CardPoint point = Card::Card_3;
             point < Card::Card_End; point = Card::CardPoint(point+1))
        {
            int pointCount = backup.pointCount(point);
            if(pointCount == 1)
            {
                Cards single = Strategy(m_player, backup).findSamePointCards(point, 1);
                return single;
            }
            else if(pointCount == 2)
            {
                Cards pair = Strategy(m_player, backup).findSamePointCards(point, 2);
                return pair;
            }
        }
    }
    return Cards();

}

Cards Strategy::getGreaterCards(PlayHand type)
{
    Player* pendPlayer = m_player->pendPlayer();
    if(pendPlayer !=nullptr&&pendPlayer->role()!=m_player->role()&&pendPlayer->cards().cardCount()<=3)
    {
        QVector<Cards> bombs = findCardsByCount(4);
        for(int i =0 ; i<bombs.size();i++)
        {
            if(PlayHand(bombs[i]).canBeat(type))
            {
                return bombs[i];
            }
        }

        Cards sj = findSamePointCards(Card::Card_SJ,1);
        Cards bj = findSamePointCards(Card::Card_BJ,1);
        if(!sj.isEmpty()&&!bj.isEmpty())
        {
            Cards jokers;
            jokers<<sj<<bj;
            return jokers;
        }
    }
    Player *nextPlayer = m_player->nextPlayer();
    Cards remain = m_cards;
    remain.remove(Strategy(m_player,remain).pickOptimalSeqSingles());

    auto beatCard = std::bind([=](const Cards& cards){
        QVector<Cards> beatCardsArray = Strategy(m_player,cards).findCardType(type,true);
        if(!beatCardsArray.isEmpty())
         {
            if(m_player->role()!=nextPlayer->role()&&nextPlayer->cards().cardCount()<=2)
            {
                return beatCardsArray.back();
            }
            else
            {
                return beatCardsArray.front();
            }

        }
        return Cards();
    },std::placeholders::_1);
    Cards cs;
    if(!(cs=beatCard(remain)).isEmpty())
    {
        return cs;
    }
    else
    {
        if(!(cs=beatCard(m_cards)).isEmpty())return cs;
    }
    return Cards();
}

bool Strategy::whetherToBeat(Cards &cs)
{
    if(cs.isEmpty())
    {
        return false;
    }
    Player* pendPlayer = m_player->pendPlayer();
    if(m_player->role()==pendPlayer->role())
    {
        Cards left = m_cards;
        left.remove(cs);
        if(PlayHand(left).handType()!=PlayHand::Hand_Unknown)
        {
            //手里的牌所剩无几并是一个完整的牌型
            return true;
        }
        Card::CardPoint basePoint = PlayHand(cs).cardPoint();
        if(basePoint == Card::Card_2||basePoint == Card::Card_SJ||basePoint==Card::Card_BJ)
        {
            return true;
        }
    }

    else
    {
        PlayHand myHand(cs);
        if((myHand.handType()==PlayHand::Hand_Triple_Single||myHand.handType()==PlayHand::Hand_Triple_Pair)&&myHand.cardPoint()==Card::Card_2)
        {
            return false;
        }
        if(myHand.handType()==PlayHand::Hand_Pair&&myHand.cardPoint()==Card::Card_2&&pendPlayer->cards().cardCount()>=10&&m_player->cards().cardCount()>=5)
        {
            return false;
        }

    }
    return true;


}

Cards Strategy::findSamePointCards(Card::CardPoint point, int count)
{
    if((count<1)||(count>4))
    {
        return Cards();
    }
    if(point == Card::Card_SJ||point == Card::Card_BJ)
    {
        if(count>1)
        {
            return Cards();
        }
        Card card;
        card.setPoint(point);
        card.setSuit(Card::Suit_Begin);
        if(m_cards.contains(card))
        {
            Cards cards;
            cards.add(card);
            return cards;
        }
        return Cards();
    }
   //不是大小王
    int findCount = 0;
    Cards findCards;
   for(int suit = Card::Suit_Begin+1;suit<Card::Suit_end;suit++)
    {
        Card card;
        card.setPoint(point);
        card.setSuit((Card::CardSuit)suit);
        if(m_cards.contains(card))
        {
            findCount++;
            findCards.add(card);
            if(findCount==count)
            {
                return findCards;
            }
        }
    }
   return Cards();

}

QVector<Cards> Strategy::findCardsByCount(int count)
{
   if(count <1||(count>4))
   {
        return QVector<Cards>();
   }
   QVector<Cards> cardArray;
   for(Card::CardPoint point = Card::Card_3;point<Card::Card_End;point = (Card::CardPoint)(point+1))
   {
        if(m_cards.pointCount(point)==count)
        {
            Cards cs;
            cs<<findSamePointCards(point,count);
            cardArray<<cs;
        }
   }
   return cardArray;
}

Cards Strategy::getRangeCards(Card::CardPoint begin, Card::CardPoint end)
{
   Cards rangeCards;
   for(Card::CardPoint point = begin;point<end;point = (Card::CardPoint)(point+1))
   {
            int count = m_cards.pointCount(point);
            Cards cs = findSamePointCards(point,count);
            rangeCards <<cs;
   }
   return rangeCards;

}

QVector<Cards> Strategy::findCardType(PlayHand hand, bool beat)
{
   PlayHand::HandType type = hand.handType();
   Card::CardPoint point = hand.cardPoint();
   int extra = hand.extra();

   Card::CardPoint beginPt = beat?Card::CardPoint(point +1):Card::Card_3;
   if(type == PlayHand::Hand_Single)
   {
        return getCards(beginPt,1);
   }
   else if(type == PlayHand::Hand_Pair)
   {
         return getCards(beginPt,2);
   }
   else if(type == PlayHand::Hand_Triple)
   {
         return getCards(beginPt,3);
   }
   else if(type ==PlayHand::Hand_Triple_Single)
   {
         return getTripleSingleOrPair(beginPt,PlayHand::Hand_Single);
   }
   else if(type ==PlayHand::Hand_Triple_Pair)
   {
          return getTripleSingleOrPair(beginPt,PlayHand::Hand_Pair);
   }
   else if(type == PlayHand::Hand_Plane)
   {
          return getPlane(beginPt);
   }
   else if(type == PlayHand::Hand_Plane_Two_Single)
   {
          return getPlane2SingleOrPair(beginPt,PlayHand::Hand_Single);
   }
   else if(type == PlayHand::Hand_Plane_Two_Pair)
   {
          return getPlane2SingleOrPair(beginPt,PlayHand::Hand_Pair);
   }
   else if(type == PlayHand::Hand_Seq_Pair)
   {
          CardInfo info;
          info.begin = beginPt;
          info.end = Card::Card_Q;
          info.number = 2;
          info.base = 3;
          info.extra = extra;
          info.beat = beat;
          info.getSeq = &Strategy::getBaseSeqPair;
          return getSepPairOrSeqSingle(info);
   }
   else if(type == PlayHand::Hand_Seq_Single)
   {
          CardInfo info;
          info.begin = beginPt;
          info.end = Card::Card_10;
          info.number = 1;
          info.base = 5;
          info.extra = extra;
          info.beat = beat;
          info.getSeq = &Strategy::getBaseSeqSingle;
          return getSepPairOrSeqSingle(info);
   }
   else if(type == PlayHand::Hand_Bomb)
   {
          return getBomb(beginPt);
   }
   else
   {
          return QVector<Cards>();
   }
}

void Strategy::pickSeqSingles(QVector<QVector<Cards> > &allSeqRecord, const QVector<Cards> &seqSingle, const Cards &cards)
{
   QVector<Cards> allSeq=Strategy(m_player,cards).findCardType(PlayHand(PlayHand::Hand_Seq_Single,Card::Card_Begin,0),false);
   if(allSeq.isEmpty())
   {
          allSeqRecord<<seqSingle;
   }
   else
   {
          Cards saveCards = cards;
          for(int i=0;i<allSeq.size();i++)
          {
            Cards aScheme = allSeq.at(i);
            Cards temp = saveCards;
            temp.remove(aScheme);
            QVector<Cards> seqArray = seqSingle;
            seqArray<<aScheme;
            pickSeqSingles(allSeqRecord,seqArray,temp);
          }
   }
}

QVector<Cards> Strategy::pickOptimalSeqSingles()
{
   QVector<QVector<Cards>> seqRecord;
   QVector<Cards> seqSignles;
   Cards save = m_cards;
   save.remove(findCardsByCount(4));
   save.remove(findCardsByCount(3));
   pickSeqSingles(seqRecord,seqSignles,save);
   if(seqRecord.isEmpty())
   {
          return QVector<Cards>();
   }
   QMap<int,int> seqMarks;
   for(int i=0;i<seqRecord.size();i++)
   {
          Cards backupCards = m_cards;
          QVector<Cards> seqArray = seqRecord[i];
          backupCards.remove(seqArray);

          QVector<Cards> singleArray = Strategy(m_player,backupCards).findCardsByCount(1);
          CardList cardlist;
          for(int j=0;j<singleArray.size();j++)
          {
            cardlist<<singleArray[j].toCardList();
          }
          int mark =0;
          for(int j =0;j<cardlist.size();j++)
          {
            mark +=cardlist[j].point()+15;
          }
          seqMarks.insert(i,mark);


   }
   int value = 0;
   int comMark=100;
   auto it = seqMarks.constBegin();
   for(;it!=seqMarks.end();it++)
   {
          if(it.value()<comMark)
          {
            value = it.key();
          }
   }
   return seqRecord[value];

}

QVector<Cards> Strategy::getCards(Card::CardPoint point, int number)
{
   QVector<Cards> findCardsArray;
   for(Card::CardPoint pt = point;pt<Card::Card_End;pt =(Card::CardPoint)(pt+1))
   {
        if(m_cards.pointCount(pt)==number)
        {
        Cards cs = findSamePointCards(pt,number);
         findCardsArray<<cs;
        }
   }
   return findCardsArray;
}

QVector<Cards> Strategy::getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type)
{
   QVector<Cards> findCardArray = getCards(begin,3);
   if(!findCardArray.isEmpty())
   {
        Cards remainCards = m_cards;
        remainCards.remove(findCardArray);

        Strategy st(m_player,remainCards);
        QVector<Cards> cardsArray = st.findCardType(PlayHand(type,Card::Card_Begin,0),false);
        if(!cardsArray.isEmpty())
        {
            for(int i=0;i<findCardArray.size();i++)
            {
                findCardArray[i].add(cardsArray.at(i));
            }
        }
        else
        {
            findCardArray.clear();
        }
   }

   return findCardArray;
}

QVector<Cards> Strategy::getPlane(Card::CardPoint begin)
{
   QVector<Cards> findCardArray;
   for(Card::CardPoint point = begin;point<=Card::Card_K;point = (Card::CardPoint)(point+1))
   {
        Cards PrevCards = findSamePointCards(point,3);
        Cards nextCards = findSamePointCards((Card::CardPoint)(point+1),3);
        if(!PrevCards.isEmpty()&&!nextCards.isEmpty())
        {
            Cards tmp;
            tmp<<PrevCards<<nextCards;
            findCardArray<<tmp;
        }
   }
   return findCardArray;
}

QVector<Cards> Strategy::getPlane2SingleOrPair(Card::CardPoint begin, PlayHand::HandType type)
{
   QVector<Cards> findCardArray = getPlane(begin);
   if(!findCardArray.isEmpty())
   {
        Cards remainCards = m_cards;
        remainCards.remove(findCardArray);

        Strategy st(m_player,remainCards);
        QVector<Cards> cardsArray = st.findCardType(PlayHand(type,Card::Card_Begin,0),false);
        if(cardsArray.size()>=2)
        {
            for(int i=0;i<findCardArray.size();i++)
            {
                Cards tmp;
                tmp<<cardsArray[0]<<cardsArray[1];
                findCardArray[i].add(tmp);
            }
        }
        else
        {
            findCardArray.clear();
        }
   }

   return findCardArray;
}

QVector<Cards> Strategy::getSepPairOrSeqSingle(CardInfo &info)
{
   QVector<Cards> findCardsArray;
   if(info.beat)
   {
        for(Card::CardPoint point = info.begin;point<=info.end;point = (Card::CardPoint)(point+1))
        {
            bool found = true;
            Cards seqCards;
            for(int i=0;i<info.extra;i++)
            {
                Cards cards = findSamePointCards((Card::CardPoint)(point+i),info.number);
                if(cards.isEmpty()||(point+info.extra>=Card::Card_2))
                {
                    found == false;
                    seqCards.clear();
                    break;
                }
                else
                {
                    seqCards<<cards;
                }
            }
            if(found)
            {
                findCardsArray<<seqCards;
                return findCardsArray;
            }
        }
   }
   else
   {
        for(Card::CardPoint point = info.begin;point<= info.end;point = (Card::CardPoint)(point+1))
        {
            Cards baseSeq = (this->*info.getSeq)(point);
            if(baseSeq.isEmpty())
            {
                continue;
            }
            findCardsArray<<baseSeq;
            int followed = info.base;
            Cards alreadyFollowedCards;
            while(true)
            {
                Card::CardPoint followedPoint = (Card::CardPoint)(point + followed);

                if(followedPoint>=Card::Card_2)
                {
                    break;
                }
                Cards follwedCards = findSamePointCards(followedPoint,info.number);
                if(follwedCards.isEmpty())
                {
                    break;
                }
                else
                {
                    alreadyFollowedCards<<follwedCards;
                    Cards newSeq = baseSeq;
                    newSeq<<alreadyFollowedCards;
                    findCardsArray<<newSeq;
                    followed++;
                }
            }

        }
   }
   return findCardsArray;
}

Cards Strategy::getBaseSeqPair(Card::CardPoint point)
{
   Cards cards0 = findSamePointCards(point,2);
   Cards cards1 = findSamePointCards((Card::CardPoint)(point+1),2);
   Cards cards2 = findSamePointCards((Card::CardPoint)(point+2),2);
   Cards baseSeq;
   if(!cards0.isEmpty()&&!cards1.isEmpty()&&!cards2.isEmpty())
   {
        baseSeq<<cards0<<cards1<<cards2;
   }
   return baseSeq;

}

Cards Strategy::getBaseSeqSingle(Card::CardPoint point)
{
   Cards cards0 = findSamePointCards(point,1);
   Cards cards1 = findSamePointCards((Card::CardPoint)(point+1),1);
   Cards cards2 = findSamePointCards((Card::CardPoint)(point+2),1);
   Cards cards3 = findSamePointCards((Card::CardPoint)(point+3),1);
   Cards cards4 = findSamePointCards((Card::CardPoint)(point+4),21);
   Cards baseSeq;
   if(!cards0.isEmpty()&&!cards1.isEmpty()&&!cards2.isEmpty()&&!cards3.isEmpty()&&!cards4.isEmpty())
   {
        baseSeq<<cards0<<cards1<<cards2<<cards3<<cards4;
   }
   return baseSeq;
}

QVector<Cards> Strategy::getBomb(Card::CardPoint begin)
{
   QVector<Cards> findCardsArray;
   for(Card::CardPoint point = begin;point<Card::Card_End;point=(Card::CardPoint)(point+1))
   {
        Cards cs = findSamePointCards(point,4);
        if(!cs.isEmpty())
        {
            findCardsArray<<cs;
        }
   }
   return findCardsArray;
}
