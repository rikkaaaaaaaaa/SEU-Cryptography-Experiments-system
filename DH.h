// DH.h
#ifndef DH_H
#define DH_H

#include <QDialog>
#include "BigInt.h"

namespace Ui {
class DH;
}

class DH : public QDialog {
    Q_OBJECT

public:
    explicit DH(QWidget *parent = nullptr);
    ~DH();

private slots:
    void on_generateAllButton_clicked();  // 自动生成
    void on_startButton_clicked();        // 手动输入启动

private:
    Ui::DH *ui;

    BigInt p, g, XA, XB, YA, YB, KA, KB;

    bool isPrime(const BigInt &n);
    BigInt generateLargePrime(int digits);
    void displayDebug(const QString &text);

    void performKeyAgreement(); // 统一的密钥协商逻辑
};

#endif // DH_H
