// 存储游戏数据的上下文
#ifndef HATRIX_WORLD
#define HATRIX_WORLD
#include <vector>
#include <list>
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
struct Vec2;

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

        bool open(int x, int y);
        bool close(int x, int y);

        const std::list<Entity *> &enumerate_entities();

        // controller api
        Entity *get_player();
        void set_player(Entity *player);

        // core api
        void core_eval(const char *cmd);

        // world api
        void print(const std::string message);

        Action *get_action();

        std::string get_time();

        std::string message;

        TheCore *core;
        Updater *updater;
        Gamemap *gamemap;

    private:
        Controller *controller;
        Renderer *renderer;

        float stability = 1.0;
};
#endif