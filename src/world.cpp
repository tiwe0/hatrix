#include <thread>
#include <chrono>
#include <iomanip>
#include "hatrix/world.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/core/the_core.hpp"
#include "hatrix/controller.hpp"
#include "hatrix/renderer.hpp"
#include "hatrix/updater.hpp"


World::World() : should_quit(false)
{
    controller = new Controller();
    renderer = new Renderer(this, controller);
    core = new TheCore(this);
    updater = new Updater(this, 1.0);
};

void World::render()
{
    renderer->render();
};

void World::update()
{
    updater->update();
}

int World::run()
{
    std::thread thread_render([this] { render(); });
    std::thread thread_update([this] { update(); });

    thread_render.join();
    thread_update.join();

    renderer->~Renderer();
    core->~TheCore();

    return 0;
};

World::~World() {
    for(Entity* entity: enumerate_entities()){
        entity->~Entity();
    }
};

std::string World::get_time() {
    std::time_t timestamp = updater->get_ticks();

    // 转换为 tm 结构
    std::tm* timeInfo = std::localtime(&timestamp);

    // 格式化日期时间
    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    return oss.str();
};

void World::add_entity(Entity *entity, int x, int y)
{
    entity->world = this;

    Position position{x, y};

    entities_vec.push_back(entity);
    entities_to_position.insert(std::pair<std::string, Position>(entity->id, position));

    position_to_entities[position].push_back(entity->id);
};

void World::remove_entity(Entity *entity) {
    Position position = entities_to_position.at(entity->id);
    entities_to_position.erase(entity->id);

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

void World::move_entity(Entity *entity, int dx, int dy) {
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

Position World::get_entity_position(std::string entity_id){
    return entities_to_position.at(entity_id);
};

const std::vector<Entity *> &World::enumerate_entities(){
    return entities_vec;
};

Entity *World::get_player()
{
    return controller->get_player();
};

Action* World::get_action()
{
    return controller->get_action();
};

void World::set_player(Entity *player)
{
    controller->set_player(player);
};
