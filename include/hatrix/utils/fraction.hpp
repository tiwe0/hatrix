#ifndef HATRIX_UTILS_FRACTION
#define HATRIX_UTILS_FRACTION

#include <iostream>
#include <numeric> // std::gcd in C++17
#include <cmath>   // std::round for float conversions

class Fraction {
private:
    int numerator;   // 分子
    int denominator; // 分母

    // 化简分数
    void simplify() {
        int gcd = std::gcd(numerator, denominator); // C++17 中的 gcd
        numerator /= gcd;
        denominator /= gcd;

        // 让分母为正
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    // 构造函数
    Fraction(int num, int den) : numerator(num), denominator(den) {
        if (den == 0) {
            throw std::invalid_argument("Denominator cannot be zero.");
        }
        simplify();
    }

    Fraction(int num) : numerator(num), denominator(1) {} // 整数构造函数

    Fraction(float value) { // 浮点数构造函数
        // 将浮点数转为分数
        const int precision = 1000000; // 精度控制
        numerator = static_cast<int>(std::round(value * precision));
        denominator = precision;
        simplify();
    }

    // 加法
    Fraction operator+(const Fraction& other) const {
        int num = numerator * other.denominator + other.numerator * denominator;
        int den = denominator * other.denominator;
        return Fraction(num, den);
    }

    Fraction operator+(int value) const {
        return *this + Fraction(value); // 转为分数再相加
    }

    Fraction operator+(float value) const {
        return *this + Fraction(value); // 转为分数再相加
    }

    // 减法
    Fraction operator-(const Fraction& other) const {
        int num = numerator * other.denominator - other.numerator * denominator;
        int den = denominator * other.denominator;
        return Fraction(num, den);
    }

    Fraction operator-(int value) const {
        return *this - Fraction(value); // 转为分数再相减
    }

    Fraction operator-(float value) const {
        return *this - Fraction(value); // 转为分数再相减
    }

    // 乘法
    Fraction operator*(const Fraction& other) const {
        int num = numerator * other.numerator;
        int den = denominator * other.denominator;
        return Fraction(num, den);
    }

    Fraction operator*(int value) const {
        return *this * Fraction(value); // 转为分数再相乘
    }

    Fraction operator*(float value) const {
        return *this * Fraction(value); // 转为分数再相乘
    }

    // 除法
    Fraction operator/(const Fraction& other) const {
        if (other.numerator == 0) {
            throw std::invalid_argument("Cannot divide by zero.");
        }
        int num = numerator * other.denominator;
        int den = denominator * other.numerator;
        return Fraction(num, den);
    }

    Fraction operator/(int value) const {
        if (value == 0) {
            throw std::invalid_argument("Cannot divide by zero.");
        }
        return *this / Fraction(value); // 转为分数再相除
    }

    Fraction operator/(float value) const {
        if (value == 0.0f) {
            throw std::invalid_argument("Cannot divide by zero.");
        }
        return *this / Fraction(value); // 转为分数再相除
    }

    // 比较运算符
    bool operator==(const Fraction& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }

    bool operator!=(const Fraction& other) const {
        return !(*this == other);
    }

    bool operator<(const Fraction& other) const {
        return numerator * other.denominator < other.numerator * denominator;
    }

    bool operator<=(const Fraction& other) const {
        return *this < other || *this == other;
    }

    bool operator>(const Fraction& other) const {
        return !(*this <= other);
    }

    bool operator>=(const Fraction& other) const {
        return !(*this < other);
    }

    float toFloat() const {
        return static_cast<float>(numerator) / static_cast<float>(denominator);
    }

    // 输出分数
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        os << f.numerator << "/" << f.denominator;
        return os;
    }
};

#endif