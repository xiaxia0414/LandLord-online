#include "gamepanel.h"
#include "ui_gamepanel.h"
#include "buttongroup.h"
#include "QDebug"
#include "endingpanel.h"
#include <QMouseEvent>
#include <QPropertyAnimation>
#include   "cards.h"
#include "datamanager.h"
GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);
    ui->btnGroup->initButton();
    ui->btnGroup->selectPanel(ButtonGroup::Start);

    //背景图
    int num = QRandomGenerator::global()->bounded(10);
    QString path = QString(":/images/background-%1.png").arg(1);
    m_pixmap.load(path);
    //窗口大小
    this->setWindowTitle("p5斗地主");
    this->setFixedSize(1000,650);
    //实例化游戏控制对象
    gameControlInit();
    //玩家得分
    upDatePlayerScore();
    //切割游戏图片
    initCardMap();
    //初始化游戏中的按钮
    initBtnGroup();
    //初始化上下文环境
    initPlayerContext();

    initGameScene();

    initCountDown();

    timer = new QTimer(this);

    connect(timer,&QTimer::timeout,this,&GamePanel::onDisplayCard);
    m_animW = new AnimationWindow(this);
    //showEndingScorePanel();
    if(DataManager::getInstance()->isNetworkMode())
    {
    Communication* comm = DataManager::getInstance()->getCommunication();
    connect(comm,&Communication::startGame,this,[=](QByteArray msg)
            {
        initGamePanel(msg);
        gameStatusPrecess(GameControl::DispatchCard);

    });
    }
    //showAnimation(AnimationType::JokerBomb);
}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::gameControlInit()
{
    m_gameCtl = new GameControl(this);
    m_gameCtl->palyerInit();

    Robot* LeftRobot = m_gameCtl->getLeftRobot();
    Robot* rightRobot = m_gameCtl->getRightRobot();
    User* user = m_gameCtl->getUserPlayer();
    m_playerList<<LeftRobot<<rightRobot<<user;
    connect(m_gameCtl,&GameControl::playerStausChaged,this,&GamePanel::onPlayerStatusChaged);
    connect(m_gameCtl,&GameControl::notifyGrabLordBet,this,&GamePanel::onGrabLordBet);
    connect(m_gameCtl,&GameControl::gameSatusChanged,this,&GamePanel::gameStatusPrecess);
    connect(m_gameCtl,&GameControl::notifyPlayHand,this,&GamePanel::onDisposePlayHand);
    connect(LeftRobot,&Robot::notifyPickCards,this,&GamePanel::disPosCard);
    connect(rightRobot,&Robot::notifyPickCards,this,&GamePanel::disPosCard);
    connect(user,&User::notifyPickCards,this,&GamePanel::disPosCard);
    m_nameList <<LeftRobot->name().toUtf8();
    m_nameList <<user->name().toUtf8();
    m_nameList <<rightRobot->name().toUtf8();

}

void GamePanel::upDatePlayerScore()
{
    ui->scorePanel->setScore(m_playerList[0]->score(),m_playerList[1]->score(),m_playerList[2]->score());
}

void GamePanel::cropImage(QPixmap &pix, int x, int y, Card &c)
{
    QPixmap sub = pix.copy(x,y,m_cardSize.width(),m_cardSize.height());
    CardPanel* panel = new CardPanel(this);
    panel->setImage(sub,m_cardBackImg);
    panel->setCart(c);
    panel->hide();
    m_cardMap.insert(c, panel);
    connect(panel,&CardPanel::cardSelected,this,&GamePanel::onCardSelected);
}

