#ifndef HATRIX_ENTITIES_WALL
#define HATRIX_ENTITIES_WALL

#include "hatrix/entities/entity.hpp"

class Wall : public Entity
{
public:
    Wall();
    ~Wall() override;
    Action *make_decision_as_npc() override;

private:

};

#endif