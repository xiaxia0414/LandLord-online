#include "endingpanel.h"
#include "QPainter"
EndingPanel::EndingPanel(bool isLord, bool isWin, QWidget *parent)
    : QWidget{parent}
{
    m_bk.load(":/images/gameover.png");
    setFixedSize(m_bk.size());
    m_title = new QLabel(this);
    if(isLord && isWin)
    {
        m_title->setPixmap(QPixmap(":/images/lord_win.png"));
    }
    else if(isLord && !isWin)
    {
        m_title->setPixmap(QPixmap(":/images/lord_fail.png"));
    }
    else if(!isLord && isWin)
    {
        m_title->setPixmap(QPixmap(":/images/farmer_win.png"));
    }
    else if(!isLord && !isWin)
    {
        m_title->setPixmap(QPixmap(":/images/farmer_fail.png"));
    }
    m_title->move(125, 130);

    m_score = new ScorePanel(this);
    m_score->move(75, 230);
    m_score->setFixedSize(260, 160);
    m_score->setMyFontColor(ScorePanel::Red);
    m_score->setMyFontSize(18);

    m_continue = new QPushButton(this);
    m_continue->move(84, 429);
    QString style = R"(
        QPushButton{border-image: url(:/images/button_normal.png)}
        QPushButton:hover{border-image: url(:/images/button_hover.png)}
        QPushButton:pressed{border-image: url(:/images/button_pressed.png)}
    )";
    m_continue->setStyleSheet(style);
    m_continue->setFixedSize(231, 48);
    connect(m_continue, &QPushButton::clicked, this, &EndingPanel::continueGame);

}

void EndingPanel::setPlayerScore(int left, int right, int me)
{
    m_score->setScore(left, right, me);
}

void EndingPanel::setPlayerName(QByteArrayList names)
{
    m_score->setPlayerName(names.at(0),names.at(1),names.at(2));
}

void EndingPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawPixmap(rect(), m_bk);
}
