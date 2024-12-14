#ifndef HATRIX_ACTION_NULL
#define HATRIX_ACTION_NULL
#include "hatrix/actions/action.hpp"

class ActionNull: public Action{
    public:
        float perform(Entity *entity) override;
};

#endif