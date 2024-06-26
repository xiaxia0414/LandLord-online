#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "cards.h"
#include "playhand.h"

class Strategy
{
public:
    Strategy();
    Strategy(Player* player,const Cards& cards);
    //出牌策略
    Cards makeStrategy();
    //第一个出牌
    Cards firstPlay();
    //得到比指定牌型大的牌
    Cards getGreaterCards(PlayHand type);
    //能大过指定的牌时，判断是出牌还是放行，返回true出牌，返回false放行
    bool whetherToBeat(Cards &cs);
    //找出指定数量的相同点数的牌，找出count张点数为点数为point的牌
    Cards findSamePointCards(Card::CardPoint point, int count);
    //找出所有点数数量为count的牌==>得到一个多张扑克牌数组
    QVector<Cards> findCardsByCount(int count);
    //根据点数范围找牌
    Cards getRangeCards(Card::CardPoint begin,Card::CardPoint end);
    //按牌型找牌，并指定要找的牌是否要大过指定的牌型
    QVector<Cards> findCardType(PlayHand hand, bool beat);
    // 9. 从指定的Cards对象中挑选出满足条件的顺子
    void pickSeqSingles(QVector<QVector<Cards>> &allSeqRecord, const QVector<Cards> &seqSingle, const Cards& cards);
    // 10. 最优的顺子的集合的筛选函数
    QVector<Cards> pickOptimalSeqSingles();

private:
    using function = Cards(Strategy::*)(Card::CardPoint point);
    struct CardInfo
    {
        Card::CardPoint begin;
        Card::CardPoint end;
        int extra;
        bool beat;
        int number;
        int base;
        function getSeq;

    };
    QVector<Cards> getCards(Card::CardPoint point , int number);//从point遍历每个找number张
    QVector<Cards> getTripleSingleOrPair(Card::CardPoint begin,PlayHand::HandType type);
    QVector<Cards> getPlane(Card::CardPoint begin);
    QVector<Cards> getPlane2SingleOrPair(Card::CardPoint begin,PlayHand::HandType type);
    QVector<Cards> getSepPairOrSeqSingle(CardInfo &info);
    Cards getBaseSeqPair(Card::CardPoint point);
    Cards getBaseSeqSingle(Card::CardPoint point);
    QVector<Cards> getBomb(Card::CardPoint begin);

private:
    Player* m_player;
    Cards m_cards;
};

#endif // STRATEGY_H
