#include "hatrix/renderer.hpp"
#include <string>
#include "hatrix/entities/entity.hpp"
#include "hatrix/world.hpp"
#include "hatrix/core/the_core.hpp"
#include "hatrix/controller.hpp"
#include "hatrix/utils/timer.hpp"
#include "hatrix/updater.hpp"
#include "hatrix/actions/move.hpp"
#include "hatrix/actions/action.hpp"
#include "hatrix/gamemap.hpp"
#include "hatrix/utils/position.hpp"

// stdscr: 36 , 130 , 0, 0
// status: 3, 130, 33, 0
// inventory: 28, 24, 3, 3
// code: 28, 89, 3, 30

Renderer::Renderer(World *world, Controller *controller) : world(world), controller(controller), fps(60.0), dead_time(0.0), last_key('\0')
{
    init();
};

Renderer::~Renderer()
{
    end();
};

void handle_resize(int signal)
{
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
    set_escdelay(0);
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

    keypad(stdscr, TRUE);                                      // keypad模式
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL); // 鼠标检测
    nodelay(stdscr, TRUE);                                     // 非阻塞模式

    box(stdscr, 0, 0);

    // 启用鼠标移动报告
    printf("\033[?1003h\n");
    refresh();

    // 新建 status_line window
    windows[0] = newwin(3, window_width, window_height - 3, 0);
    panels[0] = new_panel(windows[0]);

    // 新建 inventory window
    windows[1] = newwin(28, 24, 3, 3);
    panels[1] = new_panel(windows[1]);
    hide_panel(panels[1]);

    // 新建 code window
    windows[2] = newwin(28, 89, 3, 30);
    panels[2] = new_panel(windows[2]);
    hide_panel(panels[2]);
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

