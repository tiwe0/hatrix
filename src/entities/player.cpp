#include "hatrix/entities/player.hpp"
#include "hatrix/actions/action.hpp"
#include "hatrix/actions/null.hpp"

Player::Player() {
    glyph = '@';
};

Action *Player::make_decision()
{
    if (current_action != nullptr)
    {
        Action *tmp = current_action;
        current_action = nullptr;
        return tmp;
    }
    return new ActionNull();
};