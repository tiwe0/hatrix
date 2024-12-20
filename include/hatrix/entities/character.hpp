#ifndef HATRIX_ENTITIES_CHARACTER
#define HATRIX_ENTITIES_CHARACTER
#include <vector>
#include "hatrix/entities/normal_entity.hpp"
#include "hatrix/utils/position.hpp"

class Character: public NormalEntity {
    public:
        Character(){};

        virtual Action *make_decision_as_npc() = 0;

        int health = 100;
        int max_health = 100;
        int hunger = 100;
        int max_hunger = 100;
        int money = 0;
        int vision = 10;
        Action *current_action = nullptr;

        std::vector<Vec2> fov;
        std::vector<Vec2> path;
};

#endif
