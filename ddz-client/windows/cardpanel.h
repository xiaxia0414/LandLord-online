#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include "card.h"
#include "player.h"
#include "QPainter"

class CardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);

    void setImage(QPixmap &front,QPixmap &back);
    QPixmap image();

    void setFrontSide(bool flag);
    bool isFrontSide();

    void setSelected(bool flag);
    bool isSelected();

    void setCart(Card &card);
    Card card();

    void setOwner(Player *owner);
    Player* owner();

    void clicked();
signals:
    void cardSelected(Qt::MouseButton botton);




protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QPixmap m_front;
    QPixmap m_back;
    bool m_isfront;
    bool m_isSelected;
    Card m_card;
    Player *m_owner;
};


#endif // CARDPANEL_H
