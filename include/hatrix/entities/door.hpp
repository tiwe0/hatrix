#ifndef HATRIX_ENTITIES_DOOR
#define HATRIX_ENTITIES_DOOR

#include "hatrix/entities/entity.hpp"

class Door : public Entity
{
public:
    Door(bool is_open = false);
    ~Door() override;
    Action *make_decision_as_npc() override;

    bool open();
    bool close();

    bool is_open = false;
};

#endif