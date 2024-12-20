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
class Character;
class Gamemap;

class PathFinder{
    public:
        PathFinder(Gamemap *gamemap);

        std::vector<Vec2> get_path(int start_x, int start_y, int end_x, int end_y);

    private:
        Gamemap *gamemap;
};

class GamemapCell {
    public:
        GamemapCell(int x, int y);
        ~GamemapCell();

        bool blocking;
        bool opaque;
        Vec2 position;
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
    // 查询某一位置 第一个 符合条件的 entity, 没有则返回 nullptr
    Entity *get_first_entity_at_which(int x, int y, const std::function<bool(Entity *)>& cond);
    // 查询某一位置 渲染 entity, 没有则返回 nullptr
    Entity *get_render_entity_at(int x, int y);

    // 查询某个位置是否有符合条件的entity
    bool has_entity_at_which(int x, int y,const std::function<bool(Entity *)>& cond);

    void update_wall_glyph(int x, int y);
    void update_wall_glyph_in_range(int x, int y);

    // map cell api
    bool is_opaque(int x, int y);
    bool is_blocking(int x, int y);

    // door
    bool open(int x, int y);
    bool close(int x, int y);

    // vec
    std::vector<Vec2> visible_position;

    // path
    std::vector<Vec2> get_path(int start_x, int start_y, int end_x, int end_y);

    bool should_update_fov;
    bool should_render_fov;
    void doupdate_fov();
    void update_fov();
    void update_fov(Character *character);

private:
    // -1 时表示无限大
    int width, height;

    bool update_flag = false;
    World *world;
    PathFinder *pathfinder;

    std::list<Entity *> static_entities;
    std::list<Entity *> normal_entities;

    std::map<Vec2, GamemapCell *> position_bucket;
};

#endif