#include "hatrix/controller.hpp"
#include "hatrix/renderer.hpp"
#include "hatrix/world.hpp"
#include "hatrix/entities/wall.hpp"
#include "hatrix/entities/player.hpp"

int main()
{
    World* world_ptr = new World();

    world_ptr->add_entity(new Wall(), 0, 0);
    world_ptr->add_entity(new Wall(), 1, 0);
    world_ptr->add_entity(new Wall(), 2, 0);
    world_ptr->add_entity(new Wall(), 3, 0);
    world_ptr->add_entity(new Wall(), 3, 1);
    world_ptr->add_entity(new Wall(), 3, 2);
    world_ptr->add_entity(new Wall(), 3, 3);
    world_ptr->add_entity(new Wall(), 2, 3);
    world_ptr->add_entity(new Wall(), 1, 3);
    world_ptr->add_entity(new Wall(), 0, 3);
    world_ptr->add_entity(new Wall(), 0, 2);

    Player *player = new Player();
    world_ptr->add_entity(player, 1, 1);
    world_ptr->set_player(player);

    return world_ptr->run();
}