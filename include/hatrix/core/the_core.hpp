#ifndef HATRIX_CORE_THE_CORE
#define HATRIX_CORE_THE_CORE
#include <lua.hpp>
#include <string>

class World;
class Wall;

class TheCore{
    public:
        TheCore(World *world);
        ~TheCore();

        void eval(const char *script);
        bool last_eval_success;

        std::string last_eval_error;

        int stability = 100;

    private:
        lua_State *L;
        World *world;

        void registe_entity_class();
        void registe_world_instance();
};
#endif