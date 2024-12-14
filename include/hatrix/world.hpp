// 存储游戏数据的上下文
#ifndef HATRIX_WORLD
#define HATRIX_WORLD
#include "hatrix/entities/entity.hpp"
#include <vector>
#include <string>
#include <map>
#include <algorithm>

class Entity;

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

class World
{
    public:
        World();
        ~World();

        bool should_quit;
        const std::vector<Entity *> &enumerate_entities();

        Position get_entity_position(std::string entity_id);

        void add_entity(Entity *entity, int x, int y);
        void remove_entity(Entity *entity);
        void move_entity(Entity *entity, int dx, int dy);

    private:
        std::vector<Entity *> entities_vec;
        std::map<std::string, Position> entities_to_position;
        std::map<Position, std::vector<std::string>> position_to_entities;
};
#endif