#include "playhand.h"

PlayHand::PlayHand()
{

}

PlayHand::PlayHand(Cards &cards)
{
    classify(cards);
    judgeCardType();
}

PlayHand::PlayHand(HandType type, Card::CardPoint pt, int extra)
{
    m_type = type;
    m_pt = pt;
    m_extra = extra;
}

void PlayHand::classify(Cards &cards)
{
    CardList list = cards.toCardList();
    int cardRecord[Card::Card_End];
    memset(cardRecord,0,sizeof(int)*Card::Card_End);
    for(int i=0;i<list.size();i++)
    {
        Card c = list.at(i);
        cardRecord[c.point()]++;
    }
    m_1Card.clear();
    m_2Card.clear();
    m_3Card.clear();
    m_4Card.clear();
    for(int i=0;i<Card::Card_End;i++)
    {
        if(cardRecord[i]==1)
        {
              m_1Card.push_back((Card::CardPoint)i);
        }
       else if(cardRecord[i]==2)
        {
              m_2Card.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i]==3)
        {
              m_3Card.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i]==4)
        {
              m_4Card.push_back((Card::CardPoint)i);
        }
    }
}

void PlayHand::judgeCardType()
{
    m_type = Hand_Unknown;
    m_pt = Card::Card_Begin;
    m_extra = 0;//连对的个数
    if(isPass())
    {
        m_type = Hand_Pass;
    }
    else if(isSingle())
    {
        m_type = Hand_Single;
        m_pt = m_1Card[0];
    }
    else if(isPair())
    {
        m_type = Hand_Pair;
        m_pt = m_2Card[0];
    }
    else if(isTriple())
    {
        m_type = Hand_Triple;
        m_pt = m_3Card[0];
    }
    else if(isTripleSingle())
    {
        m_type = Hand_Triple_Single;
        m_pt = m_3Card[0];
    }
    else if(isTriplePair())
    {
        m_type = Hand_Triple_Pair;
        m_pt = m_3Card[0];
    }
    else if(isPlane())
    {
        m_type = Hand_Plane;
        m_pt = m_3Card[0];
    }
    else if(isPlaneTwoSingle())
    {
        m_type = Hand_Plane_Two_Single;
        m_pt = m_3Card[0];
    }
    else if(isPlaneTwoPair())
    {
        m_type = Hand_Plane_Two_Pair;
        m_pt = m_3Card[0];
    }
    else if(isSeqPair())
    {
        m_type = Hand_Seq_Pair;
        m_pt = m_2Card[0];
        m_extra = m_2Card.size();
    }
    else if(isSeqSingle())
    {
        m_type = Hand_Seq_Single;
        m_pt = m_1Card[0];
        m_extra = m_1Card.size();
    }
    else if(isBomb())
    {
        m_type = Hand_Bomb;
        m_pt = m_4Card[0];
    }
    else if(isBombSingle())
    {
        m_type = Hand_Bomb_Single;
        m_pt = m_4Card[0];
    }
    else if(isBombPair())
    {
        m_type = Hand_Bomb_Pair;
        m_pt = m_4Card[0];
    }
    else if(isBombJokers())
    {
        m_type = Hand_Bomb_Jokers;
    }
    else if(isBombJokersPair())
    {
        m_type = Hand_Bomb_Jokers_Pair;
    }
    else if(isBombJokersSingle())
    {
        m_type = Hand_Bomb_Jokers_Single;
    }
    else if(isBombJokersTwoSingle())
    {
        m_type = Hand_Bomb_Jokers_Two_Single;
    }

}
bool PlayHand::canBeat(const PlayHand& other)
{
    if(m_type == Hand_Unknown)
    {
        return false;
    }
    // 对方放弃出牌
    if(other.m_type == Hand_Pass)
    {
        return true;
    }
    // 我是王炸
    if(m_type == Hand_Bomb_Jokers)
    {
        return true;
    }
    if(m_type == Hand_Bomb && other.m_type >= Hand_Single && other.m_type <= Hand_Seq_Single)
    {
        return true;
    }
    // 双方的牌型一致
    if(m_type == other.m_type)
    {
        if(m_type == Hand_Seq_Pair || m_type == Hand_Seq_Single)
        {
              return m_pt > other.m_pt && m_extra == other.m_extra;
        }
        else
        {
              return m_pt > other.m_pt;
        }
    }
    return false;
}

PlayHand::HandType PlayHand::handType()
{
    return m_type;
}

Card::CardPoint PlayHand::cardPoint()
{
    return m_pt;
}

int PlayHand::extra()
{
    return m_extra;
}

