// 更新游戏上下文, 读写游戏上下文
#include <chrono>

class World;
class Entity;

// 2024年12月16日星期一晚上10点55分
#define BASETIME 1734360914

class Updater {
    public:
        Updater(World *world, float fps);
        bool should_update();
        void update();
        void doupdate();

        void update_entity(Entity *entity);

        std::time_t get_ticks();

    private:
        float fps;
        float last_time;
        float dead_time = 0.0;
        std::time_t ticks = 0;
        int base_tick = 1;

        World *world;
};