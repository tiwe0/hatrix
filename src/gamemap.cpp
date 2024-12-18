#include "hatrix/gamemap.hpp"
#include "hatrix/world.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/utils/position.hpp"
#include "hatrix/utils/shadowcasting.hpp"

GamemapCell::GamemapCell(int x, int y) : blocking(false), opaque(false) {
    position.x = x;
    position.y = y;
};

GamemapCell::~GamemapCell() { };

void GamemapCell::add_entity(Entity *entity) {
    opaque |= entity->opaque;
    blocking |= entity->blocking;
    entities.push_front(entity);
};

void GamemapCell::del_entity(Entity *entity) {
    entities.remove(entity);
    update_blocking();
    update_opaque();
};

void GamemapCell::update_blocking() {
    blocking = false;
    for (Entity* entity : entities){
        blocking |= entity->blocking;
    };
};

void GamemapCell::update_opaque(){
    opaque = false;
    for (Entity* entity : entities){
        opaque |= entity->opaque;
    };
};

Entity* GamemapCell::pop_entity(Entity *entity) {
    del_entity(entity);
    return entity;
};

Gamemap::Gamemap(World *world) : world(world) {};

Gamemap::~Gamemap() {
    for (Entity *entity : enumerate_entities())
    {
        entity->~Entity();
    }
};

GamemapCell *Gamemap::get_cell(int x, int y) {
    auto it = position_bucket.find(Position{x, y});
    GamemapCell *the_cell;
    if (it == position_bucket.end()){
        the_cell = new GamemapCell(x, y);
        position_bucket.insert(std::pair(Position{x, y}, the_cell));
    }
    else
    {
        the_cell = it->second;
    }
    return the_cell;
};

void Gamemap::add_entity(Entity *entity)
{
    if(entity->type == EntityType::NORMAL){
        normal_entities.push_front(entity);
    }
    else if (entity->type == EntityType::STATIC)
    {
        static_entities.push_front(entity);
    };
    GamemapCell *the_cell = get_cell(entity->position.x, entity->position.y);
    the_cell->add_entity(entity);
};

void Gamemap::remove_entity(Entity *entity) {
    if(entity->type == EntityType::NORMAL){
        normal_entities.remove(entity);
    }
    else if (entity->type == EntityType::STATIC)
    {
        static_entities.remove(entity);
    };
    GamemapCell *the_cell = get_cell(entity->position.x, entity->position.y);
    the_cell->del_entity(entity);
};

void Gamemap::move_entity(Entity *entity, int dx, int dy) {
    Position old_position = entity->position;
    Position new_position = Position{old_position.x + dx, old_position.y + dy};

    if(is_blocking(new_position.x, new_position.y)){
        return;
    }

    entity->position = new_position;

    get_cell(old_position.x, old_position.y)->del_entity(entity);
    get_cell(new_position.x, new_position.y)->add_entity(entity);
};

const std::list<Entity *> &Gamemap::enumerate_entities() {
    return normal_entities;
};

const std::list<Entity *> &Gamemap::enumerate_entities_at(int x, int y) {
    return get_cell(x, y)->entities;
};

bool Gamemap::is_opaque(int x, int y) {
    return get_cell(x, y)->opaque;
};

bool Gamemap::is_blocking(int x, int y){
    return get_cell(x, y)->blocking;
}

void Gamemap::update_fov() {
    doupdate_fov();
};

void Gamemap::doupdate_fov(){
    visible_position.clear();
    Entity * player = world->get_player();
    Position p = player->position;
    int distance = 7;
    utils_compute_fov(
            std::pair(p.x, p.y),
            [this](int _x, int _y){
                if(_x == INFINITY || _y == INFINITY){
                    return true;
                }
                return is_opaque(_x, _y);},
            [this](int _x, int _y){visible_position.push_back(Position{_x, _y});},
            (float) distance
    );
}
