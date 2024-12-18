#ifndef HATRIX_GAMEMAP
#define HATRIX_GAMEMAP

#include <map>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include "hatrix/utils/position.hpp"

class World;
class Entity;
struct Position;

class GamemapCell {
    public:
        GamemapCell(int x, int y);
        ~GamemapCell();

        bool blocking;
        bool opaque;
        Position position;
        std::list<Entity *> entities;

        void update_blocking();
        void update_opaque();

        void add_entity(Entity *entity);
        void del_entity(Entity *entity);
        Entity* pop_entity(Entity *entity);
};

class Gamemap
{
public:
    Gamemap(World *world, int width=-1, int height=-1);
    ~Gamemap();

    // helper
    GamemapCell *get_cell(int x, int y);

    // entity api
    // 增
    void add_entity(Entity *entity);
    // 删
    void remove_entity(Entity *entity);
    // 改
    void move_entity(Entity *entity, int dx, int dy);

    // 查
    // 查询 所有 entity
    const std::list<Entity *> &enumerate_entities();
    // 查询某一位置所有 entity
    const std::list<Entity *> &enumerate_entities_at(int x, int y);

    // map cell api
    bool is_opaque(int x, int y);
    bool is_blocking(int x, int y);

    std::vector<Position> visible_position;

    bool should_update_fov;
    bool should_render_fov;
    void doupdate_fov();
    void update_fov();

private:
    // -1 时表示无限大
    int width, height;

    bool update_flag = false;
    World *world;

    std::list<Entity *> static_entities;
    std::list<Entity *> normal_entities;

    std::map<Position, GamemapCell *> position_bucket;
};

#endif