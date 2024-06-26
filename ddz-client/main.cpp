#include "gamepanel.h"
#include "login.h"
#include <QApplication>
#include "aescrypto.h"
#include <QDebug>
#include "rsacrypto.h"
#include "gamemode.h"
#include "QBoxLayout"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    a.setFont(QFont("微软雅黑"));
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");
    //直接显示单机模式窗口
    //GamePanel w;
    //w.show();


     Login lg;
     lg.show();

    return a.exec();
}