void GamePanel::initCardMap()
{
    QPixmap pix(":/images/card.png");
    m_cardSize.setHeight(pix.height()/5);
    m_cardSize.setWidth(pix.width()/13);

    m_cardBackImg = pix.copy(2*m_cardSize.width(),4*m_cardSize.height(),m_cardSize.width(),m_cardSize.height());
    for(int i=0,suit=Card::Suit_Begin+1;suit<Card::Suit_end;suit++,i++)
    {
        for(int j=0,pt=Card::Card_Begin+1;pt<Card::Card_End;j++,pt++)
        {
            Card card((Card::CardPoint)pt,(Card::CardSuit)suit);
            cropImage(pix,j*m_cardSize.width(),i*m_cardSize.height(),card);
        }
    }

    Card c;
    c.setPoint(Card::Card_SJ);
    c.setSuit(Card::Suit_Begin);
    cropImage(pix,0,4*m_cardSize.height(),c);

    c.setPoint(Card::Card_BJ);
    cropImage(pix,m_cardSize.width(),4*m_cardSize.height(),c);
}

void GamePanel::initBtnGroup()
{
    ui->btnGroup->initButton();
    ui->btnGroup->selectPanel(ButtonGroup::Start);

    void (GamePanel::*startGame)() = &GamePanel::startGame;
    connect(ui->btnGroup,&ButtonGroup::startGame,this,startGame);
    connect(ui->btnGroup,&ButtonGroup::playHand,this,&GamePanel::onUserPlayHand);

    connect(ui->btnGroup,&ButtonGroup::pass,this,&GamePanel::onUserPass);
    connect(ui->btnGroup,&ButtonGroup::betPoint,this,[=](int point){
        m_gameCtl->getUserPlayer()->grabLordBet(point);
        ui->btnGroup->selectPanel(ButtonGroup::nullPage);
        //qDebug()<<"Btn"<<point;
        if(DataManager::getInstance()->isNetworkMode())
        {
            Message msg;
            msg.userName = DataManager::getInstance()->getUserName();
            msg.roomName = DataManager::getInstance()->getRoomName();
            msg.data1 = QByteArray::number(point);
            msg.reqcode = RequestCode::GrabLord;
            DataManager::getInstance()->getCommunication()->sendMessage(&msg);
        }
    });
}

void GamePanel::initPlayerContext()
{
    QRect cardsRect[]=
    {
        QRect(90,130,100,height()-200),
        QRect(rect().right()-190,130,100,height()-200),
        QRect(180,rect().bottom()-120,width()-500,100)

    };
    QRect playHandRect[]={
        QRect(260,150,100,100),
        QRect(rect().right()-360,150,100,100),
        QRect(150,rect().bottom()-290,width()-300,100)
    };
    QPoint roleImgPos[]=
    {
        QPoint(cardsRect[0].left()-120,cardsRect[0].height()/2-100),
        QPoint(cardsRect[1].right()-80,cardsRect[1].height()/2-100),
        QPoint(cardsRect[2].right()-80,cardsRect[2].top()-100)
    };
    int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for(int i= 0;i<m_playerList.size();i++)
    {
        PlayerContext context;
        context.align= i == index ? Horizontal : Vertical;
        context.isFrontSide = i == index ? true : false;
        context.cardRect = cardsRect[i];
        context.playHandRect = playHandRect[i];

        context.info = new QLabel(this);
        context.info->resize(181,92);
        context.info->hide();

        QRect rect = playHandRect[i];
        QPoint pt(rect.left()+(rect.width()-context.info->width())/2,
                  rect.top()+(rect.height()-context.info->height())/2);
        context.info->move(pt);

        context.roleImg = new QLabel(this);
        context.roleImg->resize(200,200);
        context.roleImg->hide();
        context.roleImg->move(roleImgPos[i]);
        context.roleImg->setScaledContents(true);
        m_contextMap.insert(m_playerList.at(i),context);
    }

}

void GamePanel::initGameScene()
{
    m_baseCard = new CardPanel(this);
    m_baseCard->setImage(m_cardBackImg,m_cardBackImg);

    m_moveCard = new CardPanel(this);
    m_moveCard->setImage(m_cardBackImg,m_cardBackImg);

    for(int i=0;i<3;i++)
    {
        CardPanel* panel = new CardPanel(this);
        panel->setImage(m_cardBackImg,m_cardBackImg);
        m_last3Card.push_back(panel);
        panel->hide();
    }

    m_baseCardPose = QPoint((width()-m_cardSize.width())/2,
                            (height()-m_cardSize.height())/2-100);
    m_baseCard->move(m_baseCardPose);
    m_moveCard->move(m_baseCardPose);

    int base = (width()-3*m_cardSize.width()-2*10)/2;
    for(int i=0;i<3;i++)
    {
        m_last3Card[i]->move(base + (m_cardSize.width()+10)*i,20);
    }
}

