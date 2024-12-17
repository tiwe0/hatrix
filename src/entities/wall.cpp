#include "hatrix/entities/wall.hpp"

#include "hatrix/actions/idle.hpp"

Wall::Wall(){
    glyph = '#';
    opaque = true;
    blocking = true;
};

Wall::~Wall() {};

Action *Wall::make_decision() {
    return new ActionIdle();
};
