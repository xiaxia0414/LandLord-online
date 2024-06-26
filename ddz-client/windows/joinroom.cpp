#include "joinroom.h"
#include "codec.h"
#include "datamanager.h"
#include "ui_joinroom.h"

JoinRoom::JoinRoom(DialogType type, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::JoinRoom)
{
    ui->setupUi(this);
    const QString titles[] ={
      "请输入要创建房间的名字",
      "请输入要搜索房间的名字"
    };
    int index = static_cast<int>(type);
    ui->label->setText(titles[index]);
    ui->stackedWidget->setCurrentIndex(index);
    setFixedSize(300,180);

    Communication* comm = DataManager::getInstance()->getCommunication();
    connect(comm,&Communication::roomExist,this,[=](bool flag){
        if(flag)
        {
            ui->joinRoom->setEnabled(true);
        }
        else
            {
            ui->joinRoom->setEnabled(false);
        }
    });

    connect(ui->creatRoom,&QPushButton::clicked,this,[=](){
        encodeMessage(RequestCode::ManualRoom);
        DataManager::getInstance()->setRoomMode(DataManager::Manual);
    });
    connect(ui->searcRoom,&QPushButton::clicked,this,[=](){
        encodeMessage(RequestCode::SearchRoom);

    });
    connect(ui->joinRoom,&QPushButton::clicked,this,[=](){
        encodeMessage(RequestCode::ManualRoom);
        DataManager::getInstance()->setRoomMode(DataManager::Manual);
    });


}

JoinRoom::~JoinRoom()
{
    delete ui;

}

void JoinRoom::encodeMessage(RequestCode code)
{
    Message msg;
    msg.reqcode = code;
    msg.userName = DataManager::getInstance()->getUserName();
    msg.roomName = ui->roomName->text().toUtf8();
    DataManager::getInstance()->getCommunication()->sendMessage(&msg);
}
