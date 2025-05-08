// EIGamal.cpp
#include "EIGamal.h"
#include "ui_EIGamal.h"
#include <QMessageBox>
#include <QTime>

EIGamal::EIGamal(QWidget *parent) : QDialog(parent), ui(new Ui::EIGamal) {
    ui->setupUi(this);
}

EIGamal::~EIGamal() {
    delete ui;
}

bool EIGamal::isPrime(const BigInt &n) {
    if (n == BigInt(2)) return true;
    if (n < BigInt(2) || (n % BigInt(2) == BigInt(0))) return false;
    for (BigInt i = BigInt(3); i * i <= n; i = i + BigInt(2)) {
        if (n % i == BigInt(0)) return false;
    }
    return true;
}

BigInt EIGamal::generateLargePrime(int digits) {
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

void EIGamal::displayDebug(const QString &text) {
    ui->outputEdit->append(text);
}

BigInt EIGamal::stringToBigInt(const QString &text) {
    QByteArray bytes = text.toUtf8();
    QString numStr;
    for (char byte : bytes) {
        numStr += QString::number((uchar)byte).rightJustified(3, '0');
    }
    return BigInt(numStr);
}

QString EIGamal::bigIntToString(const BigInt &num) {
    QString numStr = num.toString();
    if (numStr.length() % 3 != 0) {
        int pad = 3 - (numStr.length() % 3);
        numStr = QString("0").repeated(pad) + numStr;
    }
    QByteArray result;
    for (int i = 0; i < numStr.length(); i += 3) {
        QString byteStr = numStr.mid(i, 3);
        char byte = static_cast<char>(byteStr.toInt());
        result.append(byte);
    }
    return QString::fromUtf8(result);
}

void EIGamal::on_generatePGButton_clicked() {
    p = generateLargePrime(10);
    g = generateLargePrime(2);
    ui->inputPLineEdit->setText(p.toString());
    ui->inputGLineEdit->setText(g.toString());
    displayDebug("Generated p: " + p.toString());
    displayDebug("Generated g: " + g.toString());
}

void EIGamal::on_generateKeysButton_clicked() {
    p = BigInt(ui->inputPLineEdit->text());
    g = BigInt(ui->inputGLineEdit->text());
    x = BigInt(10457);  // Fixed private key for demo
    y = g.modPow(x, p);
    ui->privateKeyEdit->setText(x.toString());
    displayDebug("Private key x: " + x.toString());
    displayDebug("Public key y: " + y.toString());
}

void EIGamal::on_encryptButton_clicked() {
    QString plaintext = ui->inputEdit->text();
    QByteArray bytes = plaintext.toUtf8();
    QList<QPair<BigInt, BigInt>> ciphertexts;
    storedCiphertext.clear();

    for (int i = 0; i < bytes.size(); ++i) {
        BigInt m(QString::number((uchar)bytes[i]));
        k = BigInt(30);  // Fixed k for demo
        BigInt a = g.modPow(k, p);
        BigInt b = (y.modPow(k, p) * m) % p;
        ciphertexts.append(qMakePair(a, b));
    }

    // QString plaintext = ui->inputEdit->text();
    // QByteArray bytes = plaintext.toUtf8();


    // QByteArray iv = "000"; // Initialization vector (CBC)

    // for (int i = 0; i < bytes.size(); ++i) {
    //     BigInt m(QString::number((uchar)bytes[i]));

    //     if (i > 0) {
    //         BigInt prev_b = ciphertexts.last().second;
    //         m = m ^ prev_b; // CBC XOR with previous b
    //     }

    //     k = BigInt(30);  // Fixed k for demo
    //     BigInt a = g.modPow(k, p);
    //     BigInt b = (y.modPow(k, p) * m) % p;
    //     ciphertexts.append(qMakePair(a, b));
    // }

    for (int i = 0; i < ciphertexts.size(); ++i) {
        ui->outputEdit->append("Cipher (a, b)[" + QString::number(i) + "]: (" + ciphertexts[i].first.toString() + ", " + ciphertexts[i].second.toString() + ")");
    }

    ui->outputEdit->append("Cipher bLine: ");
    for (int i = 0; i < ciphertexts.size(); ++i) {
        ui->outputEdit->append(ciphertexts[i].second.toString() + " ");
    }
    storedCiphertext = ciphertexts; // Store for decryption
}

// void EIGamal::on_decryptButton_clicked() {
//     QByteArray recovered;
//     QList<QPair<BigInt, BigInt>> ciphertexts;
//     BigInt a(ui->cipherALineEdit->text());
//     QStringList bList = ui->cipherBLineEdit->text().split(" ");

//     for (const QString &bStr : bList) {
//         BigInt b(bStr);
//         const BigInt a1 = a;
//         const BigInt b1 = b;
//         ciphertexts.append(qMakePair(a1, b1));
//     }

//     for (int i = 0; i < storedCiphertext.size(); ++i) {
//         BigInt a = storedCiphertext[i].first;
//         BigInt b = storedCiphertext[i].second;
//         BigInt s = a.modPow(x, p);
//         BigInt s_inv = s.modInverse(p);
//         BigInt m = (b * s_inv) % p;

//         // if (i > 0) {
//         //     BigInt prev_b = storedCiphertext[i - 1].second ;
//         //     m = (m ^ prev_b) % p; // CBC XOR back
//         // }

//         QString mStr = m.toString();
//         int charVal = mStr.toInt();
//         recovered.append(static_cast<char>(charVal));

//         displayDebug("Block " + QString::number(i) + ": s = " + s.toString() + ", s_inv = " + s_inv.toString() + ", m = " + m.toString());
//     }
//     ui->outputEdit->append("Decrypted message: " + QString::fromUtf8(recovered));
// }

// QString RSA::decrypt(const QString &cipherText) {
//     logOutput("开始解密密文：" + cipherText);
//     QStringList parts = cipherText.split(" ");
//     QString result;
//     for (const QString &part : parts) {
//         BigInt c(part);
//         BigInt m = c.modPow(d, n);
//         logOutput(QString("密文 %1 -> 明文数值 %2 -> 字符 '%3'")
//                       .arg(c.toString()).arg(m.toString()).arg(QChar(m.toString().toInt())));
//         result += QChar(m.toString().toInt());
//     }
//     return result;
// }


void EIGamal::on_decryptButton_clicked() {
    QList<QPair<BigInt, BigInt>> ciphertexts;
    BigInt a2(ui->cipherALineEdit->text());
    QStringList bList = ui->cipherBLineEdit->text().split(" ",Qt::SkipEmptyParts);

    for (const QString &bStr : bList) {
        BigInt b2(bStr);
        // const BigInt a1 = a2;
        // const BigInt b1 = b2;
        ciphertexts.append(qMakePair(a2, b2));
    }

    QByteArray recovered;

    for (int i = 0; i < ciphertexts.size(); ++i) {
        BigInt a = ciphertexts[i].first;
        BigInt b;
        if(i != 0)
        {
            QString tempstr = ciphertexts[i].second.toString().mid(1);
            b = BigInt(tempstr);
        }
        else
        {
            b = ciphertexts[i].second;
        }
        storedCiphertext.append(qMakePair(a, b));
        BigInt e0(1);
        BigInt s = a.modPow(p-e0-x, p);
        //BigInt s_inv = s.modInverse(p);
        BigInt m = (b * s) % p;//崩溃

        QString mStr = m.toString();
        int charVal = mStr.toInt();
        recovered.append(static_cast<char>(charVal));

        displayDebug("Block " + QString::number(i) + ": s = " + s.toString() +  ", m = " + m.toString());
    }
    ui->outputEdit->append("Decrypted message: " + QString::fromUtf8(recovered));
}


// void EIGamal::on_decryptButton_clicked() {
//     QList<QPair<BigInt, BigInt>> ciphertexts;
//     BigInt a(ui->cipherALineEdit->text());
//     QStringList bList = ui->cipherBLineEdit->text().split(" ", Qt::SkipEmptyParts);

//     for (const QString &bStr : bList) {
//         BigInt b(bStr);
//         ciphertexts.append(qMakePair(a, b));
//     }

//     QByteArray recovered;
//     for (int i = 0; i < ciphertexts.size(); ++i) {
//         BigInt a = ciphertexts[i].first;
//         BigInt b = ciphertexts[i].second;
//         BigInt s = a.modPow(x, p);
//         BigInt s_inv = s.modInverse(p);
//         BigInt m = (b * s_inv) % p;

//         if (i > 0) {
//             BigInt prev_b = ciphertexts[i - 1].second % p;
//             m = m ^ prev_b % p; // CBC XOR back
//         }

//         QString mStr = m.toString();
//         int charVal = mStr.toInt();
//         recovered.append(static_cast<char>(charVal));

//         displayDebug("Block " + QString::number(i) + ": s = " + s.toString() + ", s_inv = " + s_inv.toString() + ", m = " + m.toString());
//     }
//     ui->outputEdit->append("Decrypted message: " + QString::fromUtf8(recovered));
// }
// void EIGamal::on_decryptButton_clicked() {
//     QList<QPair<BigInt, BigInt>> ciphertexts;

//     QString aStr = ui->cipherALineEdit->text().trimmed();
//     QString bStrRaw = ui->cipherBLineEdit->text().trimmed();
//     if (aStr.isEmpty() || bStrRaw.isEmpty()) {
//         QMessageBox::warning(this, "Input Error", "Please enter both a and b values.");
//         return;
//     }

//     BigInt a;
//     try {
//         a = BigInt(aStr);
//     } catch (...) {
//         QMessageBox::critical(this, "Input Error", "Invalid 'a' value.");
//         return;
//     }

//     QStringList bList = bStrRaw.split(" ", Qt::SkipEmptyParts);
//     for (const QString &bStr : bList) {
//         try {
//             BigInt b(bStr);
//             ciphertexts.append(qMakePair(a, b));
//         } catch (...) {
//             QMessageBox::critical(this, "Input Error", "Invalid b value: " + bStr);
//             return;
//         }
//     }

//     QByteArray recovered;
//     for (int i = 0; i < ciphertexts.size(); ++i) {
//         BigInt a = ciphertexts[i].first;
//         BigInt b = ciphertexts[i].second;
//         BigInt s = a.modPow(x, p);
//         BigInt s_inv = s.modInverse(p);
//         BigInt m = (b * s_inv) % p;

//         if (i > 0) {
//             BigInt prev_b = ciphertexts[i - 1].second % p;
//             m = m ^ prev_b % p; // CBC XOR back
//         }

//         QString mStr = m.toString();
//         bool ok = false;
//         int charVal = mStr.toInt(&ok);
//         if (!ok || charVal < 0 || charVal > 255) {
//             QMessageBox::critical(this, "Decryption Error", "Decryption failed at block " + QString::number(i));
//             return;
//         }
//         recovered.append(static_cast<char>(charVal));

//         displayDebug("Block " + QString::number(i) + ": s = " + s.toString() + ", s_inv = " + s_inv.toString() + ", m = " + m.toString());
//     }

//     ui->outputEdit->append("Decrypted message: " + QString::fromUtf8(recovered));
// }
