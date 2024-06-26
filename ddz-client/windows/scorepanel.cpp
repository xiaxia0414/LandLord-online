#include "scorepanel.h"
#include "ui_scorepanel.h"
const QString MYCOLOR[] = {"black", "white", "red", "blue", "green"};
ScorePanel::ScorePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScorePanel)
{
    ui->setupUi(this);
    m_list<<ui->RobotAScoreLab<<ui->RobotBScoreLab<<ui->lab_robotA
           <<ui->lab_robotB<<ui->lab_score<<ui->lab_score1<<ui->lab_score2
           <<ui->userScoreLab<<ui->lab_palyer;
    this->setObjectName("ScorePanel");
    this->setStyleSheet("background-color:white;"
                        "");
}

ScorePanel::~ScorePanel()
{
    delete ui;
}

void ScorePanel::setScore(int a, int b, int u)
{
    ui->RobotAScoreLab->setText(QString::number(a));
    ui->RobotBScoreLab->setText(QString::number(b));
    ui->userScoreLab->setText(QString::number(u));
}

void ScorePanel::setMyFontSize(int point)
{
    QFont font("微软雅黑",point,QFont::Bold);
    for(int i =0;i<m_list.size();i++)
    {
        m_list[i]->setFont(font);
    }
}

void ScorePanel::setMyFontColor(FontColor color)
{
    QString style = QString("QLabel{color:%1}").arg(MYCOLOR[color]);
    for(int i=0; i<m_list.size(); ++i)
    {
        m_list[i]->setStyleSheet(style);
    }
}

void ScorePanel::setPlayerName(QByteArray left, QByteArray mid, QByteArray right)
{
    ui->lab_palyer->setText(mid);
    ui->lab_robotA->setText(left);
    ui->lab_robotB->setText(right);
}
