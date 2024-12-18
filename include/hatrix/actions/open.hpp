#ifndef HATRIX_ACTIONS_OPEN
#define HATRIX_ACTIONS_OPEN
#include "hatrix/actions/action.hpp"

class ActionOpen: public Action{
    public:
        ActionOpen(int dx = 0, int dy = 0);
        float perform(Entity *entity) override;
        operator std::string() const override;
    private:
        int dx;
        int dy;
};

#endif