void GamePanel::gameStatusPrecess(GameControl::GameState status)
{
    m_gameStatus = status;
    if(m_gameStatus==GameControl::DispatchCard)
    {
        startDispatchCard();
    }
    else if(m_gameStatus==GameControl::CallingLord)
    {
        CardList last3Card = m_gameCtl->getSurplusCards().toCardList();
        for(int i=0;i<last3Card.size();i++)
        {
            QPixmap front= m_cardMap[last3Card.at(i)]->image();
            m_last3Card[i]->setImage(front,m_cardBackImg);
            m_last3Card[i]->hide();
        }
        m_gameCtl->startLordCard();

    }
    else if(m_gameStatus==GameControl::PlayingHand)
    {
        m_baseCard->hide();
        m_moveCard->hide();
        for(int i=0;i<m_last3Card.size();i++)
        {
            m_last3Card.at(i)->show();
        }
        for(int i=0;i<m_playerList.size();i++)
        {
            PlayerContext &context = m_contextMap[m_playerList.at(i)];
            context.info->hide();
            Player* player = m_playerList.at(i);
            qDebug()<<"become Lord"<<player->name()<<player->role();
            QPixmap pixmap = loadRoleImage(Player::Man,player->direction(),player->role());
            context.roleImg->setPixmap(pixmap);
            context.roleImg->show();
            context.roleImg->raise();
        }
    }

}

void GamePanel::startDispatchCard()
{
    //重置每张牌的属性
    for(auto it = m_cardMap.begin();it != m_cardMap.end();it++)
    {
        it.value()->setSelected(false);
        it.value()->setFrontSide(true);
        it.value()->hide();
    }
    for(int i=0;i<m_last3Card.size();i++)
    {
        m_last3Card.at(i)->hide();
    }
    int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for(int i =0;i<m_playerList.size();i++)
    {
        m_contextMap[m_playerList.at(i)].lastCards.clear();
        m_contextMap[m_playerList.at(i)].info->hide();
        m_contextMap[m_playerList.at(i)].roleImg->hide();
        m_contextMap[m_playerList.at(i)].isFrontSide = i == index ? true : false;
    }
    m_gameCtl->resetCarData();
    m_baseCard->show();

    ui->btnGroup->selectPanel(ButtonGroup::nullPage);

    timer->start(10);
}

void GamePanel::cardMoveStep(Player *player, int curPos)
{
    QRect cardRect = m_contextMap[player].cardRect;
    int unit[]=  // 每个玩家的单元步长
    {
        (m_baseCardPose.x()-cardRect.right())/100,
        (cardRect.left()-m_baseCardPose.x())/100,
        (cardRect.top()-m_baseCardPose.y())/100

    };
    //对应的每个牌的位置
    QPoint pos[] =
    {
        QPoint(m_baseCardPose.x()-curPos*unit[0],m_baseCardPose.y()),
        QPoint(m_baseCardPose.x()+curPos*unit[1],m_baseCardPose.y()),
        QPoint(m_baseCardPose.x(),m_baseCardPose.y()+curPos*unit[2])

    };

    int index = m_playerList.indexOf(player);
    m_moveCard->move(pos[index]);

    if(curPos == 0)
    {
        m_moveCard->show();
    }
    else if(curPos >= 100)
    {
        m_moveCard->hide();
    }
}

void GamePanel::disPosCard(Player *player, Cards &cards)
{
    CardList list = cards.toCardList();
    for(int i = 0; i<list.size(); i++)
    {
        CardPanel* panel = m_cardMap[list.at(i)];
        panel->setOwner(player);
    }
    upDatePlayerCards(player);

}

