#include "hatrix/controller.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/world.hpp"
#include "hatrix/actions/null.hpp"
#include "hatrix/entities/character.hpp"

Controller::Controller() : the_entity(nullptr), the_action(nullptr) {};

Action* Controller::make_decision(){
    if (the_action == nullptr){
        return new ActionNull();
    } else {
        Action *action = the_action;
        the_action = nullptr;
        return action;
    };
};

void Controller::set_player(Character *player)
{
    if(the_entity != nullptr){
        the_entity->controller = nullptr;
    }
    player->controller = this;
    the_entity = player;
};

Character *Controller::get_player()
{
    return the_entity;
};

void Controller::set_action(Action* action)
{
    the_action = action;
};

Action *Controller::get_action()
{
    if (the_action)
    {
        return the_action;
    }
    else
    {
        return new ActionNull();
    };
};
