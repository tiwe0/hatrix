#include "hatrix/utils/position.hpp"
#include "hatrix/utils/pathfinder.hpp"

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <functional>

// 哈希函数，使 Vec2 能作为 unordered_map 的 key
namespace std {
    template <>
    struct hash<Vec2> {
        size_t operator()(const Vec2& v) const {
            return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
        }
    };
}

struct Node {
    Vec2 position;
    int g_cost; // 从起点到当前节点的代价
    int h_cost; // 从当前节点到目标节点的启发式代价
    int f_cost() const { return g_cost + h_cost; } // f(n) = g(n) + h(n)
    Vec2 parent; // 父节点，用于回溯路径

    Node() : position(0, 0), g_cost(0), h_cost(0), parent(0, 0) {}
    Node(Vec2 pos, int g, int h, Vec2 parent)
        : position(pos), g_cost(g), h_cost(h), parent(parent) {}
};

std::vector<Vec2> utils_compute_path(Vec2 start, Vec2 target, int radius, std::function<bool(int, int)> is_blocking) {
    std::unordered_map<Vec2, Node> open_list;  // 存储待评估的节点
    std::unordered_map<Vec2, bool> closed_list; // 存储已评估的节点

    open_list.insert(std::pair(start, Node(start, 0, start.manhattan_distance(target), start)));

    std::vector<Vec2> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // 4个方向（上下左右）

    while (!open_list.empty()) {
        // 选择 f 值最小的节点
        auto current_iter = open_list.begin();
        for (auto it = open_list.begin(); it != open_list.end(); ++it) {
            if (it->second.f_cost() < current_iter->second.f_cost()) {
                current_iter = it;
            }
        }
        Vec2 current_pos = current_iter->first;
        Node current_node = current_iter->second;

        // 如果到达目标，构建路径
        if (current_pos == target) {
            std::vector<Vec2> path;
            Vec2 path_node = target;
            while (path_node != start) {
                path.push_back(path_node);
                path_node = current_node.parent;
                current_node = open_list[path_node];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        open_list.erase(current_pos);  // 移除当前节点

        // 将当前节点加入到 closed_list
        closed_list[current_pos] = true;

        // 检查相邻的节点
        for (const auto& dir : directions) {
            Vec2 neighbor(current_pos.x + dir.x, current_pos.y + dir.y);

            // 忽略不可通行或已经访问过的节点
            if (is_blocking(neighbor.x, neighbor.y) || closed_list.count(neighbor)) {
                continue;
            }

            // 计算 G 和 H 值
            int g_cost = current_node.g_cost + 1;  // 假设每步的代价为 1
            int h_cost = neighbor.manhattan_distance(target);  // 使用曼哈顿距离作为启发式函数

            // 如果节点已在 open_list 中且新的路径更优，更新它
            if (open_list.count(neighbor) == 0 || g_cost < open_list[neighbor].g_cost) {
                open_list[neighbor] = Node(neighbor, g_cost, h_cost, current_pos);
            }
        }
    }

    // 如果没有路径找到，返回空路径
    return {};
}
