#include "hatrix/renderer.hpp"

Renderer::Renderer(World *world, Controller* controller) : world(world), controller(controller), fps(60.0), dead_time(0.0), last_key('\0')
{
    init();
};

Renderer::~Renderer()
{
    end();
};

void handle_resize(int signal) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    resize_term(w.ws_row, w.ws_col);
    clear();
};

void Renderer::init()
{
    // 初始化 curses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    
    // 处理 SIGWINCH 信号
    signal(SIGWINCH, handle_resize);
    getmaxyx(stdscr, window_height, window_width);

    // 色彩模式
    if (has_colors() == false)
    {
        world->should_quit = true;
    }
    start_color();

    keypad(stdscr, TRUE); // keypad模式
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL); // 鼠标检测
    nodelay(stdscr, TRUE); // 非阻塞模式

    box(stdscr, 0, 0);

    // 启用鼠标移动报告
    printf("\033[?1003h\n");
    refresh();
};

void Renderer::end()
{
    // 禁用鼠标移动报告
    printf("\033[?1003l\n");
    clear();
    endwin();
};

void Renderer::render()
{
    last_time = get_current_time();
    while (!world->should_quit)
    {
        handle_input();
        if (shold_render())
        {
            dorender();
        }
        usleep(10000); // 防止cpu空转过高
    }
};

void Renderer::handle_input() {
    int ch = getch();
    switch (ch)
    {
    // 处理鼠标输入
    case KEY_MOUSE:
        if (getmouse(&event) == OK)
        {
            handle_mouse_input();
        }
        break;

    // 无输入
    case -1:
        break;

    // 处理键盘输入
    default:
        handle_keyboard_input(ch);
        break;
    }
};

void Renderer::handle_mouse_input() {
    mouse_x = event.x;
    mouse_y = event.y;
    mouse_action = event.bstate;
};

void Renderer::handle_keyboard_input(int c) {
    last_key = c;
    switch (c)
    {
    case 'q':
        world->should_quit = true;
        break;

    default:
        break;
    }
};

bool Renderer::shold_render()
{
    if (fps <= 0.0)
    {
        return true;
    }
    float current_time = get_current_time();
    dead_time += (current_time - last_time);
    last_time = current_time;

    if (dead_time >= 1.0 / fps)
    {
        dead_time = 0;
        return true;
    }
    return false;
};

void Renderer::dorender() {
    erase();
    render_world();
    render_ui();
    refresh();
};

void Renderer::render_world() {
    for (Entity* entity: world->enumerate_entities()){
        render_entity(entity);
    }
};

void Renderer::render_entity(Entity *entity) {
    char glyph = entity->glyph;
    int x = entity->get_x();
    int y = entity->get_y();
    mvaddch(y, x, glyph);
};

void Renderer::render_ui() {
    getmaxyx(stdscr, window_height, window_width);

    box(stdscr, 0, 0);

    mvprintw(window_height-4, 1, "Last key pressed: %c", last_key);
    mvprintw(window_height-3, 1, "Mouse at (%d, %d)", mouse_x, mouse_y);
    mvprintw(window_height-2, 1, "Window size (%d, %d)", window_width, window_height);
};