//
// Created by Hasee on 2024/5/17.
//
#include <random>
#include "Room.h"
#include "JsonParse.h"
#include "Log.h"
bool Room::initEnvironment() {
    //tcp://127.0.0.1:6379
    JsonParse json;
    auto info = json.getDatabaseInfo(JsonParse::Redis);
    std::string connStr = "tcp://" + info->ip + ":" + std::to_string(info->port);
    m_redis = new sw::redis::Redis(connStr);
    // 测试连接
    if(m_redis->ping() == "PONG")
    {
        Debug("成功连接了 redis 服务器...");
        return true;
    }
    return false;
}
// 清空数据
void Room::clear(){
    m_redis->flushdb();
}
Room::~Room() {
    if(m_redis)
    {
        delete m_redis;
    }
}
// 保存RSA密钥对
// key(filed, value)
// hset
void Room::saveRsaSecKey(std::string field, std::string value)
{
    m_redis->hset("RSA", field, value);
}

std::string Room::rsaSecKey(std::string field)
{
    auto value = m_redis->hget("RSA", field);
    if(value.has_value())
    {
        return value.value();
    }
    return std::string();
}

std::string Room::joinRoom(std::string userName) {
    std::optional<std::string> room;
    do{
        // scard
        if(m_redis->scard(TwoPlayer) > 0)
        {
            // srandmember
            room = m_redis->srandmember(TwoPlayer);
            break;
        }
        if(m_redis->scard(OnePlayer) > 0)
        {
            // srandmember
            room = m_redis->srandmember(OnePlayer);
            break;
        }
        // 添加新的房间
        room = getNewRoomName();
    }while(0);
    // 加入到某个房间中
    joinRoom(userName, room.value());
    return room.value();
}

bool Room::joinRoom(std::string userName, std::string roomName) {
    if(m_redis->zcard(roomName) >= 3)
    {
        return false;
    }
    // 检查房间是否存在
    if(!m_redis->exists(roomName))
    {
        m_redis->sadd(OnePlayer, roomName);
    }
        // 移动房间
    else if(m_redis->sismember(OnePlayer, roomName))
    {
        m_redis->smove(OnePlayer, TwoPlayer, roomName);
    }
    else if(m_redis->sismember(TwoPlayer, roomName))
    {
        m_redis->smove(TwoPlayer, ThreePlayer, roomName);
    }
    else
    {
        assert(false);  // 异常情况, 直接退出
    }
    // 将玩家添加到房间, 使用的结构是 sortedset
    m_redis->zadd(roomName, userName, 0);
    // 将玩家存储起来 hashs -> 通过玩家找到玩家的房间
    m_redis->hset("Players", userName, roomName);
    return true;
}

std::string Room::getNewRoomName() {
    // 创建随机设备对象
    std::random_device rd;
    // 创建随机数生成对象
    std::mt19937 gen(rd());
    // 创建随机数分布对象 -> 均匀分布
    std::uniform_int_distribution<int> dis(100000, 999999);
    int randNum = dis(gen);
    return std::to_string(randNum);
}

int Room::getPlayerCount(std::string roomName) {
    int count = m_redis->zcard(roomName);
    return  count;
}

void Room::updatePlayerScore(std::string roomName, std::string userName, int score) {
    m_redis->zadd(roomName, userName, score);
}

std::string Room::whereAmI(std::string userName) {
    auto value= m_redis->hget("Players",userName);
    if(value.has_value())
    {
        return value.value();
    }
    return  std::string();
}

int Room::PlayerScore(std::string roomName, std::string userName) {
    auto score = m_redis->zscore(roomName,userName);
    if(score.has_value())
    {
        return score.value();
    }
    return 0;
}

std::string Room::playersOrder(std::string roomName) {
    int index = 0;
    std::string data;
    std::vector<std::pair<std::string, double>> output;
    // 对房间中的玩家进行排序, 降序排序zrevrange
    m_redis->zrevrange(roomName, 0, -1, std::back_inserter(output));
    for(auto& it : output)
    {
        data += it.first + "-" + std::to_string(++index) + "-" + std::to_string((int)it.second) + "#";
    }
    return data;
}

void Room::leaveRoom(std::string roomName, std::string userName) {
    if(m_redis->sismember(ThreePlayer,roomName))
    {
        m_redis->smove(ThreePlayer,Invalid,roomName);
    }
    m_redis->zrem(roomName,userName);
    auto count = m_redis->zcard(roomName);
    if(count == 0)
    {
        m_redis->del(roomName);
        m_redis->srem(Invalid,roomName);
    }
}

bool Room::searchRoom(std::string roomName) {
    bool flag = m_redis->sismember(TwoPlayer,roomName);
    if(!flag)
    {
        flag = m_redis->sismember(OnePlayer,roomName);
    }
    return flag;
}
