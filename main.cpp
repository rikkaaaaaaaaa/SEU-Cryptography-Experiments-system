#include "loginwindow.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // w.show();
    LoginWindow loginWin;
    loginWin.show();  // 先显示登录窗口
    if(loginWin.exec() == QDialog::Accepted)
    {
        //MainWindow w;
        w.show();
    }

    return a.exec();
}