void GamePanel::upDatePlayerCards(Player *player)
{
    Cards cards = player->cards();
    CardList list = cards.toCardList();

    m_cardsRect = QRect();
    m_userCards.clear();

    int cardSapace = 20;
    QRect cardRect = m_contextMap[player].cardRect;
    for(int i=0;i<list.size();i++)
    {
        CardPanel* panel = m_cardMap[list.at(i)];
        panel->show();
        panel->raise();
        panel->setFrontSide(m_contextMap[player].isFrontSide);
        //qDebug()<<player->type()<<m_contextMap[player].isFrontSide;
        if(m_contextMap[player].align == Horizontal)
        {
            int leftX = cardRect.left()+(cardRect.width()-(list.size()-1)*cardSapace-panel->width())/2;
            int topY = cardRect.top()+(cardRect.height()-m_cardSize.height())/2;
            if(panel->isSelected())
            {
                topY -=10;
            }
            panel->move(leftX+cardSapace*i,topY);
            m_cardsRect = QRect(leftX,topY,cardSapace*i+m_cardSize.width(),m_cardSize.height());
            int curWidth = 0;
            if(list.size()-1==i)
            {
                curWidth = m_cardSize.width();
            }
            else
            {
                curWidth = cardSapace;
            }
            QRect cardRect(leftX+cardSapace*i,topY,curWidth,m_cardSize.height());
            m_userCards.insert(panel,cardRect);

        }
        else
        {
            int leftX = cardRect.left()+(cardRect.width()-m_cardSize.width())/2;
            int topY = cardRect.top()+(cardRect.height()-(list.size()-1)*cardSapace-panel->width())/2;
            panel->move(leftX,topY+cardSapace*i);
        }

    }

    QRect PlayCardRect = m_contextMap[player].playHandRect;
    Cards lastCards = m_contextMap[player].lastCards;
    if(!lastCards.isEmpty())
    {
        int playSpacing = 24;
        CardList lastCardList = lastCards.toCardList();
        CardList::ConstIterator itplayed = lastCardList.constBegin();
        for(int i=0; itplayed!=lastCardList.end();itplayed++,i++)
        {
            CardPanel* panel = m_cardMap[*itplayed];
            panel->setFrontSide(true);
            panel->raise();
            if(m_contextMap[player].align == Horizontal)
            {
                int leftbase = PlayCardRect.left() + (PlayCardRect.width()-(lastCardList.size()-1)*playSpacing-panel->width())/2;
                int top = PlayCardRect.top()+(PlayCardRect.height()-panel->height())/2;
                panel->move(leftbase+i*playSpacing,top);
            }
            else
            {
                int left = PlayCardRect.left() + (PlayCardRect.width()-panel->width())/2;
                int top = PlayCardRect.top();
                panel->move(left,i*playSpacing+top);
            }
        }
    }

    for(auto &item:m_contextMap)
    {
        item.roleImg->raise();
    }

}

void GamePanel::showAnimation(AnimationType type, int bet)
{
    if(type==AnimationType::LianDui)
    {
        m_animW->setFixedSize(250,150);
        m_animW->move((width()-m_animW->width())/2,200);
        m_animW->showSequence((AnimationWindow::Type)type);
    }
    else if(type==AnimationType::ShunZi)
    {
        m_animW->setFixedSize(250,150);
        m_animW->move((width()-m_animW->width())/2,200);
        m_animW->showSequence((AnimationWindow::Type)type);
    }
    else if(type==AnimationType::Plane)
    {
        m_animW->setFixedSize(360, 217);
        m_animW->move((width()-m_animW->width())/2,200);
        m_animW->showPlane1();
    }
    else if(type==AnimationType::Bomb)
    {
        m_animW->setFixedSize(360, 217);
        m_animW->move((width()-m_animW->width())/2,200);
        m_animW->showBomb1();
    }
    else if(type==AnimationType::JokerBomb)
    {
        m_animW->setFixedSize(360, 217);
        m_animW->move((width()-m_animW->width())/2,200);
        m_animW->showJokerBomb1();
    }
    else if(type==AnimationType::Bet)
    {
        m_animW->setFixedSize(160,98);
        m_animW->move((width()-m_animW->width())/2,(height()-m_animW->height())/2-140);
        m_animW->showBetScore(bet);
    }
    m_animW->show();

}

