#include "hatrix/controller.hpp"
#include "hatrix/renderer.hpp"
#include "hatrix/world.hpp"
#include "hatrix/entities/wall.hpp"
#include "hatrix/entities/player.hpp"

int main()
{
    World* world_ptr = new World();

    world_ptr->add_entity(new Wall(), -2, -2);
    world_ptr->add_entity(new Wall(), -2, -1);
    world_ptr->add_entity(new Wall(), -2, 0);
    world_ptr->add_entity(new Wall(), -2, 1);
    world_ptr->add_entity(new Wall(), -2, 2);
    world_ptr->add_entity(new Wall(), 2, -2);
    world_ptr->add_entity(new Wall(), 2, -1);
    world_ptr->add_entity(new Wall(), 2, 0);
    world_ptr->add_entity(new Wall(), 2, 1);
    world_ptr->add_entity(new Wall(), 2, 2);
    world_ptr->add_entity(new Wall(), -1, -2);
    world_ptr->add_entity(new Wall(), -1, -1);
    world_ptr->add_entity(new Wall(), 0, -2);
    world_ptr->add_entity(new Wall(), 1, -2);
    world_ptr->add_entity(new Wall(), -1, 2);
    world_ptr->add_entity(new Wall(), 0, 2);

    Player *player = new Player();
    world_ptr->add_entity(player, 1, 1);
    world_ptr->set_player(player);

    return world_ptr->run();
}