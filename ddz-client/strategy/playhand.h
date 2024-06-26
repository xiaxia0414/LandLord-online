#ifndef PLAYHAND_H
#define PLAYHAND_H
#include "cards.h"

class PlayHand
{
public:
    enum HandType
    {
        Hand_Unknown,               // 未知
        Hand_Pass,                  // 过

        Hand_Single,                // 单
        Hand_Pair,                  // 对

        Hand_Triple,                // 三个
        Hand_Triple_Single,         // 三带一
        Hand_Triple_Pair,           // 三带二

        Hand_Plane,                 // 飞机，555_666
        Hand_Plane_Two_Single,      // 飞机带单，555_666_3_4
        Hand_Plane_Two_Pair,        // 飞机带双，555_666_33_44

        Hand_Seq_Pair,              // 连对，33_44_55(_66...)
        Hand_Seq_Single,            // 顺子，34567(8...)

        Hand_Bomb,                  // 炸弹
        Hand_Bomb_Single,           // 炸弹带一个
        Hand_Bomb_Pair,             // 炸弹带一对
        Hand_Bomb_Two_Single,       // 炸弹带两单

        Hand_Bomb_Jokers,           // 王炸
        Hand_Bomb_Jokers_Single,    // 王炸带一个
        Hand_Bomb_Jokers_Pair,      // 王炸带一对
        Hand_Bomb_Jokers_Two_Single	// 王炸带两单
    };
    PlayHand();
    PlayHand(Cards &cards);
    PlayHand(HandType type,Card::CardPoint pt,int extra);



    HandType handType();
    Card::CardPoint cardPoint();
    int extra();
    bool canBeat(const PlayHand& other);

private:
    void classify(Cards& cards);
    void judgeCardType();
    bool isPass();              // 放弃出牌
    bool isSingle();            // 单
    bool isPair();              // 对
    bool isTriple();            // 三个(相同)
    bool isTripleSingle();      // 三带一
    bool isTriplePair();        // 三带二
    bool isPlane();             // 飞机
    bool isPlaneTwoSingle();    // 飞机带两单
    bool isPlaneTwoPair();      // 飞机带2对
    bool isSeqPair();           // 连对
    bool isSeqSingle();         // 顺子
    bool isBomb();              // 炸弹
    bool isBombSingle();        // 炸弹带一单
    bool isBombPair();          // 炸弹带一对
    bool isBombTwoSingle();     // 炸弹带两单
    bool isBombJokers();        // 王炸
    bool isBombJokersSingle();  // 王炸带一单
    bool isBombJokersPair();    // 王炸带一对
    bool isBombJokersTwoSingle();   // 王炸带两单

private:

    HandType m_type;
    Card::CardPoint m_pt;
    int m_extra;
    QVector<Card::CardPoint> m_1Card;
    QVector<Card::CardPoint> m_2Card;
    QVector<Card::CardPoint> m_3Card;
    QVector<Card::CardPoint> m_4Card;



};

#endif // PLAYHAND_H