bool PlayHand::isPass()
{
    if(m_1Card.size() == 0 && m_2Card.isEmpty() && m_3Card.isEmpty() && m_4Card.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isSingle()
{
    if(m_1Card.size() == 1 && m_2Card.isEmpty() && m_3Card.isEmpty() && m_4Card.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPair()
{
    if(m_1Card.size() == 0 && m_2Card.size()==1 && m_3Card.isEmpty() && m_4Card.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTriple()
{
    if(m_1Card.size() == 0 && m_2Card.size()==0 && m_3Card.size()==1 && m_4Card.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTripleSingle()
{
    if(m_1Card.size() == 1 && m_2Card.size()==0 && m_3Card.size()==1 && m_4Card.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTriplePair()
{
    if(m_1Card.size() == 0 && m_2Card.size()==1 && m_3Card.size()==1 && m_4Card.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPlane()
{
    if(m_1Card.size() == 0 && m_2Card.isEmpty() && m_3Card.size()==2 && m_4Card.isEmpty())
    {
        std::sort(m_3Card.begin(),m_3Card.end());
        if(m_3Card[0]-m_3Card[1]==1&&m_3Card[1]<Card::Card_2)
        {
              return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoSingle()
{
    if(m_1Card.size() == 2 && m_2Card.isEmpty() && m_3Card.size()==2 && m_4Card.isEmpty())
    {
        std::sort(m_3Card.begin(),m_3Card.end());
        std::sort(m_1Card.begin(),m_3Card.end());
        if(m_3Card[0]-m_3Card[1]==1&&m_3Card[1]<Card::Card_2&&m_1Card[0]!=Card::Card_SJ&&m_1Card[1]!=Card::Card_BJ)
        {
              return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoPair()
{
    if(m_1Card.size() == 0 && m_2Card.size()==2 && m_3Card.size()==2 && m_4Card.isEmpty())
    {
        std::sort(m_3Card.begin(),m_3Card.end());
        if(m_3Card[0]-m_3Card[1]==1&&m_3Card[1]<Card::Card_2)
        {
              return true;
        }
    }
    return false;
}

bool PlayHand::isSeqPair()
{
    if(m_1Card.isEmpty() && m_2Card.size()>=3 && m_3Card.isEmpty() && m_4Card.isEmpty())
    {
        std::sort(m_2Card.begin(),m_2Card.end());
        if(m_2Card.last()-m_2Card.first()==(m_2Card.size()-1)&&
            m_2Card.first()>=Card::Card_3&&m_2Card.last()<Card::Card_2)
        {
              return true;
        }
    }
    return false;
}

bool PlayHand::isSeqSingle()
{
    if(m_1Card.size() >=5 && m_2Card.isEmpty() && m_3Card.isEmpty() && m_4Card.isEmpty())
    {
        std::sort(m_1Card.begin(),m_1Card.end());
        if(m_1Card.last()-m_1Card.first()==(m_1Card.size()-1)&&
            m_1Card.first()>=Card::Card_3&&m_1Card.last()<Card::Card_2)
        {
              return true;
        }
    }
    return false;
}

bool PlayHand::isBomb()
{
    if(m_1Card.isEmpty() && m_2Card.isEmpty() && m_3Card.isEmpty() && m_4Card.size()==1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombSingle()
{
    if(m_1Card.size()==1 && m_2Card.isEmpty() && m_3Card.isEmpty() && m_4Card.size()==1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombPair()
{
    if(m_1Card.isEmpty() && m_2Card.size()==1 && m_3Card.isEmpty() && m_4Card.size()==1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombTwoSingle()
{
    if(m_1Card.size()==2 && m_2Card.isEmpty() && m_3Card.isEmpty() && m_4Card.size()==1)
    {
        std::sort(m_1Card.begin(),m_1Card.end());
        if(m_1Card.first()!=Card::Card_SJ&&m_1Card.last()!=Card::Card_BJ)
        return true;
    }
    return false;
}

bool PlayHand::isBombJokers()
{
    if(m_1Card.size()==2 && m_2Card.isEmpty() && m_3Card.isEmpty() && m_4Card.isEmpty())
    {
        std::sort(m_1Card.begin(),m_1Card.end());
        if(m_1Card.first()==Card::Card_SJ&&m_1Card.last()==Card::Card_BJ)
        return true;
    }
    return false;
}

bool PlayHand::isBombJokersSingle()
{
    if(m_1Card.size()==3 && m_2Card.isEmpty() && m_3Card.isEmpty() && m_4Card.isEmpty())
    {
        std::sort(m_1Card.begin(),m_1Card.end());
        if(m_1Card[1]==Card::Card_SJ&&m_1Card[2]==Card::Card_BJ)
        return true;
    }
    return false;
}

bool PlayHand::isBombJokersPair()
{
    if(m_1Card.size()==2 && m_2Card.size()==1 && m_3Card.isEmpty() && m_4Card.isEmpty())
    {
        std::sort(m_1Card.begin(),m_1Card.end());
        if(m_1Card.first()==Card::Card_SJ&&m_1Card.last()==Card::Card_BJ)
        return true;
    }
    return false;
}

bool PlayHand::isBombJokersTwoSingle()
{
    if(m_1Card.size()==4 && m_2Card.size()==1 && m_3Card.isEmpty() && m_4Card.isEmpty())
    {
        std::sort(m_1Card.begin(),m_1Card.end());
        if(m_1Card[2]==Card::Card_SJ&&m_1Card[3]==Card::Card_BJ)
        return true;
    }
    return false;
}
