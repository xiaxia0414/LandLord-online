#ifndef ENDINGPANEL_H
#define ENDINGPANEL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "scorepanel.h"
class EndingPanel : public QWidget
{
    Q_OBJECT
public:
    explicit EndingPanel(bool isLord, bool isWin, QWidget *parent = nullptr);

    void setPlayerScore(int left, int right, int me);
    void setPlayerName(QByteArrayList names);

private:
    QPixmap m_bk;
    QLabel* m_title;
    ScorePanel *m_score;
    QPushButton* m_continue;
protected:
    void paintEvent(QPaintEvent *event);
signals:
    void continueGame();

    // QWidget interface

};

#endif // ENDINGPANEL_H
