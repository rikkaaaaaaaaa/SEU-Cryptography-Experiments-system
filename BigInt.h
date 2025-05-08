// BigInt.h
#ifndef BIGINT_H
#define BIGINT_H

#include <QString>
#include <vector>
#include <string>

class BigInt {
public:
    BigInt();
    BigInt(const QString &number);
    BigInt(const std::string &number);
    BigInt(int number);
    BigInt trans();

    int size();
    QString toString() const;

    BigInt operator+(const BigInt &other) const;
    BigInt operator-(const BigInt &other) const;
    BigInt operator-() const;
    BigInt operator*(const BigInt &other) const;
    BigInt operator/(const BigInt &other) const;
    BigInt operator%(const BigInt &mod) const;
    friend BigInt operator^(const BigInt &a, const BigInt &b);


    bool operator==(const BigInt &other) const;
    bool operator<(const BigInt &other) const;
    bool operator>(const BigInt &other) const;
    bool operator<=(const BigInt &other) const;
    bool operator>=(const BigInt &other) const;
    bool operator!=(const BigInt &other) const;


    BigInt modPow(const BigInt &exp, const BigInt &mod) const;
    BigInt modInverse(const BigInt &mod) const;
    static BigInt gcd(BigInt a, BigInt b);

    int bitLength() const;

private:
    std::vector<int> digits;
    bool isNegative;

    void removeLeadingZeros();
    BigInt abs() const;
    int compare(const BigInt &other) const;
};

#endif // BIGINT_H
