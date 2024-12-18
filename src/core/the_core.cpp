#include "hatrix/core/the_core.hpp"
#include "hatrix/entities/wall.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/world.hpp"
#include <luabridge3/LuaBridge/LuaBridge.h>

TheCore::TheCore(World *world) : world(world)
{
    L = luaL_newstate();
    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L)
        .beginClass<Entity>("Entity")
        .endClass()
        .deriveClass<Wall, Entity>("Wall")
        .addConstructor<void()>()
        .endClass();

    luabridge::getGlobalNamespace(L)
        .beginClass<World>("World")
        .addProperty("core_connected", &World::core_connected)
        .addFunction("add_entity", &World::add_entity)
        .addFunction("remove_entity", &World::remove_entity)
        .addFunction("print", &World::print)
        .endClass();

    luabridge::setGlobal(L, world, "world");
};

void TheCore::eval(const char *script) {
    int status = luaL_dostring(L, script);
    if (status == LUA_OK){
        last_eval_success = true;
    } else {
        last_eval_success = false;
        const char *error_message = lua_tostring(L, -1);
        last_eval_error = std::string(error_message);
        lua_pop(L, 1);
    }
};

TheCore::~TheCore() {
    lua_close(L);
};