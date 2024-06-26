#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QByteArray>
#include "cards.h"
#include "communication.h"
class DataManager
{
public:
    enum GameMode{Single, Network};
    enum RoomMode{Auto, Manual};
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    static DataManager* getInstance();

    void setUserName(QByteArray name);
    void setRoomName(QByteArray name);
    void setIP(QByteArray ip);
    void setPort(QByteArray port);
    void setCommunication(Communication* commun);
    void setCards(Cards cs, Cards last);
    void setGameMode(GameMode mode);
    void setRoomMode(RoomMode mode);


    QByteArray getUserName();
    QByteArray getRoomName();
    QByteArray getIP();
    QByteArray getPort();
    Communication* getCommunication();
    Cards getCards();
    Cards getLast3Cards();
    bool isNetworkMode();
    bool isManualMode();
private:
    DataManager()=default;
    static DataManager* m_data;

    QByteArray m_userName;
    QByteArray m_roomName;
    QByteArray m_ip = "192.168.150.128";
    QByteArray m_port = "8888";
    Communication* m_commun;

    Cards m_cs;
    Cards m_last;

    GameMode m_mode;
    RoomMode m_roomMode;
};
#endif // DATAMANAGER_H
