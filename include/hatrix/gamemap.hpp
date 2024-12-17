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
        const std::vector<Entity *> enumerate_entities_at(int x, int y);

        bool is_opaque(int x, int y);

        Position get_entity_position(std::string entity_id);

        const std::vector<Position> compute_fov(int x, int y, int distance);
        void mark_as_visible(int x, int y);

        std::vector<Position> visible_position;

        void update_fov();

    private:
        World *world;

        std::vector<Entity *> entities_vec;
        std::map<std::string, Entity*> entity_ids_to_entity;
        std::map<std::string, Position> entities_to_position;
        std::map<Position, std::vector<std::string>> position_to_entities;
};

#endif