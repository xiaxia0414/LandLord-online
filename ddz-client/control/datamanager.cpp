#include "datamanager.h"
DataManager* DataManager::m_data = new DataManager;

DataManager *DataManager::getInstance()
{
    return m_data;
}

void DataManager::setUserName(QByteArray name)
{
    m_userName = name;
}

void DataManager::setRoomName(QByteArray name)
{
    m_roomName = name;
}

void DataManager::setIP(QByteArray ip)
{
    m_ip = ip;
}

void DataManager::setPort(QByteArray port)
{
    m_port = port;
}

void DataManager::setCommunication(Communication *commun)
{
    m_commun = commun;
}

void DataManager::setCards(Cards cs, Cards last)
{
    m_cs = cs;
    m_last = last;
}

void DataManager::setGameMode(GameMode mode)
{
    m_mode = mode;
}

void DataManager::setRoomMode(RoomMode mode)
{
    m_roomMode = mode;
}

QByteArray DataManager::getUserName()
{
    return m_userName;
}

QByteArray DataManager::getRoomName()
{
    return m_roomName;
}

QByteArray DataManager::getIP()
{
    return m_ip;
}

QByteArray DataManager::getPort()
{
    return m_port;
}

Communication *DataManager::getCommunication()
{
    return m_commun;
}

Cards DataManager::getCards()
{
    return m_cs;
}

Cards DataManager::getLast3Cards()
{
    return m_last;
}

bool DataManager::isNetworkMode()
{
    return m_mode == GameMode::Network ? true : false;
}

bool DataManager::isManualMode()
{
    return m_roomMode == RoomMode::Manual ? true : false;
}