void GamePanel::onUserPlayerHand()
{

}

void GamePanel::onDisposePlayHand(Player *player, const Cards &cards)
{
    auto it = m_contextMap.find(player);
    it->lastCards = cards;

    Cards& myCards = const_cast<Cards&>(cards);
    PlayHand hand(myCards);
    PlayHand::HandType type = hand.handType();
    if(type==PlayHand::Hand_Plane||type==PlayHand::Hand_Plane_Two_Pair||type==PlayHand::Hand_Plane_Two_Single)
    {
        showAnimation(Plane);
    }
    else if(type ==PlayHand::Hand_Seq_Pair)
    {
        showAnimation(LianDui);
    }
    else if(type ==PlayHand::Hand_Seq_Single)
    {
        showAnimation(ShunZi);
    }
    else if(type ==PlayHand::Hand_Bomb)
    {
        showAnimation(Bomb);
    }
    else if(type == PlayHand::Hand_Bomb_Jokers)
    {
        showAnimation(JokerBomb);
    }
    if(cards.isEmpty())
    {
        it->info->setPixmap(QPixmap(":/images/pass.png"));
        it->info->show();
       // m_bgm->playPassMusic((BGMControl::RoleSex)player->getSex());
    }

    upDatePlayerCards(player);
}

void GamePanel::hidePlayerDropCards(Player *player)
{
    auto it = m_contextMap.find(player);
    if(it!=m_contextMap.end())
    {
        if(it->lastCards.isEmpty())
        {
            it->info->hide();
        }
        else
        {
            CardList list = it->lastCards.toCardList();
            for(auto last = list.begin();last !=list.end();last++)
            {
                m_cardMap[*last]->hide();
            }
        }
        it->lastCards.clear();
    }
}

QPixmap GamePanel::loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role)
{
    QVector<QString> lordMan;
    QVector<QString> lordWoman;
    QVector<QString> farmerMan;
    QVector<QString> farmerWoman;
    lordMan<<":/images/lord_man_1.png";
    lordWoman<<":/images/lord_woman_1.png";
    farmerMan<<":/images/farmer_man_1.png";
    farmerWoman<<":/images/farmer_woman_1.png";

    QImage image;
    int random = 0;
    if(sex == Player::Man&& role!=Player::Farmar)
    {
        image.load(lordMan.at(random));
    }
    if(sex == Player::Man&& role==Player::Farmar)
    {
        image.load(farmerMan.at(random));
    }
    if(sex == Player::Woman&& role==Player::Lord)
    {
        image.load(lordWoman.at(random));
    }
    if(sex == Player::Woman && role==Player::Farmar)
    {
        image.load(farmerWoman.at(random));
    }
    QPixmap pixmap;
    if(direct == Player::Left)
    {
        pixmap  = QPixmap::fromImage(image);
    }
    else
    {
        pixmap = QPixmap::fromImage(image.mirrored(true,false));
    }
    return pixmap;


}

void GamePanel::onCardSelected(Qt::MouseButton button)
{
    if(m_gameStatus == GameControl::DispatchCard||m_gameStatus==GameControl::CallingLord)
    {
        return;
    }

    CardPanel* panel = static_cast<CardPanel*>(sender());
    if(panel->owner()!=m_gameCtl->getUserPlayer())
    {
        return;
    }
    m_curSelCard = panel;
    if(button == Qt::LeftButton)
    {
        panel->setSelected(!panel->isSelected());
        upDatePlayerCards(panel->owner());
        QSet<CardPanel*>::const_iterator it = m_curSelCards.find(panel);
        if(it == m_curSelCards.constEnd())
        {
            m_curSelCards.insert(panel);
        }
        else
        {
            m_curSelCards.erase(it);
        }

    }
    else if(button == Qt::RightButton)
    {
        onUserPlayHand();
    }
}

