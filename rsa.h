#ifndef RSA_H
#define RSA_H

#include <QDialog>
#include "BigInt.h"
#include "qtextedit.h"
namespace Ui {
class RSA;
}

class RSA : public QDialog
{
    Q_OBJECT

public:
    explicit RSA(QWidget *parent = nullptr);
    ~RSA();
    bool generateKeys(const BigInt &p, const BigInt &q);
    QString encrypt(const QString &message);
    QString decrypt(const QString &cipher);
    void setDebugOutput(QTextEdit* outputEdit); // 设置输出窗口
    //void logDebug(const QString& msg);          // 调试信息写入窗口
    QString getDebugInfo() const;
    void logOutput(const QString &text);

    BigInt generateLargePrime(int digits);
    BigInt getPublicKeyE() const;
    BigInt getPublicKeyN() const;
    BigInt getPrivateKeyD() const;

private slots:
    //void on_generatePQButton_clicked();

    // void on_Start_clicked();

    void on_setPQ_clicked();

    void on_Encrypt_clicked();

    void on_Decrypt_clicked();

private:
    BigInt p, q, n, phi;
    BigInt e, d;
    QString debugInfo;
    bool isPrime(const BigInt &num);
    BigInt stringToBigInt(const QString &text);
    QString bigIntToString(const BigInt &bigInt);
    QTextEdit* debugOutput = nullptr;
    Ui::RSA *ui;
};

#endif // RSA_H
