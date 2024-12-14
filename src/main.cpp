#include "hatrix/controller.hpp"
#include "hatrix/renderer.hpp"
#include "hatrix/world.hpp"

int main()
{
    World* world_ptr = new World();
    Controller *controller_ptr = new Controller();

    Renderer renderer(world_ptr, controller_ptr);
    renderer.render();
}