#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include "QPixmap"
#include "QPainter"
#include "QRandomGenerator"
#include "gamecontrol.h"
#include "cardpanel.h"
#include "QLabel"
#include <QMap>
#include <QTimer>
#include "animationwindow.h"
#include "countdown.h"
QT_BEGIN_NAMESPACE
namespace Ui { class GamePanel; }
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:
     using orderMap = QMap<int,QPair<QByteArray,int>>;
    GamePanel(QWidget *parent = nullptr);
    ~GamePanel();
    enum AnimationType{ShunZi,LianDui,Plane,Bomb,JokerBomb,Bet};
    void gameControlInit();
    void upDatePlayerScore();
    void cropImage(QPixmap &pix,int x,int y,Card& c);
    void initCardMap();
    void initBtnGroup();
    void initPlayerContext();
    void initGameScene();
    void gameStatusPrecess(GameControl::GameState status);
    void startDispatchCard();
    void cardMoveStep(Player* player,int curPos);
    void disPosCard(Player* player,Cards &cards);
    void upDatePlayerCards(Player* player);
    void showAnimation(AnimationType type,int bet=0);
    void onUserPlayerHand();
    void onDisposePlayHand(Player* player,const Cards& cards);
    void hidePlayerDropCards(Player* player);
    QPixmap loadRoleImage(Player::Sex sex,Player::Direction direct,Player::Role role);
    void onCardSelected(Qt::MouseButton button);
    void onUserPlayHand();
    void onUserPass();
    void showEndingScorePanel();
    void initCountDown();
    void onDisplayCard();
    void onPlayerStatusChaged(Player* player,GameControl::PlayerStatus status);
    void onGrabLordBet(Player *player,int bet,bool flag);
    void initGamePanel(QByteArray msg);
    void updatePlayerInfo(orderMap &info);
    void startGame();
    void startGame(int index);
    void notifyOtherPlayHand(Cards& cs);

private:
    enum CardAllign{Horizontal,Vertical};
    struct PlayerContext
    {
        QRect cardRect;//扑克牌显示区域
        QRect playHandRect;//出牌的区域
        CardAllign align;//扑克牌对齐方式
        bool isFrontSide;//正面还是反面
        QLabel* info;//提示信息
        QLabel* roleImg;//玩家头像
        Cards lastCards;//玩家刚打出的牌
    };

    Ui::GamePanel *ui;
    QPixmap m_pixmap;
    GameControl *m_gameCtl;
    QVector<Player*> m_playerList;
    QMap<Card, CardPanel*> m_cardMap;
    QSize m_cardSize;
    QPixmap m_cardBackImg;
    QMap<Player*, PlayerContext> m_contextMap;
    CardPanel* m_baseCard;
    CardPanel* m_moveCard;
    QVector<CardPanel*> m_last3Card;
    QPoint m_baseCardPose;
    GameControl::GameState m_gameStatus;
    QTimer *timer;
    AnimationWindow* m_animW;
    CardPanel* m_curSelCard;
    QSet<CardPanel*> m_curSelCards;
    QRect m_cardsRect;
    QHash<CardPanel*,QRect> m_userCards;
    CountDown* m_countDown;
    QByteArrayList m_nameList;

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void panelColsed();
    // QWidget interface

    // QWidget interface


};


#endif // GAMEPANEL_H
