//
// Created by Hasee on 2024/5/17.
//

#ifndef CLION_SOURCE_UPLOAD_MARKER_ROOM_H
#define CLION_SOURCE_UPLOAD_MARKER_ROOM_H
#include <sw/redis++/redis++.h>
class Room {
public:
    Room() = default;
    ~Room();
    // 初始化 - 连接redis, redis++
    bool initEnvironment();
    // 清空数据
    void clear();
    // 保存RSA密钥对
    // key(filed, value)
    // hset
    void saveRsaSecKey(std::string field, std::string value);
    std::string rsaSecKey(std::string field);

    std::string joinRoom(std::string userName);
    bool joinRoom(std::string userName,std::string roomName);

    std::string getNewRoomName();

    int getPlayerCount(std::string roomName);
    void updatePlayerScore(std::string roomName, std::string userName, int score);
    std::string whereAmI(std::string userName);
    int PlayerScore(std::string roomName, std::string userName);
    std::string playersOrder(std::string roomName);
    void leaveRoom(std::string roomName, std::string userName);
    bool searchRoom(std::string roomName);

private:
    sw::redis::Redis* m_redis;
    const std::string OnePlayer = "OnePlayer";
    const std::string TwoPlayer = "TwoPlayers";
    const std::string ThreePlayer = "ThreePlayers";
    const std::string Invalid = "Invalid";
};


#endif //CLION_SOURCE_UPLOAD_MARKER_ROOM_H
