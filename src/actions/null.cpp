#include "hatrix/actions/null.hpp"

float ActionNull::perform(Entity *entity) {
    return 0.0;
};

ActionNull::operator std::string() const {
    return std::string("Null");
};
