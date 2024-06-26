#include "mybutton.h"
#include "QMouseEvent"
#include "QDebug"
MyButton::MyButton(QWidget *parent)
    : QPushButton{parent}
{
    this->setFixedSize(90,45);
    connect(this,&MyButton::clicked,this,[=](){
        qDebug()<<"btnClicked;";
    });
}

void MyButton::setImage(QString normal, QString hover, QString pressed)
{
    m_normal=normal;
    m_hover=hover;
    m_pressed = pressed;
    m_pixmap.load(m_normal);
    update();
}
void MyButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        m_pixmap.load(m_pressed);
        update();
    }

    QPushButton::mousePressEvent(event);

}

void MyButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        m_pixmap.load(m_normal);
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void MyButton::enterEvent(QEvent *event)
{
     Q_UNUSED(event)
    m_pixmap.load(m_hover);
    update();
}

void MyButton::leaveEvent(QEvent *event)
{
     Q_UNUSED(event)
    m_pixmap.load(m_normal);
    update();
}

void MyButton::paintEvent(QPaintEvent *event)
{
     Q_UNUSED(event)
    QPainter p(this);
    p.drawPixmap(rect(),m_pixmap);

}
