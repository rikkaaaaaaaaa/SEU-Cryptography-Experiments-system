// loginwindow.cpp
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QWidget>
#include <QPixmap>
#include <QPalette>
#include <qlineedit.h>

// 构造函数：初始化 UI
LoginWindow::LoginWindow(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    // 设置对话框标题
    this->setWindowTitle("登录");


    // 设置背景图
    QPixmap pixmap=QPixmap(":/C:/Users/white/Desktop/Background.png").scaled(this->size());
    QPalette palette;
    //设置主窗口背景图片
    palette.setBrush(QPalette::Window,QBrush(pixmap));

    //setStyleSheet("border-image:url(:/new/prefix1/C:/Users/white/Desktop/Background.jpg)");
    // void QWidget::paintEvent(QPaintEvent *ev) {
    //     QPainter painter(this);
    //     painter.drawPixmap(rect(), QPixmap(":/bg.jpg"), QRect());
    // }
    // QPixmap bg(":/../../../../23822-1.png");  // 资源文件路径
    // QPalette palette;
    // palette.setBrush(QPalette::Window, bg.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    // this->setPalette(palette);

    // 设置密码编辑框显示星号
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

// 析构函数：释放资源
LoginWindow::~LoginWindow()
{
    delete ui;
}

bool LoginWindow::getReady()
{
    return ready;
}

// 当用户点击登录按钮时触发
void LoginWindow::on_loginButton_clicked()
{
    // 这里简单判断用户名和密码是否正确
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text().trimmed();

    if (username == "seu" && password == "seucse") {
        // 登录成功，接受对话框
        accept();
        ready = 1;
        //accepted();
        //QMessageBox::information(this, "登录成功", "登录成功！");
    } else {
        // 登录失败，弹出警告消息框
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
    }
}
