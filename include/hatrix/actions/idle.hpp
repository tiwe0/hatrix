#ifndef HATRIX_ACTIONS_IDLE
#define HATRIX_ACTIONS_IDLE
#include "hatrix/actions/action.hpp"

class ActionIdle: public Action{
    public:
        float perform(Entity *entity) override;
};

#endif