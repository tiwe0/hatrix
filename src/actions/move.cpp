#include "hatrix/actions/move.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/world.hpp"

ActionMove::ActionMove(int dx, int dy) : dx(dx), dy(dy) {};

float ActionMove::perform(Entity* entity) {
    entity->world->move_entity(entity, dx, dy);
    return 1.0;
};

ActionMove::operator std::string() const {
    return std::string("Move: ") + std::to_string(dx) + std::string(", ") + std::to_string(dy);
};