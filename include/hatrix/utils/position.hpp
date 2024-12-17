#ifndef HATRIX_UTILS_POSTION
#define HATRIX_UTILS_POSTION
struct Position
{
    int x, y;

    bool operator<(const Position& other) const {
        if (x == other.x) {
            return y < other.y; // 若 x 相等，按 y 排序
        }
        return x < other.x; // 按 x 排序
    }
};

#endif
