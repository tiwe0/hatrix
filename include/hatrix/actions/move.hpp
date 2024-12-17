#ifndef HATRIX_ACTIONS_MOVE
#define HATRIX_ACTIONS_MOVE
#include "hatrix/actions/action.hpp"

class ActionMove: public Action{
    public:
        ActionMove(int dx, int dy);
        float perform(Entity *entity) override;
        operator std::string() const override;
    private:
        int dx;
        int dy;
};

#endif