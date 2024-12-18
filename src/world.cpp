#include <thread>
#include <chrono>
#include <iomanip>
#include <list>
#include "hatrix/world.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/core/the_core.hpp"
#include "hatrix/controller.hpp"
#include "hatrix/renderer.hpp"
#include "hatrix/updater.hpp"
#include "hatrix/gamemap.hpp"
#include "hatrix/utils/position.hpp"


World::World() : should_quit(false)
{
    gamemap = new Gamemap(this);
    controller = new Controller();
    renderer = new Renderer(this, controller);
    core = new TheCore(this);
    updater = new Updater(this, 60.0);
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
    gamemap->~Gamemap();
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
    entity->position.x = x;
    entity->position.y = y;
    gamemap->add_entity(entity);
};

void World::remove_entity(Entity *entity) {
    gamemap->remove_entity(entity);
};

void World::move_entity(Entity *entity, int dx, int dy) {
    gamemap->move_entity(entity, dx, dy);
};

bool World::open(int x, int y){
    return gamemap->open(x, y);
};

bool World::close(int x, int y){
    return gamemap->close(x, y);
}

const std::list<Entity *> &World::enumerate_entities(){
    return gamemap->enumerate_entities();
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

void World::core_eval(const char* cmd){
    core->eval(cmd);
};

void World::print(const std::string _message){
    message = _message;
};
