#ifndef HATRIX_ENTITIES_STATIC_ENTITY
#define HATRIX_ENTITIES_STATIC_ENTITY
#include "hatrix/entities/entity.hpp"

class Action;

class StaticEntity : public Entity{
    public:
        StaticEntity(){
            type = EntityType::STATIC;
        };
        virtual Action *make_decision_as_npc() = 0;
};

#endif