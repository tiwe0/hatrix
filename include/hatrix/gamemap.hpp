#ifndef HATRIX_GAMEMAP
#define HATRIX_GAMEMAP

#include <map>
#include <vector>
#include <string>

class World;
class Entity;
struct Position;

class Gamemap{
    public:
        Gamemap(World* world);
        ~Gamemap();

        void add_entity(Entity *entity, int x, int y);
        void remove_entity(Entity *entity);
        void move_entity(Entity *entity, int dx, int dy);
        const std::vector<Entity *> &enumerate_entities();
        const std::vector<Entity *>enumerate_entities_at(int x, int y);
        Position get_entity_position(std::string entity_id);
        void update_visiable();

        bool is_opaque(int x, int y);

        bool is_transparent(int x, int y);

        std::vector<Position> visiable_positions;

    private:
        World *world;

        // 追踪所有实体
        std::vector<Entity *> entities_vec;

        // 根据实体 id 追踪实体
        std::map<std::string, Entity *> entity_id_to_entity;

        // 根据实体 id 追踪 位置
        std::map<std::string, Position> entities_to_position;

        // 根据位置追踪 实体id
        std::map<Position, std::vector<std::string>> position_to_entities;
};

#endif