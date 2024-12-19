#include "hatrix/entities/door.hpp"
#include "hatrix/actions/action.hpp"
#include "hatrix/actions/null.hpp"
#include "hatrix/world.hpp"
#include "hatrix/gamemap.hpp"

Door::Door(bool is_open) : is_open(is_open) {
    if(is_open){

    setcchar(&glyph, L"/", A_NORMAL, 0, nullptr);
    } else {

    setcchar(&glyph, L"+", A_NORMAL, 0, nullptr);
    };
    blocking = !is_open;
    opaque = !is_open;
};

bool Door::open() {
    setcchar(&glyph, L"/", A_NORMAL, 0, nullptr);
    is_open = false;
    blocking = false;
    opaque = false;
    return true;
};

bool Door::close() {
    setcchar(&glyph, L"+", A_NORMAL, 0, nullptr);
    is_open = true;
    blocking = true;
    opaque = true;
    return true;
};

Action *Door::make_decision_as_npc() {
    return new ActionNull();
};
