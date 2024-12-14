#include "hatrix/controller.hpp"

Controller::Controller() : the_entity(nullptr), the_action(nullptr) {};

Action* Controller::make_decision(){
    if (the_action == nullptr){
        return new ActionNull();
    } else {
        Action *action = the_action;
        the_action = nullptr;
        return action;
    }
}

void Controller::set_controller(Entity * entity)
{
    the_entity->set_controller(nullptr);
    the_entity = entity;
    the_entity->set_controller(this);
}

