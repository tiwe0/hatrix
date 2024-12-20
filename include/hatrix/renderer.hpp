// 渲染游戏, 只读游戏数据, 处理外部 io
#ifndef HATRIX_RENDERER
#define HATRIX_RENDERER

#include <wchar.h>
#include <unistd.h> // 用于 usleep
#include <stdio.h>
#include <chrono> // 用于获取时间
#include <sys/ioctl.h> // 获取终端大小
#include <unistd.h>
#include <panel.h>
#include <ncurses.h>
#include <sstream>

class World;
class Controller;
class Entity;
class Character;
class Editor;

class Renderer
{
public:
    Renderer(World *world, Controller *conrtoller);
    ~Renderer();
    void render();

private:

    void init();
    void hinit_pair();
    void hinit_color();
    void end();

    bool shold_render();

    void update_mouse_position();

    void handle_input();
    void handle_mouse_input();
    void handle_keyboard_input(int c);
    void handle_code_mode_input(int c);
    void handle_play_mode_input(int c);

    void dorender();

    void render_world();
    void render_entity(Entity *entity);

    // Character render
    void render_memory(Character *character);
    void render_fov(Character *character);
    void render_path(Entity *entity);

    // Player
    void render_player();

    void render_ui();
    void render_status_panel();
    void render_inventory_panel();
    void render_code_panel();
    void render_debug_panel();

    void code_mode_on();
    void code_mode_off();

    World *world;
    Controller *controller;

    WINDOW *windows[4];
    PANEL *panels[4];

    // 代码编辑器
    Editor* code_editor;

    // 渲染器状态
    bool show_debug = false;
    bool show_inventory = false;
    bool show_code = false;
    bool code_mode = false;
    bool debug_mode = false;
    bool mouse_moved = true;

    // render target
    int target_x = 0;
    int target_y = 0;

    // render offset
    int offset_x = 0;
    int offset_y = 0;

    int mouse_world_x;
    int mouse_world_y;

    bool in_viewver(int y, int x);

    int compute_render_x(int x);
    int compute_render_y(int y);

    void update_mouse_world_position();

    float fps;
    float last_time;
    float dead_time;

    MEVENT event;

    int mouse_x;
    int mouse_y;
    int mouse_action;

    int last_mouse_x;
    int last_mouse_y;

    int window_width;
    int window_height;

    int last_key;

    const int code_width = 87;
    const int code_height = 26;
};
#endif