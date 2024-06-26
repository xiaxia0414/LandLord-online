#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QRunnable>
#include "tcpserver.h"
#include "codec.h"
#include "aescrypto.h"
class Communication : public QObject ,public QRunnable
{
    Q_OBJECT
public:
    enum KeyLen{L16 =16,L24=24,L32 = 32};
    explicit Communication(Message& msg, QObject *parent = nullptr);
    ~Communication();
    inline void stopLoop()
    {
        m_stop = true;
    }
    //给服务器发数据
    void sendMessage(Message* msg,bool encrypt = true);
    //解析数据
    void praseRecvMessage();
    void handleRsaFenfa(Message* msg);
    QByteArray generateAesKey(KeyLen len);
     void parseCards(QByteArray data1, QByteArray data2);

protected:
    void run() override;
private:
    bool m_stop = false;
    Message m_msg ;
    TcpSocket* m_socket = nullptr;
    QByteArray m_aesKey;
    AesCrypto* m_aes = nullptr;

signals:
    void connectFailed();
    void connected();
    void loginOk();
    void registerOk();
    void failedMsg(QByteArray failedMsg);
    void playerCount(int count);
    void startGame(QByteArray msg);
    void roomExist(bool flag);
    void somebodyLeave(int count);
};

#endif // COMMUNICATION_H
