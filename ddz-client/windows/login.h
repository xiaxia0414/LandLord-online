#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLineEdit>
#include "codec.h"
#include <QFrame>>
namespace Ui {
class Login;
}

class Login : public QFrame
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    bool verifyData(QLineEdit* edit);

    void startConnect(Message* msg);

    void onLogin();
    void onRegister();
    void onNetOK();

    void saveUserInfo();
    void loadUserInfo();

private:
    Ui::Login *ui;
    bool m_isConnected;
     const QByteArray KEY = "天长地久有时尽,此恨绵绵无绝期!";

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void on_remberPW_clicked(bool checked);
};

#endif // LOGIN_H