void Renderer::handle_input()
{
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

void Renderer::handle_mouse_input()
{
    mouse_x = event.x;
    mouse_y = event.y;
    mouse_action = event.bstate;
};

void Renderer::handle_code_mode_input(int c)
{
    int position = code_y * code_width + code_x;
    switch (c)
    {
    // Esc
    case '\033':
        show_code = false;
        code_mode_off();
        hide_panel(panels[2]);
        break;

    // Del
    case '\177':
        if (position <= 0)
        {
            break;
        }
        buffer[position-1] = '\0';
        code_x -= 1;
        if (code_x <= -1){
            code_x = code_width - 1;
            code_y -= 1;
        }
        break;

    default:
        if (position >= code_width * code_height)
        {
            break;
        }
        buffer[position] = c;
        code_x += 1;
        if (code_x == code_width)
        {
            code_x = 0;
            code_y += 1;
        }
        break;
    }
};


void Renderer::handle_play_mode_input(int c)
{
    switch (c)
    {
    case 'q':
        world->should_quit = true;
        break;

    case '\t':
        show_inventory = (!show_inventory);
        if (show_inventory)
        {
            show_panel(panels[1]);
        }
        else
        {
            hide_panel(panels[1]);
        }
        break;

    case 'w':
        controller->set_action(new ActionMove(0, -1));
        break;

    case 'a':
        controller->set_action(new ActionMove(-1, 0));
        break;

    case 's':
        controller->set_action(new ActionMove(0, 1));
        break;

    case 'd':
        controller->set_action(new ActionMove(1, 0));
        break;

    case ' ':
        show_code = true;
        code_mode_on();
        show_panel(panels[2]);
        break;

    case 'o':
        world->core->eval("world.core_connected = true");
        break;

    case 'p':
        world->core->eval("world.core_connected = false");
        break;

    default:
        break;
    }
};

void Renderer::handle_keyboard_input(int c)
{
    last_key = c;
    if (code_mode)
    {
        handle_code_mode_input(c);
    }
    else
    {
        handle_play_mode_input(c);
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

void Renderer::dorender()
{
    erase();
    for (int i = 0; i < 3; i++)
    {
        werase(windows[i]);
    };

    render_world();
    render_ui();

    update_panels();
    doupdate();
};

void Renderer::render_ground()
{
    world->gamemap->update_fov();
    for (Position p : world->gamemap->visible_position)
    {
        int rx = compute_render_x(p.x);
        int ry = compute_render_y(p.y);
        mvaddch(ry, rx, '.');
    };
};

void Renderer::render_world()
{
    render_ground();
    for (Position p: world->gamemap->visible_position){
        for (Entity * entity : world->gamemap->enumerate_entities_at(p.x, p.y)){
            render_entity(entity);
        };
    };
};

bool Renderer::in_viewver(int y, int x) {
    return (
        (target_y - window_height / 2 < y) && 
        (y < target_y + window_height / 2) && 
        (target_x - window_width / 2 < x) && 
        (x < target_x + window_width / 2));
};

int Renderer::compute_render_x(int x) {
    int dx = x - target_x;
    return window_width / 2 + dx + offset_x;
};

int Renderer::compute_render_y(int y) {
    int dy = y - target_y;
    return window_height / 2 + dy + offset_y;
};

void Renderer::render_entity(Entity *entity)
{
    char glyph = entity->glyph;
    int x = entity->get_x();
    int y = entity->get_y();
    if(in_viewver(y, x)){
        int rx = compute_render_x(x);
        int ry = compute_render_y(y);
        mvaddch(ry, rx, glyph);
    };
};

static const char *status_format = "time: %s\thealth: %d%%\thunger: %d%%\tmoney: %d$\tstable: %d%%";

void Renderer::render_status_panel()
{
    wborder(windows[0], ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);
    mvwprintw(windows[0], 0, 63, "Status");
    mvwprintw(windows[0], 1, 10, status_format, world->get_time().c_str(), 23, 42, 38, 88);
};

void Renderer::render_inventory_panel()
{
    if (!show_inventory)
        return;
    box(windows[1], 0, 0);
    mvwprintw(windows[1], 0, 7, "Inventory");
};

static int render_row, render_col;

void reset_render_rc(){
    render_row = 1;
    render_col = 1;
}

void update_render_rc(int code_height, int code_width, char c)
{
    if (' ' <= c && c <= '~')
    {
        render_col += 1;
        if (render_col - 1 >= code_width)
        {
            render_col = 1;
            render_row += 1;
        };
        return;
    };
    if (c == '\t'){
        render_col += 4;
        if (render_col - 1 >= code_width)
        {
            render_col = 1;
            render_row += 1;
        };
        return;
    }
    if (c == '\n'){
        render_col = 1;
        render_row += 1;
        return;
    }
}

void Renderer::render_code_panel()
{
    if (!show_code)
        return;
    box(windows[2], 0, 0);
    mvwprintw(windows[2], 0, 42, "Code");

    mvwprintw(windows[2], 0, 0, "(%d, %d)", render_row, render_col);

    reset_render_rc();

    for (char *p = buffer; ((int)(p - buffer) < code_height * code_width && (*p != '\0')); p++)
    {
        char c = *p;
        if (' '<=c && c <= '~'){
            mvwaddch(windows[2], render_row, render_col, c);
        }
        update_render_rc(code_height, code_width, c);
    }

    wmove(windows[2], render_row, render_col);
};

void Renderer::render_ui()
{
    getmaxyx(stdscr, window_height, window_width);

    box(stdscr, 0, 0);

    mvprintw(0, 0, "(%d, %d)", target_x, target_y);
    mvprintw(window_height - 5, 1, std::string(*world->get_action()).c_str());

    render_status_panel();
    render_inventory_panel();
    render_code_panel();

    mvprintw(window_height - 6, 1, "Window size (%d, %d)", window_width, window_height);
    if (world->core_connected)
    {
        mvprintw(window_height - 4, 1, "connected");
    }
    else
    {
        mvprintw(window_height - 4, 1, "disconnected");
    };
};

void Renderer::code_mode_on()
{
    code_mode = true;
    echo();
    curs_set(1);
};

void Renderer::code_mode_off()
{
    code_mode = false;
    noecho();
    curs_set(0);
};
