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

class World;
class Controller;
class Entity;
class Editor;

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
    void handle_code_mode_input(int c);
    void handle_play_mode_input(int c);

    void dorender();

    void render_world();
    void render_ground();
    void render_entity(Entity *entity);

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

    // render target
    int target_x = 0;
    int target_y = 0;

    // render offset
    int offset_x = 0;
    int offset_y = 0;

    bool in_viewver(int y, int x);

    int compute_render_x(int x);
    int compute_render_y(int y);

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

    const int code_width = 87;
    const int code_height = 26;
};
#endif