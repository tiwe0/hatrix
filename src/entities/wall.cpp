#include "hatrix/entities/wall.hpp"

#include "hatrix/actions/idle.hpp"

Wall::Wall(){
    glyph = '#';
    opaque = true;
    blocking = true;
    type = EntityType::STATIC;
};

Wall::~Wall() {};

Action *Wall::make_decision_as_npc() {
    return new ActionIdle();
};
