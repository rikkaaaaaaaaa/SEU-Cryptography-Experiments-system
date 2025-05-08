#include "mainwindow.h"
#include "DH.h"
#include "rsa.h"
#include "ui_mainwindow.h"
#include "encryption.h"
#include "EIGamal.h"
#include <QCryptographicHash>
#include <QMessageBox>

bool isAES;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 连接信号槽
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startProcess);
    //connect(ui->generateKeyButton, &QPushButton::clicked, this, &MainWindow::generateKey);
}

// 释放资源
MainWindow::~MainWindow()
{
    delete ui;
}


// 处理加密/解密
void MainWindow::startProcess()
{
    QString m_inputText = ui->inputText->toPlainText();
    QString m_key = ui->keyText->toPlainText();
    QString m_mode = ui->modeComboBox->currentText();
    QString m_algorithm = ui->algorithmComboBox->currentText();

    QString inputText = m_inputText;
    QString key = m_key;
    QString mode = m_mode;
    QString algorithm = m_algorithm;
    if (inputText.isEmpty() || key.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入明文/密文和密钥！");
        return;
    }

    updateDebugLog("模式: " + mode + " | 算法: " + algorithm);
    updateDebugLog("使用密钥: " + key);

    QString result;
    if (algorithm == "AES") {
        isAES = 1;
        if (mode == "Encrypt") {
            result = Encryption::aesEncrypt(inputText, key);
        } else if (mode == "Decrypt") {
            result = Encryption::aesDecrypt(inputText, key);
        } else if (mode == "Encrypt & Decrypt") {
            QString enc = Encryption::aesEncrypt(inputText, key);
            QString dec = Encryption::aesDecrypt(enc, key);
            updateDebugLog("加密后密文: " + enc);
            updateDebugLog("解密后明文: " + dec);
            return;
        }
    } else if (algorithm == "DES") {
        isAES = 0;
        if (mode == "Encrypt") {
            result = Encryption::desEncrypt(inputText, key);
        } else if (mode == "Decrypt") {
            result = Encryption::desDecrypt(inputText, key);
        } else if (mode == "Encrypt & Decrypt") {
            QString enc = Encryption::desEncrypt(inputText, key);
            QString dec = Encryption::desDecrypt(enc, key);
            updateDebugLog("加密后密文: " + enc);
            updateDebugLog("解密后明文: " + dec);
            return;
        }
    }

    updateDebugLog("结果: " + result);
}



// 更新调试日志
void MainWindow::updateDebugLog(const QString &message)
{
    ui->debugTextEdit->append(message);
}


// 生成随机密钥
void MainWindow::on_generateKeyButton_clicked()
{
    QString key = Encryption::generateRandomKey(isAES);
    ui->keyText->setText(key);
    updateDebugLog("生成密钥: " + key);
}


// void MainWindow::on_algorithmComboBox_currentTextChanged()
// {
//     m_algorithm = ui->algorithmComboBox->currentText();

// }


// void MainWindow::on_modeComboBox_currentTextChanged()
// {
//     m_mode = ui->modeComboBox->currentText();
// }


// void MainWindow::on_keyText_textChanged()
// {
//     m_key = ui->keyText->toPlainText();
// }


void MainWindow::on_actionEx_Euclid_alg_triggered()
{
    Euclid *euclidWindow = new Euclid(this);
    euclidWindow->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动释放内存
    euclidWindow->show();
}


void MainWindow::on_actionRSA_EnDecry_triggered()
{
    RSA *rsaWindow = new RSA(this);
    rsaWindow->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动释放内存
    rsaWindow->show();
}


void MainWindow::on_actionEIGamal_triggered()
{
    EIGamal *EIGamalWindow = new EIGamal(this);
    EIGamalWindow->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动释放内存
    EIGamalWindow->show();
}


void MainWindow::on_actionDiffle_Hellman_triggered()
{
    DH *dhWindow = new DH(this);
    dhWindow->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动释放内存
    dhWindow->show();
}

