//
// Created by Hasee on 2024/4/27.
//
#include <netinet/in.h>
#include "Communication.h"
#include "Codec.h"
#include "RsaCrypto.h"
#include "Log.h"
#include "AesCrypto.h"
#include "JsonParse.h"
#include "RoomList.h"
void Communication::parseRequest(Buffer *buf)
{
   std::string data =  buf->data(sizeof(int));
   int length = *(int*)data.data();
   length = ntohl(length);
   data = buf->data(length);
    if(m_aes)
    {
       data = m_aes->decrypt(data);
    }
    sendCallback myfunc = sendMessage;
    Message resMsg;
   Codec codec(data);
   shared_ptr<Message> ptr = codec.decodeMsg();
    Debug("recvReqcode:%d",(int)resMsg.rescode);
    Debug("Reqcode:%d,%d,%d",RequestCode::AesFenfa,RequestCode::UserLogin,RequestCode::Register);
    switch (ptr->reqcode) {
        case RequestCode::AesFenfa:
            handleAesFenfa(ptr.get(),resMsg);
            Debug("handleAesFenfa");
            break;
        case RequestCode::UserLogin:
            Debug("handleLogin");
            handleLogin(ptr.get(),resMsg);
            break;
        case RequestCode::Register:
            handleRegister(ptr.get(),resMsg);
            Debug("handleRegister");
            break;
        case RequestCode::AutoRoom:
        case RequestCode::ManualRoom:
            Debug("ManualRoom");
            handleAddRoom(ptr.get(),resMsg);
            myfunc = std::bind(&Communication::readyForPlay,this,resMsg.roomName,std::placeholders::_1);
            break;
        case RequestCode::GrabLord:
            resMsg.data1 = ptr->data1;
            resMsg.rescode = RespondCode::OtherGrabLord;
            myfunc = std::bind(&Communication::notifyOtherPlayers,this,std::placeholders::_1,ptr->roomName,ptr->userName);
            break;
        case RequestCode::PlayAHand:
            resMsg.data1 = ptr->data1;
            resMsg.data2 = ptr->data2;
            resMsg.rescode = RespondCode::OtherPlayHand;
            myfunc = std::bind(&Communication::notifyOtherPlayers, this, std::placeholders::_1, ptr->roomName, ptr->userName);
            break;
        case RequestCode::GameOver:
            handleGameOver(ptr.get());
            myfunc = nullptr;
            break;
        case RequestCode::Continue:
            restartGame(ptr.get());
            myfunc = nullptr;
            break;
        case RequestCode::SearchRoom:
            handleSearchRoom(ptr.get(),resMsg);
            break;
        case RequestCode::LeaveRoom:
            handleLeaveRoom(ptr.get(),resMsg);
            myfunc = nullptr;
            break;
        case RequestCode::Goodbye:
            handleGoodBye(ptr.get());
            myfunc = nullptr;
        default:
            Debug("handFailed");
            break;
    }
    Debug("sendRescode:%d",resMsg.rescode);
    if(myfunc!= nullptr) {
        codec.reLoad(&resMsg);
        myfunc(codec.encodeMsg());
    }
}

void Communication::handleAesFenfa(Message *reqMsg,Message& resMsg) {

    RsaCrypto rsa;
    rsa.parseStringToKey(m_redis->rsaSecKey("PrivateKey"),RsaCrypto::PrivateKey);

    m_aesKey = rsa.priKeyDecrypt(reqMsg->data1);
    Hash h(HashType::Sha224);
    h.addData(m_aesKey);
    std::string res = h.result();
    resMsg.rescode = RespondCode::AesVerifyOK;
    if(reqMsg->data2!=res)
    {
        Debug("私钥校验失败--aes");
        resMsg.rescode = RespondCode::Failed;
        resMsg.data1 = "Aes 密钥哈希校验失败";
    }
    else
    {
        m_aes = new AesCrypto(AesCrypto::AES_CBC_256, m_aesKey);
        Debug("私钥校验成功--aes");

    }
    Codec codec(&resMsg);

   // sendMessage(codec.encodeMsg());
}

void Communication::setCallback(Communication::sendCallback fun1, Communication::deleteCallback fun2) {
    sendMessage = fun1;
    disconnect = fun2;
}

