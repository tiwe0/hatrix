#ifndef HATRIX_ENTITIES_NORMAL_ENTITY
#define HATRIX_ENTITIES_NORMAL_ENTITY
#include "hatrix/entities/entity.hpp"

class Action;

class NormalEntity : public Entity{
    public:
        NormalEntity() {
            type = EntityType::NORMAL;
        };
        virtual Action *make_decision_as_npc() = 0;
};

#endif