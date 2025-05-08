#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QString>

class Encryption
{
public:
    static QString generateRandomKey(bool isAES);  // 生成 AES / DES 随机密钥

    static QString aesEncrypt(const QString &plaintext, const QString &key);  // AES 加密
    static QString aesDecrypt(const QString &ciphertext, const QString &key); // AES 解密

    static QString desEncrypt(const QString &plaintext, const QString &key);  // DES 加密
    static QString desDecrypt(const QString &ciphertext, const QString &key); // DES 解密
};

#endif // ENCRYPTION_H
