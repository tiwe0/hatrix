#ifndef HATRIX_ENTITIES_DOOR
#define HATRIX_ENTITIES_DOOR

#include "hatrix/entities/static_entity.hpp"

class Door : public StaticEntity
{
public:
    Door(bool is_open = false);

    Action *make_decision_as_npc() override;

    bool open();
    bool close();

    bool is_open = false;
};

#endif