#include "hatrix/entities/door.hpp"
#include "hatrix/actions/action.hpp"
#include "hatrix/actions/null.hpp"
#include "hatrix/world.hpp"
#include "hatrix/gamemap.hpp"

Door::Door(bool is_open) : is_open(is_open) {
    if(is_open){
        glyph = '/';
    } else {
        glyph = '+';
    };
    blocking = !is_open;
    opaque = !is_open;
};

bool Door::open() {
    glyph = '/';
    is_open = false;
    blocking = false;
    opaque = false;
    return true;
};

bool Door::close() {
    glyph = '+';
    is_open = true;
    blocking = true;
    opaque = true;
    return true;
};

Action *Door::make_decision_as_npc() {
    return new ActionNull();
};
