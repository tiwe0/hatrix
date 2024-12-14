// 渲染游戏, 只读游戏数据, 处理外部 io
#ifndef HATRIX_RENDERER
#define HATRIX_RENDERER

#include <unistd.h> // 用于 usleep
#include <ncurses.h>
#include <stdio.h>
#include <chrono> // 用于获取时间
#include <sys/ioctl.h> // 获取终端大小
#include <unistd.h>
#include "hatrix/world.hpp"
#include "hatrix/controller.hpp"
#include "hatrix/utils/timer.hpp"

class Renderer
{
public:
    Renderer(World *world, Controller *conrtoller);
    ~Renderer();
    void render();

private:

    void init();
    void end();

    bool shold_render();

    void handle_input();
    void handle_mouse_input();
    void handle_keyboard_input(int c);

    void dorender();

    void render_world();
    void render_entity(Entity *entity);
    void render_ui();

    World *world;
    Controller *controller;

    float fps;
    float last_time;
    float dead_time;

    MEVENT event;

    int mouse_x;
    int mouse_y;
    int mouse_action;

    int window_width;
    int window_height;

    int last_key;
};
#endif