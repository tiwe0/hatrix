#ifndef HATRIX_ENTITIES_WALL
#define HATRIX_ENTITIES_WALL

#include "hatrix/entities/entity.hpp"

class Wall : public Entity
{
public:
    Wall();
    ~Wall() override;
    Action *make_decision() override;

private:

};

#endif