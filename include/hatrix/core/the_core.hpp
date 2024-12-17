#ifndef HATRIX_CORE_THE_CORE
#define HATRIX_CORE_THE_CORE
#include <lua.hpp>

class World;
class Wall;

class TheCore{
    public:
        TheCore(World *world);
        ~TheCore();

        void eval(const char *script);

    private:
        lua_State *L;
        World *world;
};
#endif