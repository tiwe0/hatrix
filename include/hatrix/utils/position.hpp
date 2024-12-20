#ifndef HATRIX_UTILS_POSTION
#define HATRIX_UTILS_POSTION
#include <cmath>
struct Vec2
{
    int x, y;

    Vec2(int x = 0, int y = 0) : x(x), y(y) {};

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    };

    bool operator!=(const Vec2& other) const {
        return x != other.x || y != other.y;
    };

    bool operator<(const Vec2& other) const {
        if (x == other.x) {
            return y < other.y; // 若 x 相等，按 y 排序
        }
        return x < other.x; // 按 x 排序
    };

    Vec2 operator+(const Vec2 &other) const
    {
        return Vec2{x + other.x, y + other.y};
    };

    Vec2 operator-(const Vec2 &other) const
    {
        return Vec2{x - other.x, y - other.y};
    };

    float norm() const
    {
        return std::sqrtf(x * x + y * y);
    };

    // 计算曼哈顿距离
    int manhattan_distance(const Vec2& target) const {
        return std::abs(x - target.x) + std::abs(y - target.y);
    };

    // 计算欧几里得距离
    float euclidean_distance(const Vec2& target) const {
        return std::sqrt((x - target.x) * (x - target.x) + (y - target.y) * (y - target.y));
    };
};

#endif
