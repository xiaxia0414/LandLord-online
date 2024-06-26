#include "buttongroup.h"
#include "ui_buttongroup.h"
#include "QDebug"
ButtonGroup::ButtonGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::initButton()
{
    ui->start->setImage(":/images/start-1.png",":/images/start-3.png",":/images/start-2.png");

    ui->playCard->setImage(":/images/chupai_btn-1.png",":/images/chupai_btn-3.png",":/images/chupai_btn-2.png");

    ui->pass->setImage(":/images/pass_btn-1.png",":/images/pass_btn-3.png",":/images/pass_btn-2.png");
    ui->play->setImage(":/images/chupai_btn-1.png",":/images/chupai_btn-3.png",":/images/chupai_btn-2.png");

    ui->giveUp->setImage(":/images/buqiang-1.png",":/images/buqiang-3.png",":/images/buqiang-2.png");
    ui->oneScore->setImage(":/images/1fen-1.png",":/images/1fen-3.png",":/images/1fen-2.png");
    ui->twoScore->setImage(":/images/2fen-1.png",":/images/2fen-3.png",":/images/2fen-2.png");
    ui->threeScore->setImage(":/images/3fen-1.png",":/images/3fen-3.png",":/images/3fen-2.png");

    // connect(ui->start,&MyButton::clicked,this,&ButtonGroup::startGame);
    // connect(ui->playCard,&MyButton::clicked,this,&ButtonGroup::playHand);
    // connect(ui->play,&MyButton::clicked,this,&ButtonGroup::playHand);
    // connect(ui->pass,&MyButton::clicked,this,&ButtonGroup::pass);
    // // connect(ui->giveUp,&MyButton::clicked,this,[=](){
    //     emit betPoint(0);
    //      qDebug()<<"btnEmit0";
    // });
    // connect(ui->oneScore,&MyButton::clicked,this,[=](){
    //     // emit betPoint(1);
    //     // qDebug()<<"btnEmit1";
    // });
    // connect(ui->twoScore,&MyButton::clicked,this,[=](){
    //     if(isSended==false)
    //         {emit betPoint(2);
    //         qDebug()<<"btnEmit2";}
    //     isSended = true;
    // });
    // connect(ui->threeScore,&MyButton::clicked,this,[=](){
    //     emit betPoint(3);
    //      qDebug()<<"btnEmit3";
    // });
    // connect(ui->start,&MyButton::clicked,this,[=](){

    //     qDebug()<<"stgm";
    // });
}

void ButtonGroup::selectPanel(Panel type,int bet)
{
    ui->stackedWidget->setCurrentIndex(type);
    if(type !=CallLord)
    {
        return;
    }
    if(bet == 0)
    {
        ui->oneScore->setVisible(true);
        ui->twoScore->setVisible(true);
        ui->threeScore->setVisible(true);
    }
    else if(bet == 1)
    {
        ui->oneScore->setVisible(false);
        ui->twoScore->setVisible(true);
        ui->threeScore->setVisible(true);
    }
    else if(bet == 2)
    {
        ui->oneScore->setVisible(false);
        ui->twoScore->setVisible(false);
        ui->threeScore->setVisible(true);
    }
}

void ButtonGroup::on_oneScore_clicked()
{
    emit betPoint(1);
}


void ButtonGroup::on_giveUp_clicked()
{
    emit betPoint(0);
}


void ButtonGroup::on_twoScore_clicked()
{
    emit betPoint(2);
}


void ButtonGroup::on_threeScore_clicked()
{
    emit betPoint(3);
}


void ButtonGroup::on_pass_clicked()
{
    emit pass();
}


void ButtonGroup::on_play_clicked()
{
    emit playHand();
}


void ButtonGroup::on_playCard_clicked()
{
    emit playHand();
}


void ButtonGroup::on_start_clicked()
{
    emit startGame();
}