void GamePanel::onUserPlayHand()
{

    if(m_gameStatus!=GameControl::PlayingHand)
    {
        return;
    }
    if(m_gameCtl->currentPlayer()!=m_gameCtl->getUserPlayer())
    {
        return;
    }
    if(m_curSelCards.isEmpty())
    {
        return;
    }

    Cards cs;
    for(auto it = m_curSelCards.begin();it!=m_curSelCards.end();it++)
    {
        Card card = (*it)->card();
        cs.add(card);
    }
    PlayHand hand(cs);
    PlayHand::HandType type = hand.handType();
    if(type==PlayHand::Hand_Unknown)
    {
        return;
    }

    if(m_gameCtl->pendPlayer()!=m_gameCtl->getUserPlayer())
    {

        Cards cards = m_gameCtl->pendCards();
        if(!hand.canBeat(PlayHand(cards)))
        {
            return;
        }

    }
    m_countDown->stopCountDown();
    m_gameCtl->getUserPlayer()->playHand(cs);
    notifyOtherPlayHand(cs);
    m_curSelCards.clear();
}

void GamePanel::onUserPass()
{
    m_countDown->stopCountDown();
    Player* curPlayer = m_gameCtl->currentPlayer();
    Player* userPlayer = m_gameCtl->getUserPlayer();
    if(curPlayer!=userPlayer)
    {
        return;
    }
    Player* pendPlayer = m_gameCtl->pendPlayer();
    if(pendPlayer==userPlayer||pendPlayer==nullptr)
    {
        return;
    }
    Cards empty;
    userPlayer->playHand(empty);
    notifyOtherPlayHand(empty);
    for(auto it=m_curSelCards.begin();it!=m_curSelCards.end();it++)
    {
        (*it)->setSelected(false);
    }
    m_curSelCards.clear();

    upDatePlayerCards(userPlayer);
}

void GamePanel::showEndingScorePanel()
{
    bool islord = m_gameCtl->getUserPlayer()->role() == Player::Lord ? true : false;
    bool isWin = m_gameCtl->getUserPlayer()->isWin();
    EndingPanel* panel = new EndingPanel(islord, isWin, this);
    panel->show();
    panel->move((width() - panel->width()) / 2, -panel->height());
    panel->setPlayerScore(m_gameCtl->getLeftRobot()->score(),
                          m_gameCtl->getRightRobot()->score(),
                          m_gameCtl->getUserPlayer()->score());
    panel->setPlayerName(m_nameList);
    // if(isWin)
    // {
    //     m_bgm->playEndingMusic(true);
    // }
    // else
    // {
    //     m_bgm->playEndingMusic(false);
    // }

    QPropertyAnimation *animation = new QPropertyAnimation(panel, "geometry", this);
    // 动画持续的时间
    animation->setDuration(1500);   // 1.5s
    // 设置窗口的起始位置和终止位置
    animation->setStartValue(QRect(panel->x(), panel->y(), panel->width(), panel->height()));
    animation->setEndValue(QRect((width() - panel->width()) / 2, (height() - panel->height()) / 2,
                                 panel->width(), panel->height()));
    // 设置窗口的运动曲线
    animation->setEasingCurve(QEasingCurve(QEasingCurve::OutBounce));
    // 播放动画效果
    animation->start();

    // 处理窗口信号
    connect(panel, &EndingPanel::continueGame, this, [=]()
            {
                panel->close();
                panel->deleteLater();
                animation->deleteLater();
                ui->btnGroup->selectPanel(ButtonGroup::nullPage);
                if(DataManager::getInstance()->isNetworkMode())
                {
                    Message msg;
                    if(DataManager::getInstance()->isManualMode())
                    {

                        msg.roomName =  DataManager::getInstance()->getRoomName();
                        msg.reqcode = RequestCode::Continue;
                    }
                    else
                    {
                        msg.reqcode = RequestCode::AutoRoom;
                    }
                msg.userName= DataManager::getInstance()->getUserName();
                DataManager::getInstance()->getCommunication()->sendMessage(&msg);
                }
                else
                {
                    gameStatusPrecess(GameControl::DispatchCard);
                    // m_bgm->startBGM(80);
                }

    });
}

