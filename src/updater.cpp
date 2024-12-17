#include "hatrix/updater.hpp"
#include "hatrix/world.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/utils/timer.hpp"
#include "hatrix/controller.hpp"
#include "hatrix/actions/action.hpp"
#include "hatrix/gamemap.hpp"

Updater::Updater(World *world, float fps) : world(world), fps(fps) {};

void Updater::update()
{
    last_time = get_current_time();

    while (!world->should_quit)
    {
        if (should_update())
        {
            doupdate();
        }
    }
};

time_t Updater::get_ticks()
{
    return ticks + BASETIME;
}

bool Updater::should_update()
{
    if (fps <= 0.0)
    {
        ticks += base_tick;
        return true;
    }
    float current_time = get_current_time();
    dead_time += (current_time - last_time);
    last_time = current_time;

    if (dead_time >= 1.0 / fps)
    {
        dead_time = 0.0;
        ticks += base_tick;
        return true;
    }

    return false;
};

void Updater::doupdate()
{
    world->gamemap->update_visiable();
    for (Entity *entity : world->enumerate_entities())
    {
        update_entity(entity);
    };
};

void Updater::update_entity(Entity *entity)
{
    if (entity->is_busy())
    {
        entity->elapse(base_tick);
        return;
    }

    Action *action;
    // 由 Controller 做出decision;
    if (entity->is_under_controll())
    {
        action = entity->get_controller()->make_decision();
    } else {
        action = entity->make_decision();
    }
    float busy_time = action->perform(entity);
    entity->set_busy(busy_time);
    action->~Action();
};
