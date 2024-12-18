#include "hatrix/actions/open.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/world.hpp"

ActionOpen::ActionOpen(int dx, int dy) : dx(dx), dy(dy) {};

float ActionOpen::perform(Entity *entity) {
    if(dx == 0 && dy == 0){
        dx = entity->direction.x;
        dy = entity->direction.y;
    };
    int target_x = dx + entity->position.x;
    int target_y = dy + entity->position.y;
    entity->world->open(target_x, target_y);
    return 1.0f;
};

ActionOpen::operator std::string() const {
    return std::string("Open");
};