void Communication::handleRegister(Message *reqMsg, Message &resMsg) {
    //查询数据库
    char sql[1024];
    sprintf(sql,"SELECT * FROM `user` WHERE `name`='%s';",resMsg.userName.data());
    bool flag = m_mysql->query(sql);
    Debug("flag = %d, sql = %s", flag, sql);
    if(flag&&!m_mysql->next())
    {
        // 将注册信息写入到数据库
        m_mysql->transaction();
        sprintf(sql, "INSERT INTO `user`(name, passwd, phone, date) VALUES('%s', '%s', '%s', NOW());",
                reqMsg->userName.data(), reqMsg->data1.data(), reqMsg->data2.data());
        bool fl1 = m_mysql->update(sql);
        Debug("fl1 = %d, sql = %s", fl1, sql);
        sprintf(sql, "INSERT INTO `information`(name, score, status) VALUES('%s', 0, 0);",
                reqMsg->userName.data());
        bool fl2 = m_mysql->update(sql);
        Debug("fl2 = %d, sql = %s", fl2, sql);
        if(fl1 && fl2)
        {
            m_mysql->commit();
            resMsg.rescode = RespondCode::RegisterOk;
        }
        else
        {
            m_mysql->rollback();
            resMsg.rescode = RespondCode::Failed;
            resMsg.data1 = "数据库插入数据失败!";
        }
    }
    else
    {
        resMsg.rescode = RespondCode::Failed;
        resMsg.data1 = "用户已存在";
    }
}

Communication::Communication() {
    JsonParse json;
    std::shared_ptr<DBInfo> info = json.getDatabaseInfo(JsonParse::Mysql);
    m_mysql = new MySqlConn;
    bool flag= m_mysql->connect(info->user,info->dbname,info->passwd,info->ip,info->port);
    assert(flag);
    m_redis = new Room;
    flag =  m_redis->initEnvironment();
    assert(flag);
}

void Communication::handleLogin(Message *reqMsg, Message &resMsg) {
    Debug("StartHandleLogin");
    char sql[1024];
    sprintf(sql, "SELECT * FROM `user` WHERE `name`='%s' AND passwd='%s' AND (select  count(*) from information where name = '%s' AND status=0);", reqMsg->userName.data(),
            reqMsg->data1.data(), reqMsg->userName.data());
    Debug("StartHandleLogin2");
    bool flag = m_mysql->query(sql);
    Debug("StartSelect:%d",flag);
    if(flag && m_mysql->next())
    {
        m_mysql->transaction();
        sprintf(sql, "update `information` set status=1 WHERE `name`='%s'", reqMsg->userName.data());
        bool flag1 = m_mysql->update(sql);
        if(flag1)
        {
            m_mysql->commit();
            resMsg.rescode = RespondCode::LoginOk;
            Debug("用户登录成功了.....................................");
            return;
        }
        m_mysql->rollback();
    }
    Debug("Selected");
    resMsg.rescode = RespondCode::Failed;
    resMsg.data1 = "用户名或者密码错误, 或者当前用户已经成功登录了...";
}

Communication::~Communication() {
    if(m_redis)
    {
        delete m_redis;
    }
    if(m_aes)
    {
        delete m_aes;
    }
    if(m_mysql)
    {
        delete m_mysql;
    }
}

void Communication::handleAddRoom(Message *reqMsg, Message &resMsg) {

    std::string  oldRoom = m_redis->whereAmI(reqMsg->userName);
    int score = m_redis->PlayerScore(oldRoom,reqMsg->userName);
    if(oldRoom != std::string())
    {
        m_redis->leaveRoom(oldRoom, reqMsg->userName);
        RoomList::getInstance()->removePlayer(oldRoom, reqMsg->userName);
    }
    bool flag = true;
    std::string roomName;
    if(reqMsg->reqcode == RequestCode::AutoRoom)
    {
       roomName= m_redis->joinRoom(resMsg.userName);
    }
    else
    {
        roomName = reqMsg->roomName;
        flag = m_redis->joinRoom(resMsg.userName,roomName);
    }
    if(flag)
    {
        if(score == 0)
        {
             // 查询mysql, 并将其存储到redis中
            std::string sql = "select score from information where name = '" + reqMsg->userName + "'";
            bool fl = m_mysql->query(sql);
            assert(fl);
            m_mysql->next();
            score = std::stoi(m_mysql->value(0));
        }
        m_redis->updatePlayerScore(roomName, reqMsg->userName, score);
        RoomList* room = RoomList::getInstance();
        room->addUser(roomName,reqMsg->userName,sendMessage);

        resMsg.rescode = RespondCode::JoinRoomOK;
        resMsg.data1 = to_string( m_redis->getPlayerCount(roomName));
        resMsg.roomName = roomName;

    }
    else
    {
        resMsg.rescode = RespondCode::Failed;
        resMsg.data1 = "抱歉，加入房间失败";
    }

}

void Communication::readyForPlay(std::string roomName, std::string data) {
    RoomList* instance = RoomList::getInstance();
    UserMap players = instance->getPlayers(roomName);
    for(auto item :players)
    {
        item.second(data);
        Debug("当前房间人数: %ld",players.size());

    }
    if(players.size()==3)
    {
        startGame(roomName,players);
    }

}

