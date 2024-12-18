#ifndef HATRIX_UTILS_POSTION
#define HATRIX_UTILS_POSTION
struct Vec2
{
    int x, y;

    bool operator<(const Vec2& other) const {
        if (x == other.x) {
            return y < other.y; // 若 x 相等，按 y 排序
        }
        return x < other.x; // 按 x 排序
    }
};

#endif
