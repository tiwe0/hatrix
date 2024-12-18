#ifndef HATRIX_ENTITY
#define HATRIX_ENTITY
#include <string>
#include "hatrix/utils/position.hpp"

class Controller;
class World;
class Action;
class Vec2;

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

        Action *make_decision();
        Action *make_decision_as_player();
        virtual Action *make_decision_as_npc() = 0;

        char glyph;
        std::string id;
        bool blocking;
        bool opaque;
        EntityType type;
        Vec2 position;
        Vec2 direction{0, 0};
        World *world;
        Controller *controller = nullptr;
        float busy_time = 0;

};
#endif