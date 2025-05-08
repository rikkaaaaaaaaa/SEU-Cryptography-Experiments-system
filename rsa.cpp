#include "rsa.h"
#include "qtextedit.h"
#include "ui_rsa.h"
#include <QMessageBox> // 添加头文件以弹出提示框
#include <QRandomGenerator>

RSA::RSA(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RSA)
{
    p = BigInt(61);
    q = BigInt(53);
    n = p * q;
    phi = (p - BigInt(1)) * (q - BigInt(1));
    e = BigInt(17); // 公开指数
    d = e.modInverse(phi);
    ui->setupUi(this);
}

RSA::~RSA()
{
    delete ui;
}

// bool isPrime(const BigInt &n) {
//     if (n < BigInt(2)) return false;
//     if (n == BigInt(2)) return true;
//     if ((n % BigInt(2)) == BigInt(0)) return false;

//     BigInt i = BigInt(3);
//     BigInt one = BigInt(1);
//     while (i * i <= n) {
//         if ((n % i) == BigInt(0)) return false;
//         i = i + BigInt(2);
//     }
//     return true;
// }

bool RSA::isPrime(const BigInt &num) {
    if (num < BigInt(2)) return false;
    for (BigInt i = BigInt(2); i * i <= num; i = i + BigInt(1)) {
        if (num % i == BigInt(0)) return false;
    }
    return true;
}

void RSA::logOutput(const QString &text) {
    if (ui->outputEdit)
        ui->outputEdit->append(text);
}

bool RSA::generateKeys(const BigInt &p_, const BigInt &q_) {
    p = p_;
    q = q_;

    logOutput("开始生成密钥...");
    if (!isPrime(p)) {
        logOutput("输入的 p 不是素数，终止。");
        return 0;
    }
    if (!isPrime(q)) {
        logOutput("输入的 q 不是素数，终止。");
        return 0;
    }

    n = p * q;
    phi = (p - BigInt(1)) * (q - BigInt(1));
    e = BigInt(65537); // 常用公钥
    d = e.modInverse(phi);

    logOutput("p: " + p.toString());
    logOutput("q: " + q.toString());
    logOutput("n = p * q: " + n.toString());
    logOutput("phi = (p-1)*(q-1): " + phi.toString());
    logOutput("e (公钥): " + e.toString());
    logOutput("d (私钥): " + d.toString());
    return 1;
}

QString RSA::encrypt(const QString &message) {
    logOutput("开始加密消息：" + message);
    QString result;
    for (QChar ch : message) {
        BigInt m(ch.unicode());//采取Unicode编码
        BigInt c = m.modPow(e, n);
        logOutput(QString("字符 '%1' -> 明文数值 %2 -> 密文 %3")
                      .arg(ch).arg(m.toString()).arg(c.toString()));
        result += c.toString() + " ";
    }
    return result.trimmed();
}

QString RSA::decrypt(const QString &cipherText) {
    logOutput("开始解密密文：" + cipherText);
    QStringList parts = cipherText.split(" ");
    QString result;
    for (const QString &part : parts) {
        BigInt c(part);
        BigInt m = c.modPow(d, n);
        logOutput(QString("密文 %1 -> 明文数值 %2 -> 字符 '%3'")
                      .arg(c.toString()).arg(m.toString()).arg(QChar(m.toString().toInt())));
        result += QChar(m.toString().toInt());
    }
    return result;
}

// void RSA::generateKeys(const BigInt &pInput, const BigInt &qInput) {
//     debugInfo.clear();

//     if (!isPrime(pInput) || !isPrime(qInput)) {
//         debugInfo = "p 或 q 不是素数，程序终止。";
//         return;
//     }

//     p = pInput;
//     q = qInput;
//     n = p * q;
//     phi = (p - BigInt(1)) * (q - BigInt(1));
//     e = BigInt(65537); // 常用公开指数
//     d = e.modInverse(phi);

