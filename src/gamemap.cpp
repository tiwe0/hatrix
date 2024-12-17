#include "hatrix/gamemap.hpp"
#include "hatrix/world.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/utils/position.hpp"
#include "hatrix/utils/shadowcasting.hpp"

// TODO 后续要优化

Gamemap::Gamemap(World *world) : world(world) {};

Gamemap::~Gamemap() {
    for (Entity *entity : enumerate_entities())
    {
        entity->~Entity();
    }
};

void Gamemap::add_entity(Entity *entity, int x, int y)
{
    entity->world = this->world;

    entity_id_to_entity.insert(std::pair<std::string, Entity *>(entity->id, entity));

    Position position{x, y};

    entities_vec.push_back(entity);
    entities_to_position.insert(std::pair<std::string, Position>(entity->id, position));

    position_to_entities[position].push_back(entity->id);
};

void Gamemap::remove_entity(Entity *entity) {
    Position position = entities_to_position.at(entity->id);
    entities_to_position.erase(entity->id);
    entity_id_to_entity.erase(entity->id);

    auto it = std::find(position_to_entities[position].begin(), position_to_entities[position].end(), entity->id);
    if(it != position_to_entities[position].end())
    {
        position_to_entities[position].erase(it);
    }

    auto _it = std::find(entities_vec.begin(), entities_vec.end(), entity);
    if (_it != entities_vec.end()){
        entities_vec.erase(_it);
    }
};

void Gamemap::move_entity(Entity *entity, int dx, int dy) {
    Position old_position = get_entity_position(entity->id);

    Position new_position = Position{old_position.x + dx, old_position.y + dy};
    entities_to_position[entity->id] = new_position;

    std::vector<std::string> &position_vec = position_to_entities[old_position];
    auto it = std::find(position_vec.begin(), position_vec.end(), entity->id);
    if (it != position_vec.end())
    {
        position_vec.erase(it);
    }
    position_to_entities[new_position].push_back(entity->id);
};

Position Gamemap::get_entity_position(std::string entity_id){
    return entities_to_position.at(entity_id);
};

const std::vector<Entity *>Gamemap::enumerate_entities_at(int x, int y) {
    std::vector<Entity *> entities;
    for (std::string id : position_to_entities[Position{x, y}]){
        entities.push_back(entity_id_to_entity[id]);
    };
    return entities;
};

const std::vector<Entity *> &Gamemap::enumerate_entities(){
    return entities_vec;
};

// TODO 修改为圆形
void Gamemap::update_visiable()
{
    visiable_positions.clear();

    Entity *player = world->get_player();
    Position position = world->get_entity_position(player->id);

    int x = position.x;
    int y = position.y;
    float distance = 10.0;

    std::function<void(int, int)> _mark_as_visiable = [this](int tx, int ty)
    {
        visiable_positions.push_back(Position{tx, ty});
    };

    std::function<bool(int, int)> _is_opaque = [this](int tx, int ty) -> bool
    {
        return is_opaque(tx, ty);
    };

    compute_fov(std::make_pair(x, y), _is_opaque, _mark_as_visiable, distance);
}

// TODO 优化
bool Gamemap::is_opaque(int x, int y){
    for (Entity *entity : enumerate_entities_at(x, y)){
        if(entity->opaque){
            return true;
        };
    };
    return false;
}

bool Gamemap::is_transparent(int x, int y){
    return !is_opaque(x, y);
}