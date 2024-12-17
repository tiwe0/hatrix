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
        Position get_entity_position(std::string entity_id);

    private:
        World *world;

        std::vector<Entity *> entities_vec;
        std::map<std::string, Position> entities_to_position;
        std::map<Position, std::vector<std::string>> position_to_entities;
};

#endif