//     debugInfo += "p: " + p.toString() + "\n";
//     debugInfo += "q: " + q.toString() + "\n";
//     debugInfo += "n: " + n.toString() + "\n";
//     debugInfo += "phi: " + phi.toString() + "\n";
//     debugInfo += "e: " + e.toString() + "\n";
//     debugInfo += "d: " + d.toString() + "\n";
// }

// QString RSA::encrypt(const QString &message) {
//     QString result;
//     debugInfo += "\n[加密过程]\n";
//     for (QChar ch : message) {
//         BigInt m(ch.unicode());
//         BigInt c = m.modPow(e, n);
//         result += c.toString() + " ";
//         debugInfo += QString("m=%1 -> c=%2\n").arg(m.toString()).arg(c.toString());
//     }
//     return result.trimmed();
// }

// QString RSA::decrypt(const QString &cipher) {
//     QStringList list = cipher.split(" ", Qt::SkipEmptyParts);
//     QString result;
//     debugInfo += "\n[解密过程]\n";
//     for (const QString &numStr : list) {
//         BigInt c(numStr);
//         BigInt m = c.modPow(d, n);
//         result += QChar(m.toString().toInt());
//         debugInfo += QString("c=%1 -> m=%2\n").arg(c.toString()).arg(m.toString());
//     }
//     return result;
// }

QString RSA::getDebugInfo() const {
    return debugInfo;
}


void RSA::setDebugOutput(QTextEdit* outputEdit) {
    debugOutput = outputEdit;
}

BigInt RSA::generateLargePrime(int digits) {
    while (true) {
        QString numStr;
        numStr += QString::number(QRandomGenerator::global()->bounded(1, 9)); // First digit non-zero
        for (int i = 1; i < digits; ++i)
            numStr += QString::number(QRandomGenerator::global()->bounded(0, 10));
        BigInt candidate(numStr);
        if (isPrime(candidate))
            return candidate;
    }
}


BigInt RSA::getPublicKeyE() const { return e; }
BigInt RSA::getPublicKeyN() const { return n; }
BigInt RSA::getPrivateKeyD() const { return d; }


// void RSA::on_Start_clicked()
// {
//     BigInt p(ui->inputPLineEdit->text());
//     BigInt q(ui->inputQLineEdit->text());

//     setDebugOutput(ui->outputEdit);
//     generateKeys(p, q); // 若p或q不是素数，将弹窗提示并中止

// }


void RSA::on_setPQ_clicked()
{
    BigInt p = generateLargePrime(10);  // 可根据需要调整位数
    BigInt q = generateLargePrime(10);
    ui->inputPLineEdit->setText(p.toString());
    ui->inputQLineEdit->setText(q.toString());
}


void RSA::on_Encrypt_clicked()
{
    BigInt p(ui->inputPLineEdit->text());
    BigInt q(ui->inputQLineEdit->text());
    setDebugOutput(ui->outputEdit);
    if(generateKeys(p, q)) // 若p或q不是素数，将弹窗提示并中止
    {
        QString plaintext = ui->InputEdit->toPlainText();
        if (plaintext.isEmpty()) {
            QMessageBox::warning(this, "警告", "请输入需要加密的明文！");
            return;
        }

        QString cipher = encrypt(plaintext);
        ui->outputEdit->append("加密结果：\n" + cipher);
    }

}


void RSA::on_Decrypt_clicked()
{
    BigInt p(ui->inputPLineEdit->text());
    BigInt q(ui->inputQLineEdit->text());
    setDebugOutput(ui->outputEdit);
    if(generateKeys(p, q)) // 若p或q不是素数，将弹窗提示并中止
    {
        QString cipherText = ui->InputEdit->toPlainText();
        if (cipherText.isEmpty()) {
            QMessageBox::warning(this, "警告", "请输入需要解密的密文！");
            return;
        }

        QString plaintext = decrypt(cipherText);
        ui->outputEdit->append("解密结果：\n" + plaintext);
    }

}