void GamePanel::initCountDown()
{
    m_countDown = new CountDown(this);
    m_countDown->move((width()-m_countDown->width())/2,(height()-m_countDown->height())/2+30);
    connect(m_countDown,&CountDown::notMuchTime,this,[=]{
        //播放提示音
    });
    connect(m_countDown,&CountDown::timeout,this,&GamePanel::onUserPass);
    User *userPlayer = m_gameCtl->getUserPlayer();
    connect(userPlayer,&User::startCountDown,this,[=](){
        if(m_gameCtl->pendPlayer()!=userPlayer&&m_gameCtl->pendPlayer()!=nullptr){
            m_countDown->showCountDown();
        }
    });
}

void GamePanel::onDisplayCard()
{
    static int curMovePos = 0;
    Player* curPlayer = m_gameCtl->currentPlayer();
    if(curMovePos >=100)
    {
        Card card = m_gameCtl->takeOneCard();
        curPlayer->storeDispatchCard(card);
        Cards cs(card);
        disPosCard(curPlayer,cs);
        m_gameCtl->setCurrentPlayer(curPlayer->nextPlayer());
        curMovePos = 0;
        cardMoveStep(curPlayer,curMovePos);
        if(m_gameCtl->getSurplusCards().cardCount()==3)
        {
            timer->stop();
            gameStatusPrecess(GameControl::CallingLord);
            return;
        }
    }
    cardMoveStep(curPlayer,curMovePos);
    curMovePos +=15;
}

void GamePanel::onPlayerStatusChaged(Player *player, GameControl::PlayerStatus status)
{
    //qDebug()<<player->name()<<status;
    if(status==GameControl::ThinkingForCallLord)
    {
        if(player == m_gameCtl->getUserPlayer())
        {
            ui->btnGroup->selectPanel(ButtonGroup::CallLord);
        }
    }
    else if(status == GameControl::ThinkingForPlayHand)
    {
        hidePlayerDropCards(player);
        if(player == m_gameCtl->getUserPlayer())
        {
            Player* pendPlayer = m_gameCtl->pendPlayer();
            if(pendPlayer == m_gameCtl->getUserPlayer()||pendPlayer==nullptr)
            {
                ui->btnGroup->selectPanel(ButtonGroup::PlayCard);
            }
            else
            {
                ui->btnGroup->selectPanel(ButtonGroup::passOrPlay);
            }
        }
        else
        {
            ui->btnGroup->selectPanel(ButtonGroup::nullPage);
        }
    }
    else if(status == GameControl::Winning)
    {
        m_contextMap[m_gameCtl->getLeftRobot()].isFrontSide = true;
        m_contextMap[m_gameCtl->getRightRobot()].isFrontSide = true;
        upDatePlayerCards(m_gameCtl->getRightRobot());
        upDatePlayerCards(m_gameCtl->getLeftRobot());
        upDatePlayerScore();
        m_gameCtl->setCurrentPlayer(player);
        showEndingScorePanel();
        if(DataManager::getInstance()->isNetworkMode())
        {
        Message msg{
            .userName = DataManager::getInstance()->getUserName(),
            .roomName = DataManager::getInstance()->getRoomName(),
                .data1 = QByteArray::number(m_gameCtl->getUserPlayer()->score()),
                .reqcode = RequestCode::GameOver
        };
        DataManager::getInstance()->getCommunication()->sendMessage(&msg);
        }
    }

}

void GamePanel::onGrabLordBet(Player *player, int bet, bool flag)
{
    PlayerContext context = m_contextMap[player];
    //qDebug()<<bet;
    if(bet == 0)
    {
        context.info->setPixmap(QPixmap(":/images/buqinag.png"));
    }
    else
    {
        if(flag)
        {
            context.info->setPixmap(QPixmap(":/images/jiaodizhu.png"));
        }
        else
        {
            context.info->setPixmap(QPixmap(":/images/qiangdizhu.png"));
        }
    }
    context.info->show();
    showAnimation(Bet,bet);
}

