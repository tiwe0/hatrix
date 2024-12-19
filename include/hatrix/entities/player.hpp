#ifndef HATRIX_ENTITIES_PLAYER
#define HATRIX_ENTITIES_PLAYER
#include "hatrix/entities/entity.hpp"
#include "hatrix/utils/position.hpp"
#include "hatrix/entities/character.hpp"

class Action;


class Player: public Character {
    public:
        Player();
        Action *make_decision_as_npc() override;
};

#endif
