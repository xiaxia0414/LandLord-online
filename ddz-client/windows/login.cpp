 #include "login.h"
#include "gamemode.h"
#include "qcryptographichash.h"
#include "ui_login.h"
#include "QRegularExpressionValidator"
#include <datamanager.h>
#include "communication.h"
#include "QThreadPool"
#include "gamepanel.h"
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QPainter>
Login::Login(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::Login)
{
    this->setFixedSize(640,360);
    this->setWindowTitle("登录");
    //this->ui->widget->setWindowTitle("登录");
    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->setObjectName("Login");
    //this->setStyleSheet("#Login{border-image:url(:/images/logBackground3.png);}");
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->homePageBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->ipConfiBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->goRegBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(1);
    });

    QRegularExpression expreg("^[a-zA-Z0-9_]{3,16}$");
    QRegularExpressionValidator *val1 = new QRegularExpressionValidator(expreg, this);
    ui->userName->setValidator(val1);
    ui->regUserName->setValidator(val1);
    /*
     * 密码:
     *  1. 长度为4到12个字符
     *  2. 包含至少一个大写字母、小写字母、数字和特殊字符
    */
    expreg.setPattern("^[a-zA-Z0-9]{3,16}$");
    QRegularExpressionValidator *val2 = new QRegularExpressionValidator(expreg, this);
    ui->passWord ->setValidator(val2);
    ui->regPassword->setValidator(val2);

    // 手机号校验
    expreg.setPattern("^1[3456789]\\d{9}$");
    QRegularExpressionValidator *val3 = new QRegularExpressionValidator(expreg, this);
    ui->regPhonenumber->setValidator(val3);

    // IP校验
    expreg.setPattern("^((\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.){3}(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])$");
    QRegularExpressionValidator *val4 = new QRegularExpressionValidator(expreg, this);
    ui->ipAddress->setValidator(val4);

    // 端口校验
    expreg.setPattern("^(0|([1-9]\\d{0,3})|([1-5]\\d{4})|(6[0-4]\\d{3})|(65[0-4]\\d{2})|(655[0-2]\\d)|(6553[0-5]))$");
    QRegularExpressionValidator *val5 = new QRegularExpressionValidator(expreg, this);
    ui->port->setValidator(val5);

    // 处理按钮点击事件
    connect(ui->lgBtn, &QPushButton::clicked, this, &Login::onLogin);
    connect(ui->regBtn, &QPushButton::clicked, this, &Login::onRegister);
    connect(ui->setBtn, &QPushButton::clicked, this, &Login::onNetOK);
    m_isConnected =false;

    loadUserInfo();

    ui->ipAddress->setText(DataManager::getInstance()->getIP());
    ui->port->setText(DataManager::getInstance()->getPort());
    connect(ui->setBtn,&QPushButton::clicked,this,[=](){
        DataManager::getInstance()->setPort(ui->port->text().toUtf8());
        DataManager::getInstance()->setIP(ui->ipAddress->text().toUtf8());
    });

    connect(ui->startSingelMode,&QPushButton::clicked,this,[=](){
        GamePanel* game = new GamePanel();
        game->show();
        this->hide();
        connect(game,&GamePanel::destroyed,this,[=](){
            this->show();
        });
    });

}
void Login::onLogin()
{
    bool flag1 = verifyData(ui->userName);
    bool flag2 = verifyData(ui->passWord);
    if(flag1 && flag2)
    {
        Message msg;
        msg.userName = ui->userName->text().toUtf8();
        msg.reqcode = RequestCode::UserLogin;
        qDebug()<<"reqCode:"<<msg.reqcode;
        QByteArray passwd = ui->passWord->text().toUtf8();
        passwd = QCryptographicHash::hash(passwd, QCryptographicHash::Sha224).toHex();
        msg.data1 = passwd;
        // 连接服务器
        startConnect(&msg);

    }
}

