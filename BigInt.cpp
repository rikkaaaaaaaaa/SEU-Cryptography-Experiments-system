// BigInt.cpp
#include "BigInt.h"
#include <algorithm>
#include <sstream>
#include <cmath>

BigInt::BigInt() : isNegative(false) {
    digits.push_back(0);
}

BigInt::BigInt(const QString &number) : BigInt(number.toStdString()) {}

BigInt::BigInt(const std::string &number) {
    isNegative = false;
    int start = 0;
    if (number[0] == '-') {
        isNegative = true;
        start = 1;
    }
    for (int i = number.length() - 1; i >= start; --i) {
        digits.push_back(number[i] - '0');
    }
    removeLeadingZeros();
}

BigInt::BigInt(int number) {
    isNegative = number < 0;
    if (number == 0) digits.push_back(0);
    number = std::abs(number);
    while (number > 0) {
        digits.push_back(number % 10);
        number /= 10;
    }
}

int BigInt::size()
{
    return this->size();
}
BigInt BigInt::trans()
{
    QString str = this->toString();
    return BigInt(str.mid(0,str.size()-1));
}
QString BigInt::toString() const {
    std::string str;
    if (isNegative && !(digits.size() == 1 && digits[0] == 0)) str += "-";
    for (int i = digits.size() - 1; i >= 0; --i) str += (digits[i] + '0');
    return QString::fromStdString(str);
}

void BigInt::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    if (digits.size() == 1 && digits[0] == 0) isNegative = false;
}

BigInt BigInt::operator-() const {
    BigInt result = *this;
    if (!(digits.size() == 1 && digits[0] == 0)) {
        result.isNegative = !isNegative;
    }
    return result;
}

BigInt BigInt::operator+(const BigInt &other) const {
    if (isNegative == other.isNegative) {
        BigInt result;
        result.isNegative = isNegative;
        result.digits.clear();
        int carry = 0;
        for (size_t i = 0; i < std::max(digits.size(), other.digits.size()) || carry; ++i) {
            int d1 = i < digits.size() ? digits[i] : 0;
            int d2 = i < other.digits.size() ? other.digits[i] : 0;
            int sum = d1 + d2 + carry;
            result.digits.push_back(sum % 10);
            carry = sum / 10;
        }
        return result;
    }
    return *this - (-other);
}

BigInt BigInt::operator-(const BigInt &other) const {
    if (isNegative != other.isNegative) return *this + (-other);

    if (abs() < other.abs()) return -(other - *this);

    BigInt result;
    result.digits.clear();
    result.isNegative = isNegative;
    int borrow = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
        int d1 = digits[i];
        int d2 = i < other.digits.size() ? other.digits[i] : 0;
        int sub = d1 - d2 - borrow;
        if (sub < 0) {
            sub += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.digits.push_back(sub);
    }
    result.removeLeadingZeros();
    return result;
}

BigInt BigInt::operator*(const BigInt &other) const {
    BigInt result;
    result.digits = std::vector<int>(digits.size() + other.digits.size());
    result.isNegative = isNegative != other.isNegative;

    for (size_t i = 0; i < digits.size(); ++i) {
        for (size_t j = 0; j < other.digits.size(); ++j) {
            result.digits[i + j] += digits[i] * other.digits[j];
            if (result.digits[i + j] >= 10) {
                result.digits[i + j + 1] += result.digits[i + j] / 10;
                result.digits[i + j] %= 10;
            }
        }
    }
    result.removeLeadingZeros();
    return result;
}

BigInt BigInt::operator/(const BigInt &other) const {
    BigInt quotient, remainder;
    quotient.digits.resize(digits.size());
    remainder = BigInt(0);
    quotient.isNegative = isNegative != other.isNegative;

    for (int i = digits.size() - 1; i >= 0; --i) {
        remainder.digits.insert(remainder.digits.begin(), digits[i]);
        remainder.removeLeadingZeros();
        int x = 0, l = 0, r = 10;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInt t = other.abs() * BigInt(m);
            if (t <= remainder) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        quotient.digits[i] = x;
        remainder = remainder - other.abs() * BigInt(x);
    }
    quotient.removeLeadingZeros();
    return quotient;
}

BigInt BigInt::operator%(const BigInt &mod) const {
    return *this - (*this / mod) * mod;
}

bool BigInt::operator<=(const BigInt &other) const {
    return (*this < other) || (*this == other);
}

bool BigInt::operator>=(const BigInt &other) const {
    return (*this > other) || (*this == other);
}

bool BigInt::operator!=(const BigInt &other) const {
    return !(*this == other);
}

BigInt BigInt::modPow(const BigInt &exp, const BigInt &mod) const {
    BigInt result(1);
    BigInt base = *this % mod;
    BigInt exponent = exp;
    while (!(exponent == BigInt(0))) {
        if ((exponent.digits[0] % 2) == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exponent = exponent / BigInt(2);
    }
    return result;
}

BigInt BigInt::gcd(BigInt a, BigInt b) {
    while (!(b == BigInt(0))) {
        BigInt r = a % b;
        a = b;
        b = r;
    }
    return a;
}

BigInt BigInt::modInverse(const BigInt &mod) const {
    BigInt a = *this, m = mod;
    BigInt m0 = m, y = BigInt(0), x = BigInt(1);
    while (!(a == BigInt(1))) {
        BigInt q = a / m;
        BigInt t = m;
        m = a % m, a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < BigInt(0)) x = x + m0;
    return x;
}

bool BigInt::operator==(const BigInt &other) const {
    return digits == other.digits && isNegative == other.isNegative;
}

bool BigInt::operator<(const BigInt &other) const {
    return compare(other) < 0;
}

bool BigInt::operator>(const BigInt &other) const {
    return compare(other) > 0;
}

int BigInt::compare(const BigInt &other) const {
    if (isNegative != other.isNegative)
        return isNegative ? -1 : 1;
    if (digits.size() != other.digits.size())
        return isNegative ? other.digits.size() - digits.size() : digits.size() - other.digits.size();
    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i])
            return isNegative ? other.digits[i] - digits[i] : digits[i] - other.digits[i];
    }
    return 0;
}

BigInt BigInt::abs() const {
    BigInt result = *this;
    result.isNegative = false;
    return result;
}

BigInt operator^(const BigInt &a, const BigInt &b) {
    BigInt x = a;
    BigInt y = b;
    QString result;

    QString sx = x.toString();
    QString sy = y.toString();

    // Pad shorter string with leading zeros
    int len = std::max(sx.length(), sy.length());
    sx = sx.rightJustified(len, '0');
    sy = sy.rightJustified(len, '0');

    for (int i = 0; i < len; ++i) {
        int d1 = sx[i].digitValue();
        int d2 = sy[i].digitValue();
        result += QString::number(d1 ^ d2);
    }

    return BigInt(result);
}

int BigInt::bitLength() const {
    if (*this == BigInt(0)) return 0;

    BigInt n = *this;
    int bits = 0;
    while (n > BigInt(0)) {
        n = n / BigInt(2);
        bits++;
    }
    return bits;
}
