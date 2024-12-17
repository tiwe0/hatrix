#ifndef HATRIX_ENTITY
#define HATRIX_ENTITY
#include <string>

class Controller;
class World;
class Action;

enum EntityType
{
    STATIC,
    NORMAL
};

class Entity
{
    public:
        Entity();
        virtual ~Entity();

        void elapse(float time);

        bool is_busy();
        void set_busy(float busy_time);

        bool is_under_controll();

        virtual Action *make_decision() = 0;

        char glyph;
        std::string id;
        World *world;

        int get_x();
        int get_y();

        void set_controller(Controller *controller);
        Controller *get_controller();

        EntityType type;
        bool blocking;
        bool opaque;

    private:
        Controller *controller = nullptr;
        float busy_time = 0;
};
#endif