#ifndef HATRIX_CONTROLLER
#define HATRIX_CONTROLLER
#include "hatrix/entities/entity.hpp"
#include "hatrix/world.hpp"
#include "hatrix/actions/null.hpp"

class Entity;
class World;
class Action;
class ActionNull;

class Controller {
    public:
        Controller();
        void set_controller(Entity *entity);
        Action *make_decision();

    private:
        Entity *the_entity;
        Action *the_action;
};
#endif