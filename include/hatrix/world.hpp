// 存储游戏数据的上下文
#ifndef HATRIX_WORLD
#define HATRIX_WORLD
#include <vector>
#include <string>
#include <map>
#include <algorithm>

class Action;
class TheCore;
class Entity;
class Controller;
class Renderer;
class Updater;
class Gamemap;
struct Position;

class World
{
    public:
        World();
        ~World();

        bool should_quit;
        bool core_connected = false;

        void render();
        void update();
        int run();

        // gammap api;
        void add_entity(Entity *entity, int x, int y);
        void remove_entity(Entity *entity);
        void move_entity(Entity *entity, int dx, int dy);
        const std::vector<Entity *> &enumerate_entities();
        Position get_entity_position(std::string entity_id);


        // controller api
        Entity *get_player();
        void set_player(Entity *player);

        Action *get_action();

        std::string get_time();

        TheCore *core;
        Updater *updater;
        Gamemap *gamemap;

    private:
        Controller *controller;
        Renderer *renderer;

        float stability = 1.0;

        std::vector<Entity *> entities_vec;
        std::map<std::string, Position> entities_to_position;
        std::map<Position, std::vector<std::string>> position_to_entities;
};
#endif