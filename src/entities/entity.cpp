#include <string>
#include "hatrix/entities/entity.hpp"
#include "hatrix/utils/timer.hpp"
#include "hatrix/actions/action.hpp"
#include "hatrix/world.hpp"


Entity::Entity() : glyph('@')
{
    id = generate_timestamp_id();
};

Entity::~Entity()
{
    world->remove_entity(this);
};

void Entity::elapse(float time) {
    busy_time -= time;
    if (busy_time <= 0.0) {
        busy_time = 0.0;
    };
};

bool Entity::is_busy() {
    return busy_time > 0.0;
};

void Entity::set_busy(float _busy_time)
{
    busy_time = _busy_time;
};

bool Entity::is_under_controll() {
    return controller != nullptr;
};

Controller *Entity::get_controller()
{
    return controller;
};

void Entity::set_controller(Controller * the_controller)
{
    controller = the_controller;
};

int Entity::get_x() {
    return world->get_entity_position(id).x;
};

int Entity::get_y() {
    return world->get_entity_position(id).y;
};
