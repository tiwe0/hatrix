#ifndef HATRIX_CONTROLLER
#define HATRIX_CONTROLLER

class Entity;
class World;
class Action;

class Controller {
    public:
        Controller();
        void set_controller(Entity *entity);
        void set_player(Entity *player);
        Entity *get_player();
        Action *make_decision();
        void set_action(Action* action);
        Action *get_action();

    private:
        Entity *the_entity;
        Action *the_action;
};
#endif