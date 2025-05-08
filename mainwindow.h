#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include "Euclid.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // QString m_inputText = 0;
    // QString m_mode = 0;
    // QString m_algorithm = 0;
    // QString m_key = 0;

private slots:
    void startProcess();  // 处理加密/解密
    //void generateKey();   // 生成密钥
    void updateDebugLog(const QString &message); // 输出调试信息

    void on_generateKeyButton_clicked();

    // void on_algorithmComboBox_currentTextChanged();

    // void on_modeComboBox_currentTextChanged();

    // void on_keyText_textChanged();

    void on_actionEx_Euclid_alg_triggered();

    void on_actionRSA_EnDecry_triggered();

    void on_actionEIGamal_triggered();

    void on_actionDiffle_Hellman_triggered();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
