//
// Created by Hasee on 2024/5/19.
//

#ifndef CLION_SOURCE_UPLOAD_MARKER_ROOMLIST_H
#define CLION_SOURCE_UPLOAD_MARKER_ROOMLIST_H
#include <map>
#include <string>
#include <functional>
#include <mutex>
using callback = std::function<void(std::string)>;
using UserMap = std::map<std::string, callback>;

class RoomList {
public:
    RoomList(const RoomList&) = delete;
    RoomList& operator=(const RoomList&) = delete;
    static RoomList* getInstance();

    // 添加用户
    void addUser(std::string roomName, std::string userName, callback sendMessage);
    // 通过房间名字取出房间内的成员信息
    UserMap getPlayers(std::string roomName);
    // 得到当前房间中除指定玩家外的其他玩家
    UserMap getPartners(std::string roomName, std::string userName);
    // 删除指定房间中的玩家
    void removePlayer(std::string roomName, std::string userName);
    // 清空房间内所有玩家
    void removeRoom(std::string roomName);


private:
    RoomList()=default;
    std::map<std::string, UserMap> m_roomMap;   // 共享资源, 需要多线程访问
    std::mutex m_mutex;
};


#endif //CLION_SOURCE_UPLOAD_MARKER_ROOMLIST_H
