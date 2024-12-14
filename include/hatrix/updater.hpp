// 更新游戏上下文, 读写游戏上下文
#include "hatrix/world.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/utils/timer.hpp"

class Updater {
    public:
        Updater(float fps);
        bool should_update();
        void update(World *world);
        void doupdate(World *world);

        void update_entity(Entity *entity);

    private:
        float fps;
        float last_time;
        float dead_time;
        float base_tick = 1.0;
};