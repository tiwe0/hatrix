#ifndef HATRIX_CONTROLLER
#define HATRIX_CONTROLLER

class Entity;
class World;
class Action;
class Character;

class Controller {
    public:
        Controller();
        void set_player(Character *player);
        Character *get_player();
        Action *make_decision();
        void set_action(Action* action);
        Action *get_action();

    private:
        Character *the_entity;
        Action *the_action;
};
#endif