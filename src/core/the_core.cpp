#include "hatrix/core/the_core.hpp"

#include "hatrix/world.hpp"

#include "hatrix/entities/entity.hpp"
#include "hatrix/entities/static_entity.hpp"
#include "hatrix/entities/normal_entity.hpp"
#include "hatrix/entities/wall.hpp"
#include "hatrix/entities/door.hpp"
#include "hatrix/entities/character.hpp"
#include "hatrix/entities/player.hpp"
#include <luabridge3/LuaBridge/LuaBridge.h>

TheCore::TheCore(World *world) : world(world)
{
    L = luaL_newstate();
    luaL_openlibs(L);

    registe_entity_class();
    registe_world_instance();
};

void TheCore::registe_entity_class() {
    luabridge::getGlobalNamespace(L)
        .beginClass<Entity>("Entity")
        .endClass()
        .deriveClass<StaticEntity, Entity>("StaticEntity")
        .endClass()
        .deriveClass<NormalEntity, Entity>("NormalEntity")
        .endClass()
        .deriveClass<Wall, StaticEntity>("Wall")
        .addConstructor<void()>()
        .endClass()
        .deriveClass<Door, StaticEntity>("Door")
        .addConstructor<void()>()
        .endClass()
        .deriveClass<Character, NormalEntity>("Character")
        .endClass()
        .deriveClass<Player, NormalEntity>("Player")
        .addConstructor<void()>()
        .endClass() ;
};

void TheCore::registe_world_instance() {
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
