#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
class AnimationWindow : public QWidget
{
    Q_OBJECT
public:
    enum Type{Sequence, Pair};
    explicit AnimationWindow(QWidget *parent = nullptr);
    void showBetScore(int bet);
    void showSequence(Type type);
    // 显示王炸
    void showJokerBomb();
    // 显示炸弹
    void showBomb();
    // 显示飞机
    void showPlane();

    void showJokerBomb1();
    // 显示炸弹
    void showBomb1();
    // 显示飞机
    void showPlane1();
private:
    QPixmap m_image;
    int m_index = 0;
    int m_x = 0;
signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void hide1();
};

#endif // ANIMATIONWINDOW_H
