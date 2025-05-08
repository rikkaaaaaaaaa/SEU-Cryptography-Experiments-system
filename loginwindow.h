// loginwindow.h
#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QDialog *parent = nullptr);
    ~LoginWindow();
    bool getReady();

private slots:
    // 当用户点击登录按钮时执行槽函数
    void on_loginButton_clicked();

private:
    Ui::LoginWindow *ui;
    bool ready = 0;
};

#endif // LOGINWINDOW_H
