#include "hatrix/core/the_core.hpp"
#include "hatrix/entities/wall.hpp"
#include "hatrix/world.hpp"
#include <luabridge3/LuaBridge/LuaBridge.h>

TheCore::TheCore(World *world) : world(world)
{
    L = luaL_newstate();
    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L)
        .beginClass<Wall>("Wall")
        .endClass();

    luabridge::getGlobalNamespace(L)
        .beginClass<World>("World")
        .addProperty("core_connected", &World::core_connected)
        .addFunction("add_entity", &World::add_entity)
        .addFunction("remove_entity", &World::remove_entity)
        .endClass();

    luabridge::setGlobal(L, world, "world");
};

void TheCore::eval(const char *script) {
    luaL_dostring(L, script);
};

TheCore::~TheCore() {
    lua_close(L);
};