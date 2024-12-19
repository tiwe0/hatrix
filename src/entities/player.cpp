#include "hatrix/entities/player.hpp"
#include "hatrix/actions/action.hpp"
#include "hatrix/actions/null.hpp"

Player::Player() {
    setcchar(&glyph, L"@", A_NORMAL, 0, nullptr);
};

Action *Player::make_decision_as_npc()
{
    if (current_action != nullptr)
    {
        Action *tmp = current_action;
        current_action = nullptr;
        return tmp;
    }
    return new ActionNull();
};
