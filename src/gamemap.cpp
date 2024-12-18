#include <typeinfo>
#include "hatrix/gamemap.hpp"
#include "hatrix/world.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/utils/position.hpp"
#include "hatrix/utils/shadowcasting.hpp"
#include "hatrix/entities/player.hpp"
#include "hatrix/entities/door.hpp"
#include "hatrix/entities/wall.hpp"

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

Gamemap::Gamemap(World *world, int width, int height) : world(world), width(width), height(height) {};

Gamemap::~Gamemap() {
    for (Entity *entity : enumerate_entities())
    {
        entity->~Entity();
    }
};

int inline positive_mod(int a, int b) {
    return ((a % b) + b) % b;
}

GamemapCell *Gamemap::get_cell(int x, int y) {
    if(width != -1 && width !=0){
        x = positive_mod(x, width);
    };
    if (height != -1 && height != 0)
    {
        y = positive_mod(y, height);
    };
    auto it = position_bucket.find(Vec2{x, y});
    GamemapCell *the_cell;
    if (it == position_bucket.end()){
        the_cell = new GamemapCell(x, y);
        position_bucket.insert(std::pair(Vec2{x, y}, the_cell));
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
    entity->direction = Vec2{dx, dy};

    Vec2 old_position = entity->position;
    Vec2 new_position = Vec2{old_position.x + dx, old_position.y + dy};

    if(is_blocking(new_position.x, new_position.y)){
        return;
    }

    entity->position = new_position;

    get_cell(old_position.x, old_position.y)->del_entity(entity);
    get_cell(new_position.x, new_position.y)->add_entity(entity);
};

bool Gamemap::open(int x, int y) {
    Entity *the_entity = get_first_entity_at_which(x, y, [](Entity *entity)
                                                         { return typeid(*entity) == typeid(Door); });
    Door *the_door = (Door *)the_entity;
    if (the_door == nullptr){
        return false;
    }else {
        the_door->open();
        get_cell(x, y)->update_blocking();
        get_cell(x, y)->update_opaque();
        return true;
    }
};

bool Gamemap::close(int x, int y) {
    Entity *the_entity = get_first_entity_at_which(x, y, [](Entity *entity)
                                                         { return typeid(*entity) == typeid(Door); });
    Door *the_door = (Door *)the_entity;
    if (the_door == nullptr){
        return false;
    }else {
        the_door->close();
        get_cell(x, y)->update_blocking();
        get_cell(x, y)->update_opaque();
        return true;
    }
};

const std::list<Entity *> &Gamemap::enumerate_entities() {
    return normal_entities;
};

const std::list<Entity *> &Gamemap::enumerate_entities_at(int x, int y) {
    return get_cell(x, y)->entities;
};

Entity *Gamemap::get_first_entity_at_which(int x, int y, std::function<bool(Entity *)> cond) {
    for(Entity* entity: enumerate_entities_at(x, y)){
        if(cond(entity)){
            return entity;
        };
    };
    return nullptr;
};

Entity *Gamemap::get_render_entity_at(int x, int y) {
    Entity *entity = get_first_entity_at_which(x, y, [](Entity *the_entity)
                                               { return typeid(*the_entity) == typeid(Character); });
    if(entity == nullptr ){
        if(enumerate_entities_at(x, y).size() == 0){
            return nullptr;
        };
        return enumerate_entities_at(x, y).front();
    };
    return entity;
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

void Gamemap::update_fov(Character *character){
    character->fov.clear();
    utils_compute_fov(
            std::pair(character->position.x, character->position.y),
            [this](int _x, int _y){return is_opaque(_x, _y);},
            [character](int _x, int _y){character->fov.push_back(Vec2{_x, _y});},
            (float) character->vision
    );
};

void Gamemap::doupdate_fov(){
    Character *player = world->get_player();
    update_fov(player);
}

bool Gamemap::has_entity_at_which(int x, int y, std::function<bool(Entity *)> cond) {
    return get_first_entity_at_which(x, y, cond) != nullptr;
};

void Gamemap::update_wall_glyph_in_range(int x, int y){
    update_wall_glyph(x, y);
    update_wall_glyph(x+1, y);
    update_wall_glyph(x, y+1);
    update_wall_glyph(x-1, y);
    update_wall_glyph(x, y-1);
};

void Gamemap::update_wall_glyph(int x, int y) {
    Entity *entity = get_first_entity_at_which(x, y, [](Entity *entity)
                                               { return typeid(*entity) == typeid(Wall); });
    if(entity == nullptr){
        return;
    };
    Wall *wall = (Wall *)entity;
    wall->update_glyph();
};