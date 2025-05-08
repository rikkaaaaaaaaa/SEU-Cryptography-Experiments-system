// DH.cpp
#include "DH.h"
#include "ui_DH.h"
#include <QTime>

DH::DH(QWidget *parent) : QDialog(parent), ui(new Ui::DH) {
    ui->setupUi(this);
}

DH::~DH() {
    delete ui;
}

bool DH::isPrime(const BigInt &n) {
    if (n == BigInt(2)) return true;
    if (n < BigInt(2) || (n % BigInt(2) == BigInt(0))) return false;
    for (BigInt i = BigInt(3); i * i <= n; i = i + BigInt(2)) {
        if (n % i == BigInt(0)) return false;
    }
    return true;
}

BigInt DH::generateLargePrime(int digits) {
    QTime time = QTime::currentTime();
    srand((uint)time.msec());
    while (true) {
        QString candidateStr = "";
        candidateStr += QString::number(rand() % 9 + 1);
        for (int i = 1; i < digits; ++i) {
            candidateStr += QString::number(rand() % 10);
        }
        BigInt candidate(candidateStr);
        if (isPrime(candidate)) return candidate;
    }
}

void DH::displayDebug(const QString &text) {
    ui->outputTextEdit->append(text);
}

// 核心：统一的密钥协商逻辑
void DH::performKeyAgreement() {
    g = BigInt(2); // 固定原根

    // 计算公开值
    YA = g.modPow(XA, p);
    YB = g.modPow(XB, p);

    // 计算双方的会话密钥
    KA = YB.modPow(XA, p);
    KB = YA.modPow(XB, p);

    // 输出结果
    ui->outputTextEdit->clear();
    displayDebug("[Diffie-Hellman 密钥协商过程]");
    displayDebug("使用的大素数 p: " + p.toString());
    displayDebug("原根 g: " + g.toString());
    displayDebug("A的私钥 XA: " + XA.toString());
    displayDebug("B的私钥 XB: " + XB.toString());
    displayDebug("A的公开值 YA = g^XA mod p: " + YA.toString());
    displayDebug("B的公开值 YB = g^XB mod p: " + YB.toString());
    displayDebug("A计算的会话密钥 KA = YB^XA mod p: " + KA.toString());
    displayDebug("B计算的会话密钥 KB = YA^XB mod p: " + KB.toString());

    if (KA == KB) {
        displayDebug("✅ 双方会话密钥一致，协商成功！");
        int bitLength = KA.bitLength();
        displayDebug("共享密钥 K 的比特位数: " + QString::number(bitLength) + " bits");
    } else {
        displayDebug("❌ 会话密钥不一致，协商失败！");
    }
}

// 自动生成：GenerateAll按钮
void DH::on_generateAllButton_clicked() {
    // 自动生成 p、XA、XB
    int p_digit = 10;
    if(ui->lineEdit->text().isEmpty())
    {

    }
    else
    {
        p_digit = ui->lineEdit->text().toInt();
    }

    p = generateLargePrime(p_digit);

    QTime time = QTime::currentTime();
    srand((uint)time.msec());
    QString xaStr, xbStr;
    for (int i = 0; i < 5; ++i) xaStr += QString::number(rand() % 10);
    for (int i = 0; i < 5; ++i) xbStr += QString::number(rand() % 10);
    XA = BigInt(xaStr);
    XB = BigInt(xbStr);

    // 将生成的值同步到LineEdit
    ui->inputPLineEdit->setText(p.toString());
    ui->inputXALineEdit->setText(XA.toString());
    ui->inputXBLineEdit->setText(XB.toString());

    // 开始密钥协商
    performKeyAgreement();
}

// 手动输入：Start按钮
void DH::on_startButton_clicked() {
    // 从LineEdit读取数据
    QString pStr = ui->inputPLineEdit->text();
    QString xaStr = ui->inputXALineEdit->text();
    QString xbStr = ui->inputXBLineEdit->text();

    if (pStr.isEmpty() || xaStr.isEmpty() || xbStr.isEmpty()) {
        displayDebug("❗请输入完整的 p、XA、XB！");
        return;
    }

    p = BigInt(pStr);
    XA = BigInt(xaStr);
    XB = BigInt(xbStr);

    // 开始密钥协商
    performKeyAgreement();
}
