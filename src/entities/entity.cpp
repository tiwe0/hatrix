#include <string>
#include "hatrix/entities/entity.hpp"
#include "hatrix/controller.hpp"
#include "hatrix/utils/timer.hpp"
#include "hatrix/actions/action.hpp"
#include "hatrix/world.hpp"
#include "hatrix/utils/position.hpp"


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

Action* Entity::make_decision(){
    if(is_under_controll()){
        return make_decision_as_player();
    };
    return make_decision_as_npc();
}

Action* Entity::make_decision_as_player(){
    return controller->make_decision();
}
