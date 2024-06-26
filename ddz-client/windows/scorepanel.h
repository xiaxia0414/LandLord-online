#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>
#include <QLabel>
namespace Ui {
class ScorePanel;
}

class ScorePanel : public QWidget
{
    Q_OBJECT

public:
    enum FontColor{Black, White, Red, Blue, Green};
    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();
    void setScore(int a,int b,int u);
    void setMyFontSize(int point);
    void setMyFontColor(FontColor color);
    void setPlayerName(QByteArray left,QByteArray mid,QByteArray right);

private:
    Ui::ScorePanel *ui;
     QVector<QLabel*> m_list;
};

#endif // SCOREPANEL_H