bool Login::verifyData(QLineEdit* edit)
{
    if(edit->hasAcceptableInput() == false)
    {
        //edit->setStyleSheet("border: 2px solid red;");
        return false;
    }
    else
    {
        //edit->setStyleSheet("none");
    }
    return true;
}

void Login::startConnect(Message *msg)
{
    if(!m_isConnected)
    {
        Communication *task = new Communication(*msg);
        connect(task,&Communication::connectFailed,this,[=](){
            QMessageBox::critical(this,"连接服务器","连接服务器失败");
            m_isConnected = false;
        });
        connect(task,&Communication::connected,this,[=](){
            qDebug()<<"服务器连接成功";
            m_isConnected = true;
        });
        connect(task,&Communication::loginOk,this,[=](){
            DataManager::getInstance()->setUserName(ui->userName->text().toUtf8());
            saveUserInfo();
            GameMode* mode = new GameMode;
            mode->show();
            this->hide();

        });
        connect(task,&Communication::registerOk,this,[=](){
            ui->stackedWidget->setCurrentIndex(0);
        });
        connect(task,&Communication::failedMsg,this,[=](QByteArray failedMsg){
            QMessageBox::critical(this,"ERROR",failedMsg);
        });
        QThreadPool::globalInstance()->start(task);
        DataManager::getInstance()->setCommunication(task);
    }
    else
    {
        DataManager::getInstance()->getCommunication()->sendMessage(msg);
    }
}


void Login::onRegister()
{
    bool flag1 = verifyData(ui->regUserName);
    bool flag2 = verifyData(ui->regPassword);
    bool flag3 = verifyData(ui->regPhonenumber);
    if(flag1 && flag2 && flag3)
    {
         Message msg;
         msg.userName = ui->regUserName->text().toUtf8();
         msg.reqcode = RequestCode::Register;
         QByteArray passwd = ui->regPassword->text().toUtf8();
         passwd = QCryptographicHash::hash(passwd, QCryptographicHash::Sha224).toHex();
         msg.data1 = passwd;
        msg.data2 = ui->regPhonenumber->text().toUtf8();
        // 连接服务器
         startConnect(&msg);

    }
}

void Login::onNetOK()
{
    bool flag1 = verifyData(ui->ipAddress);
    bool flag2 = verifyData(ui->port);
    if(flag1 && flag2)
    {
        DataManager* instance = DataManager::getInstance();
        instance->setIP(ui->ipAddress->text().toUtf8());
        instance->setPort(ui->port->text().toUtf8());
    }
}

void Login::saveUserInfo()
{
    if(ui->remberPW->isChecked())
    {
        QJsonObject obj;
        obj.insert("user", ui->userName->text());
        obj.insert("passwd", ui->passWord->text());
        QJsonDocument doc(obj);
        QByteArray json = doc.toJson();
        // aes 加密
        AesCrypto aes(AesCrypto::AES_CBC_128, KEY.left(16));
        json = aes.encrypt(json);
        // 写文件
        QFile file("passwd.bin");
        file.open(QFile::WriteOnly);
        file.write(json);
        file.close();
    }
    else
    {
        QFile file("passwd.bin");
        file.remove();
    }

}

void Login::loadUserInfo()
{
    QFile file("passwd.bin");
    bool flag  = file.open(QFile::ReadOnly);
    if(flag)
    {
        ui->remberPW->setChecked(true);
        QByteArray all = file.readAll();
        AesCrypto aes(AesCrypto::AES_CBC_128, KEY.left(16));
        all = aes.decrypt(all);
        QJsonDocument doc = QJsonDocument::fromJson(all);
        QJsonObject obj = doc.object();
        QString name = obj.value("user").toString();
        QString passwd = obj.value("passwd").toString();
        ui->userName->setText(name);
        ui->passWord->setText(passwd);
    }
    else
    {
        ui->remberPW->setChecked(false);
    }
}

void Login::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/logBackground3.jpg");  // 指定背景图片路径
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}

Login::~Login()
{
    delete ui;
}

void Login::on_remberPW_clicked(bool checked)
{

}

