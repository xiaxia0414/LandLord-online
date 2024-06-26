#include "communication.h"
#include "cards.h"
#include "datamanager.h"
#include "rsacrypto.h"
#include "taskqueue.h"
#include <QDateTime>
#include <QDebug>
#include <QThread>

Communication::Communication(Message& msg,QObject *parent)
    : QObject{parent}, QRunnable(),m_msg(msg)
{
    this->setAutoDelete(true);
}

Communication::~Communication()
{
    if(m_aes)
    {
        delete m_aes;
    }
}

void Communication::sendMessage(Message *msg,bool encrypt)
{
    Codec codec(msg);
    QByteArray data = codec.encodeMsg();
    if(encrypt)
    {
         data = m_aes->encrypt(data);
    }
    m_socket->sendMsg(data);
}

void Communication::praseRecvMessage()
{
   QByteArray data = m_socket->recvMsg(2);
    if(data.isEmpty())return;
    Codec codec(data);
   QSharedPointer<Message> ptr = codec.decodeMsg();
    qDebug()<<ptr->rescode<<","<<QString(ptr->data1)<<","<<ptr->data2;
   qDebug()<<"reqcode"<<(int)RequestCode::AutoRoom;
   switch (ptr->rescode) {
    case RespondCode::LoginOk:
       emit loginOk();
         qDebug()<<"登录成功";
       break;
    case RespondCode::RegisterOk:
        emit registerOk();
         qDebug()<<"注册成功";
       break;
    case RespondCode::RsaFenFa:
        handleRsaFenfa(ptr.get());
        break;
    case RespondCode::AesVerifyOK:
        m_aes = new AesCrypto(AesCrypto::AES_CBC_256,m_aesKey);
        sendMessage(&m_msg);
        qDebug()<<"aes密钥分发成功";
        break;
    case RespondCode::Failed:
        emit failedMsg(ptr->data1);
    case RespondCode::JoinRoomOK:
        qDebug()<<"JoinRoomOK";
        DataManager::getInstance()->setRoomName(ptr->roomName);
        emit playerCount(ptr->data1.toInt());
        break;
    case RespondCode::DealCards:
        parseCards(ptr->data1,ptr->data2);
        break;
    case RespondCode::StartGame:
        emit startGame(ptr->data1);
        break;
    case RespondCode::OtherGrabLord:
    {
        Task t;
        t.bet = ptr->data1.toInt();
        TaskQueue::getInstance()->add(t);
        break;
    }
    case RespondCode::OtherPlayHand:
    {
        Task t;
        QDataStream stream(ptr->data2);
        for(int i=0;i<ptr->data1.toInt();++i)
        {
            Card c;
            stream>>c;
            t.cs.add(c);
        }
        TaskQueue::getInstance()->add(t);
        break;
    }
    case RespondCode::SearchRoomOK:
    {
        bool flag = ptr->data1 == "true"? true:false;
        emit roomExist(flag);
        break;
    }
    case RespondCode::OtherLeaveRoom:
    {
        int count = ptr->data1.toInt();
        emit somebodyLeave(count);
        break;
    }
   default:
       break;
   }

}

void Communication::handleRsaFenfa(Message* msg)
{
    RsaCrypto rsa;
    rsa.parseStringToKey(msg->data1,RsaCrypto::PublicKey);
    //校验签名
    //bool flag = rsa.verify(msg->data2,msg->data1);
   // assert(flag);
    //生成aes密钥
    m_aesKey = generateAesKey(KeyLen::L32);
    Message resMsg;
     qDebug("before message sended");
    resMsg.reqcode = RequestCode::AesFenfa;
    resMsg.data1 = rsa.pubKeyEncrypt(m_aesKey);
    resMsg.data2 = QCryptographicHash::hash(m_aesKey,QCryptographicHash::Sha224).toHex();
    sendMessage(&resMsg,false);
    qDebug("message sended");
}

QByteArray Communication::generateAesKey(KeyLen len)
{
    QByteArray time = QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss.zzz").toUtf8();
    QCryptographicHash hash(QCryptographicHash::Sha3_384);
    hash.addData(time);
    time = hash.result();
    time = time.left(len);
    return time;
}

void Communication::parseCards(QByteArray data1, QByteArray data2)
{
    // xx-xx#xxx-xx#xx-xx
    auto func = std::bind([=](QByteArray msg){
        auto lst = msg.left(msg.length()-1).split('#');
        Cards cs;
        for(const auto& item : lst)
        {
            auto sub = item.split('-');
            Card card(static_cast<Card::CardPoint>(sub.last().toInt()),
                      static_cast<Card::CardSuit>(sub.first().toInt()));
            cs.add(card);
        }
        return cs;
    }, std::placeholders::_1);
    Cards cards = func(data1);
    Cards last = func(data2);
    // 存储数据
    DataManager::getInstance()->setCards(cards, last);
}

void Communication::run()
{
    m_socket = new TcpSocket;
    // 连接服务器
    DataManager* obj = DataManager::getInstance();
    bool flag = m_socket->connectToHost(obj->getIP(), obj->getPort().toInt());
    qDebug() << "========== 通信的子线程id: " << QThread::currentThread() << "connected ? " << flag;
    if(!flag)
    {
        // 通知主线程连接诶失败了
        emit connectFailed();
        delete m_socket;
        return;
    }
    // 连接成功了
    emit connected();
    while(!m_stop)
    {
        // 处理服务器回复的数据
        // 首先得到非对称加密的公钥, 发送加密之后的对称加密秘钥给服务器, 服务器回复接收秘钥成功的消息, 然后发送登录或者注册请求
        praseRecvMessage();
    }
    m_socket->disConnect();
    delete m_socket;
}
