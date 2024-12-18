#include "hatrix/actions/close.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/world.hpp"

ActionClose::ActionClose(int dx, int dy) : dx(dx), dy(dy) {};

float ActionClose::perform(Entity *entity) {
    if(dx == 0 && dy == 0){
        dx = entity->direction.x;
        dy = entity->direction.y;
    };
    int target_x = dx + entity->position.x;
    int target_y = dy + entity->position.y;
    entity->world->close(target_x, target_y);
    return 1.0f;
};

ActionClose::operator std::string() const {
    return std::string("Close");
};