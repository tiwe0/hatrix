#include "hatrix/entities/wall.hpp"

#include "hatrix/actions/idle.hpp"

Wall::Wall(){
    glyph = '#';
};

Wall::~Wall() {};

Action *Wall::make_decision() {
    return new ActionIdle();
};
