#include "hatrix/actions/idle.hpp"

float ActionIdle::perform(Entity *entity) {
    return 1.0;
};

ActionIdle::operator std::string() const {
    return std::string("Idle");
};
