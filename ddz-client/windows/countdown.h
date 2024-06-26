#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
class CountDown : public QWidget
{
    Q_OBJECT
public:
    explicit CountDown(QWidget *parent = nullptr);
    void showCountDown();
    void stopCountDown();

signals:
    void notMuchTime();
    void timeout();
protected:
    void paintEvent(QPaintEvent* ev);
private:
    QPixmap m_clock;
    QPixmap m_number;
    QTimer* m_timer;
    int m_count;

};

#endif // COUNTDOWN_H
