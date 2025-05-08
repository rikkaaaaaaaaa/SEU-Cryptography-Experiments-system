// EIGamal.h
#ifndef EIGAMAL_H
#define EIGAMAL_H

#include <QDialog>
#include <QString>
#include "BigInt.h"

namespace Ui {
class EIGamal;
}

class EIGamal : public QDialog {
    Q_OBJECT

public:
    explicit EIGamal(QWidget *parent = nullptr);
    ~EIGamal();
    BigInt stringToBigInt(const QString &text);
    QString bigIntToString(const BigInt &num);

private slots:
    void on_generatePGButton_clicked();
    void on_generateKeysButton_clicked();
    void on_encryptButton_clicked();
    void on_decryptButton_clicked();

private:
    Ui::EIGamal *ui;
    BigInt p, g, x, y, k;
    QList<QPair<BigInt, BigInt>> storedCiphertext;


    bool isPrime(const BigInt &n);
    BigInt generateLargePrime(int digits);
    void displayDebug(const QString &text);
};

#endif // EIGAMAL_H
