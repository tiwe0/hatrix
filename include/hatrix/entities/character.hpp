#ifndef HATRIX_ENTITIES_CHARACTER
#define HATRIX_ENTITIES_CHARACTER
#include <vector>
#include <set>
#include <list>
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
        int memory = 512;
        Action *current_action = nullptr;

        void mark_as_visited(int x, int y);

        std::vector<Vec2> fov;
        std::vector<Vec2> path;
        std::set<Vec2> visited;
    private:
        std::list<Vec2> _visited; // for record order;
};

#endif
