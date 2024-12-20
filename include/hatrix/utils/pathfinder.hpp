#ifndef HATRIX_UTILS_PATHFINDER
#define HATRIX_UTILS_PATHFINDER

#include <functional>
#include <vector>
struct Vec2;

std::vector<Vec2> utils_compute_path(Vec2 start, Vec2 end, std::function<bool(int, int)> is_blocking);

#endif