void GamePanel::initGamePanel(QByteArray msg)
{
    int index = 1;
    // 格式: 用户名-次序-分数#用户名-次序-分数#用户名-次序-分数#
    qDebug() << "==============msg: " << msg;
    orderMap order;
    auto lst = msg.left(msg.length()-1).split('#');
    for(const auto& item : lst)
    {
        auto sub = item.split('-');
        order.insert(sub.at(1).toInt(), QPair(sub.at(0), sub.at(2).toInt()));
        if(sub.at(0) == DataManager::getInstance()->getUserName())
        {
            index = sub.at(1).toInt();
        }
    }
    updatePlayerInfo(order);
    // 开始游戏
    startGame(index);
}

void GamePanel::updatePlayerInfo(orderMap &info)
{
    int lscore,rscore,mscore;
    QByteArray left,right,mid;
    QByteArray current = DataManager::getInstance()->getUserName();
    if(current == info.value(1).first)
    {
        mid = info.value(1).first;
        right = info.value(2).first;
        left = info.value(3).first;
        mscore = info.value(1).second;
        rscore = info.value(2).second;
        lscore = info.value(3).second;
    }
    else if(current == info.value(2).first)
    {
        mid = info.value(2).first;
        right = info.value(3).first;
        left = info.value(1).first;
        mscore = info.value(2).second;
        rscore = info.value(3).second;
        lscore = info.value(1).second;
    }
    else if(current == info.value(3).first)
    {
        mid = info.value(3).first;
        right = info.value(1).first;
        left = info.value(2).first;
        mscore = info.value(3).second;
        rscore = info.value(1).second;
        lscore = info.value(2).second;
    }

    ui->scorePanel->setPlayerName(left,mid,right);
    m_gameCtl->getLeftRobot()->setScore(lscore);
    m_gameCtl->getRightRobot()->setScore(rscore);
    m_gameCtl->getUserPlayer()->setScore(mscore);


    m_nameList.clear();
    m_nameList<<left<<mid<<right;


}

void GamePanel::startGame()
{
    startGame(1);
}

void GamePanel::startGame(int index)
{
    m_gameCtl->setCurrentPlayer(index);
    ui->btnGroup->selectPanel(ButtonGroup::nullPage);
    //m_gameCtl->clearPlayerScore();
    upDatePlayerScore();
    gameStatusPrecess(GameControl::DispatchCard);
}

void GamePanel::notifyOtherPlayHand(Cards &cs)
{
     DataManager* instance =DataManager::getInstance();
     if(instance->isNetworkMode())
     {
         Message msg;
         msg.userName = instance->getUserName();
         msg.roomName = instance->getRoomName();
         msg.data1 = QByteArray::number(cs.cardCount());
         QDataStream stream(&msg.data2, QIODevice::ReadWrite);
         // 将cs转换为容器类型
         CardList cds = cs.toCardList();
         for(const auto& item : cds)
         {
             stream << item;
         }
         msg.reqcode = RequestCode::PlayAHand;
         instance->getCommunication()->sendMessage(&msg);
     }
}

void GamePanel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(rect(),m_pixmap);
}

void GamePanel::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<event->button()<<event->pos();
    if(event->button()==Qt::NoButton)
    {
         //qDebug()<<"LeftButton";
        QPoint pt = event->pos();
        if(!m_cardsRect.contains(pt))
        {
            m_curSelCard = nullptr;
        }
        else
        {
            QList<CardPanel*> list = m_userCards.keys();
            for(int i=0;i<list.size();i++)
            {
                CardPanel* panel = list.at(i);
                if(m_userCards[panel].contains(pt)&&m_curSelCard!=panel)
                {
                    panel->clicked();
                    m_curSelCard = panel;
                    //qDebug()<<panel->card().point();
                }

            }
        }
    }
}

void GamePanel::closeEvent(QCloseEvent *event)
{
    emit panelColsed();
    if(DataManager::getInstance()->isNetworkMode())
    {
        Message msg;
        msg.reqcode = RequestCode::LeaveRoom;
        msg.userName = DataManager::getInstance()->getUserName();
        msg.roomName = DataManager::getInstance()->getRoomName();
        DataManager::getInstance()->getCommunication()->sendMessage(&msg);
    }
    event->accept();
    deleteLater();
}
