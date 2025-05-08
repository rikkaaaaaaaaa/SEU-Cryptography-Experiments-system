#include "encryption.h"
#include <openssl/aes.h>
#include <openssl/des.h>
#include <QCryptographicHash>
#include "encryption.h"
#include <QCryptographicHash>
#include <QDebug>
#include <openssl/evp.h>
#include <openssl/rand.h>


// 生成随机密钥（AES 32 字节，DES 8 字节）
QString Encryption::generateRandomKey(bool isAES)
{
    QByteArray key = QCryptographicHash::hash("my_secret_key", QCryptographicHash::Sha256);
    return isAES ? key.toHex().left(32) : key.toHex().left(16); // AES 256 位，DES 64 位（16 进制 16 长度）
}

// **🔹 AES 加密（EVP 方式）**
QString Encryption::aesEncrypt(const QString &plaintext, const QString &key)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return QString();

    QByteArray keyData = key.toUtf8().left(32); // AES 256-bit 密钥
    QByteArray iv(16, '\0'); // AES 需要 16 字节 IV

    QByteArray plainData = plaintext.toUtf8();
    QByteArray encryptedData(plainData.size() + AES_BLOCK_SIZE, Qt::Uninitialized);
    int outlen, totalLen = 0;

    // 初始化 AES CBC 加密
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(keyData.data()), reinterpret_cast<const unsigned char *>(iv.data()));

    // 执行加密
    EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char *>(encryptedData.data()), &outlen, reinterpret_cast<const unsigned char *>(plainData.data()), plainData.size());
    totalLen += outlen;

    // 处理最终块
    EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(encryptedData.data()) + totalLen, &outlen);
    totalLen += outlen;
    EVP_CIPHER_CTX_free(ctx);

    encryptedData.resize(totalLen);
    return encryptedData.toHex();
}

// **🔹 AES 解密（EVP 方式）**
QString Encryption::aesDecrypt(const QString &ciphertext, const QString &key)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return QString();

    QByteArray keyData = key.toUtf8().left(32);
    QByteArray iv(16, '\0');

    QByteArray cipherData = QByteArray::fromHex(ciphertext.toUtf8());
    QByteArray decryptedData(cipherData.size(), Qt::Uninitialized);
    int outlen, totalLen = 0;

    // 初始化 AES CBC 解密
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(keyData.data()), reinterpret_cast<const unsigned char *>(iv.data()));

    // 执行解密
    EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char *>(decryptedData.data()), &outlen, reinterpret_cast<const unsigned char *>(cipherData.data()), cipherData.size());
    totalLen += outlen;

    // 处理最终块
    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(decryptedData.data()) + totalLen, &outlen);
    totalLen += outlen;
    EVP_CIPHER_CTX_free(ctx);

    decryptedData.resize(totalLen);
    return QString::fromUtf8(decryptedData);
}




// // 生成随机密钥（AES 32 字节，DES 8 字节）
// QString Encryption::generateRandomKey(bool isAES)
// {
//     QByteArray key = QCryptographicHash::hash("my_secret_key", QCryptographicHash::Sha256);
//     return isAES ? key.toHex().left(32) : key.toHex().left(16); // AES 256 位，DES 64 位（16 进制 16 长度）
// }

// // **AES 加密**
// QString Encryption::aesEncrypt(const QString &plaintext, const QString &key)
// {
//     QByteArray plainData = plaintext.toUtf8();
//     QByteArray keyData = key.toUtf8().left(32); // AES 需要 256-bit 密钥

//     AES_KEY encryptKey;
//     AES_set_encrypt_key(reinterpret_cast<const unsigned char *>(keyData.data()), 256, &encryptKey);

//     QByteArray encryptedData(plainData.size(), Qt::Uninitialized);
//     AES_encrypt(reinterpret_cast<const unsigned char *>(plainData.data()),
//                 reinterpret_cast<unsigned char *>(encryptedData.data()), &encryptKey);

//     return encryptedData.toHex();
// }

// // **AES 解密**
// QString Encryption::aesDecrypt(const QString &ciphertext, const QString &key)
// {
//     QByteArray cipherData = QByteArray::fromHex(ciphertext.toUtf8());
//     QByteArray keyData = key.toUtf8().left(32);

//     AES_KEY decryptKey;
//     AES_set_decrypt_key(reinterpret_cast<const unsigned char *>(keyData.data()), 256, &decryptKey);

//     QByteArray decryptedData(cipherData.size(), Qt::Uninitialized);
//     AES_decrypt(reinterpret_cast<const unsigned char *>(cipherData.data()),
//                 reinterpret_cast<unsigned char *>(decryptedData.data()), &decryptKey);

//     return QString::fromUtf8(decryptedData);
// }

// **🔹DES 加密（补全）**
QString Encryption::desEncrypt(const QString &plaintext, const QString &key)
{
    QByteArray plainData = plaintext.toUtf8();
    QByteArray keyData = key.toUtf8().left(8); // DES 需要 8 字节密钥
    QByteArray iv = "12345678"; // 8 字节 IV

    DES_cblock desKey;
    DES_key_schedule keySchedule;

    // 设置密钥
    memcpy(desKey, keyData.data(), 8);
    DES_set_key_unchecked(&desKey, &keySchedule);

    // **PKCS5Padding：填充数据至 8 字节倍数**
    int padding = 8 - (plainData.size() % 8);
    for (int i = 0; i < padding; ++i)
        plainData.append(static_cast<char>(padding));

    QByteArray encryptedData(plainData.size(), Qt::Uninitialized);
    DES_cblock ivec;
    memcpy(ivec, iv.data(), 8);

    // **执行 CBC 模式加密**
    DES_ncbc_encrypt(reinterpret_cast<const unsigned char *>(plainData.data()),
                     reinterpret_cast<unsigned char *>(encryptedData.data()),
                     plainData.size(), &keySchedule, &ivec, DES_ENCRYPT);

    return encryptedData.toHex();
}

// **🔹DES 解密（补全）**
QString Encryption::desDecrypt(const QString &ciphertext, const QString &key)
{
    QByteArray cipherData = QByteArray::fromHex(ciphertext.toUtf8());
    QByteArray keyData = key.toUtf8().left(8);
    QByteArray iv = "12345678"; // 8 字节 IV

    DES_cblock desKey;
    DES_key_schedule keySchedule;

    // 设置密钥
    memcpy(desKey, keyData.data(), 8);
    DES_set_key_unchecked(&desKey, &keySchedule);

    QByteArray decryptedData(cipherData.size(), Qt::Uninitialized);
    DES_cblock ivec;
    memcpy(ivec, iv.data(), 8);

    // **执行 CBC 模式解密**
    DES_ncbc_encrypt(reinterpret_cast<const unsigned char *>(cipherData.data()),
                     reinterpret_cast<unsigned char *>(decryptedData.data()),
                     cipherData.size(), &keySchedule, &ivec, DES_DECRYPT);

    // **去除 PKCS5Padding**
    int padding = static_cast<int>(decryptedData.at(decryptedData.size() - 1));
    decryptedData.chop(padding);

    return QString::fromUtf8(decryptedData);
}
