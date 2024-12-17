#ifndef HATRIX_ENTITIES_PLAYER
#define HATRIX_ENTITIES_PLAYER
#include "hatrix/entities/entity.hpp"

class Action;

class Player: public Entity {
    public:
        Player();
        Action *make_decision() override;

    private:
        int health = 100;
        int max_health = 100;
        int hunger = 100;
        int max_hunger = 100;
        int money = 0;

        Action *current_action = nullptr;
};

#endif
