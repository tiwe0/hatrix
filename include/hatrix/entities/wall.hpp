#ifndef HATRIX_ENTITIES_WALL
#define HATRIX_ENTITIES_WALL

#include "hatrix/entities/static_entity.hpp"

class Wall : public StaticEntity
{
public:
    Wall();
    Action *make_decision_as_npc() override;

private:

};

#endif