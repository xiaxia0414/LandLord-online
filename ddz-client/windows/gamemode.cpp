#include "gamemode.h"
#include "joinroom.h"
#include "ui_gamemode.h"
#include "gamepanel.h"
#include "codec.h"
#include "datamanager.h"
#include "gamepanel.h"
GameMode::GameMode(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameMode)
{
    this->setFixedSize(1024,768);
    this->setWindowTitle("游戏模式");
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);


//#if 0

    Communication *comm = DataManager::getInstance()->getCommunication();
    connect(comm,&Communication::playerCount,this,[=](int count){
        QString tip =QString("当前<%1>房间人数:%2人,正在等待其他玩家进入,请稍等")
                          .arg(DataManager::getInstance()->getRoomName().data()).arg(count-1);
        ui->tip->setText(tip);
    });

    connect(comm,&Communication::somebodyLeave ,this,[=](int count){
        QString tip =QString("当前<%1>房间人数:%2人,正在等待其他玩家进入,请稍等")
                          .arg(DataManager::getInstance()->getRoomName().data()).arg(count);
        ui->tip->setText(tip);
    });
    connect(comm, &Communication::startGame, this, [=](QByteArray msg){
        this->hide();
        GamePanel* panel = new GamePanel;
        connect(panel, &GamePanel::panelColsed, this, &GameMode::show);
        panel->show();
        // 初始化数据
        panel->initGamePanel(msg);
        // 断开信号槽连接
        disconnect(comm, &Communication::startGame, this, nullptr);
    });
//#endif
    connect(ui->singleMode,&QPushButton::clicked,this,[=](){
        GamePanel *panel = new GamePanel();
        panel->show();
        connect(panel,&GamePanel::panelColsed,this,[=](){
            this->show();
        });
        this->hide();
        DataManager::getInstance()->setGameMode(DataManager::Single);
    });

    connect(ui->netMode,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(1);
        DataManager::getInstance()->setGameMode(DataManager::Network);
    });
    connect(ui->backToGameMode,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
//#if 0
    connect(ui->autoCreatRoom,&QPushButton::clicked,this,[=](){
        Message msg;
        msg.userName = DataManager::getInstance()->getUserName();
        msg.reqcode = RequestCode::AutoRoom;
        DataManager::getInstance()->getCommunication()->sendMessage(&msg);
        DataManager::getInstance()->setRoomMode(DataManager::Auto);


    });
    connect(ui->manualCreatRoom,&QPushButton::clicked,this,[=](){
        JoinRoom* room= new JoinRoom(DialogType::Manual);
        room->setWindowTitle("创建房间");
        room->show();
    });
    connect(ui->searchRoom,&QPushButton::clicked,this,[=](){
        JoinRoom* room= new JoinRoom(DialogType::Search);
        room->setWindowTitle("搜索房间");
        room->show();
    });
//#endif
}

GameMode::~GameMode()
{
    delete ui;
}

void GameMode::closeEvent(QCloseEvent *event)
{
//#if 0
    if(DataManager::getInstance()->isNetworkMode())
    {
        Message msg;
        msg.reqcode = RequestCode::Goodbye;
        msg.userName = DataManager::getInstance()->getUserName();
        msg.roomName = DataManager::getInstance()->getRoomName();
        DataManager::getInstance()->getCommunication()->sendMessage(&msg);
        DataManager::getInstance()->getCommunication()->stopLoop();
    }
//#endif
}

void GameMode::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/gameModeBackground.jpg");  // 指定背景图片路径
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}