void Communication::initCards() {
    m_cards.clear();
    // 花色
    for(int i=1; i<=4; ++i)
    {// 点数
        for(int j=1; j<=13; ++j)
        {
            m_cards.insert(make_pair(i, j));
        }
    }
    m_cards.insert(make_pair(0, 14));
    m_cards.insert(make_pair(0, 15));

}

std::pair<int, int> Communication::takeOneCard() {
    // 生成随机数
    // 创建随机设备对象
    std::random_device rd;
    // 创建随机数生成对象
    std::mt19937 gen(rd());
    // 创建随机数分布对象 -> 均匀分布
    std::uniform_int_distribution<int> dis(0, m_cards.size()-1);
    int randNum = dis(gen);
    auto it = m_cards.begin();
    // 遍历map容器
    for(int i=0; i<randNum; ++i, ++it);
    m_cards.erase(it);
    return *it;
}

void Communication::dealCards(UserMap players) {
    Message msg;
    // 洗牌
    initCards();
    std::string &all = msg.data1;
    for(int i=0; i<51; ++i)
    {
        // 取出一张牌
        auto card = takeOneCard();
        std::string sub = std::to_string(card.first) + "-" + std::to_string(card.second) + "#";
        all += sub;
    }
    // 剩余的3张底牌
    std::string &lastCard = msg.data2;
    for(const auto& item : m_cards)
    {
        std::string sub = std::to_string(item.first) + "-" + std::to_string(item.second) + "#";
        lastCard += sub;
    }
    msg.rescode = RespondCode::DealCards;
    Codec codec(&msg);
    // 遍历当前房间中的所有玩家
    for(const auto& player : players)
    {
        player.second(codec.encodeMsg());
    }
}

void Communication::startGame(std::string roomName, UserMap players) {
    // 房间满了
    // 发牌数据
    dealCards(players);
    // 通知客户端可以开始游戏了
    Message msg;
    msg.rescode = RespondCode::StartGame;
    // data1: userName-次序-分数, 次序的规则: 谁分数高, 谁优先抢地主
    msg.data1 = m_redis->playersOrder(roomName);
    Codec codec(&msg);
    for(const auto& item : players)
    {
        item.second(codec.encodeMsg());
    }
}

void Communication::notifyOtherPlayers(std::string data, std::string roomName, std::string userName) {
    auto players = RoomList::getInstance()->getInstance()->getPartners(roomName,userName);
    for(const auto& item : players)
    {
        item.second(data);
        Debug("++++++++++++++房间名：%s，玩家名字：%s",roomName.data(),userName.data());
        Debug(" other player name: %s", item.first.data());
    }
}

void Communication::handleGameOver(Message *reqMsg) {
    int score = std::stoi(reqMsg->data1);
    m_redis->updatePlayerScore(reqMsg->roomName,reqMsg->userName,score);
    char sql[1024];
    sprintf(sql, "update information set score=%d where name='%s'", score, reqMsg->userName.data());
    m_mysql->update(sql);
}

void Communication::restartGame(Message *reqMsg) {
    auto player = RoomList::getInstance()->getPlayers(reqMsg->roomName);
    if(player.size()== 3)
    {
        RoomList::getInstance()->removeRoom(reqMsg->roomName);

    }
    RoomList::getInstance()->addUser(reqMsg->roomName,reqMsg->userName,sendMessage);
    player = RoomList::getInstance()->getPlayers(reqMsg->roomName);
    if(player.size()==3)
    {
        startGame(reqMsg->roomName,player);
    }
}

void Communication::handleSearchRoom(Message *reqMsg, Message &resMsg) {
    bool flag = m_redis->searchRoom(reqMsg->roomName);
    resMsg.rescode = RespondCode::SearchRoomOK;
    reqMsg->data1 = flag ? "true" : "false";
}

void Communication::handleLeaveRoom(Message *reqMsg, Message &resMsg) {
    m_redis->leaveRoom(reqMsg->roomName,reqMsg->userName);
    RoomList::getInstance()->removePlayer(reqMsg->roomName,reqMsg->userName);
    resMsg.rescode = RespondCode::OtherLeaveRoom;
    auto players = RoomList::getInstance()->getPlayers(reqMsg->roomName);
    for(auto item : players)
    {
        Codec codec(&resMsg);
        item.second(codec.encodeMsg());
    }
}

void Communication::handleGoodBye(Message *reqMsg) {
    char sql[10240] = {0};
    sprintf(sql, "update information set status = 0 where name = '%s'", reqMsg->userName.data());
    m_mysql->update(sql);
    Debug("===========sql = %s, userName: %s", sql, reqMsg->userName.data());
    // 和客户端断开连接
    disconnect();
}
