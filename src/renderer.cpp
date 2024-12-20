#include "hatrix/renderer.hpp"
#include "hatrix/utils/predicate.hpp"
#include <bitset>
#include <vector>
#include <string>
#include "hatrix/entities/entity.hpp"
#include "hatrix/entities/player.hpp"
#include "hatrix/world.hpp"
#include "hatrix/core/the_core.hpp"
#include "hatrix/controller.hpp"
#include "hatrix/utils/timer.hpp"
#include "hatrix/updater.hpp"
#include "hatrix/actions/move.hpp"
#include "hatrix/actions/action.hpp"
#include "hatrix/actions/open.hpp"
#include "hatrix/actions/close.hpp"
#include "hatrix/gamemap.hpp"
#include "hatrix/utils/position.hpp"
#include "hatrix/entities/character.hpp"
#include "hatrix/ui/editor.hpp"

// stdscr: 36 , 130 , 0, 0
// status: 3, 130, 33, 0
// inventory: 28, 24, 3, 3
// code: 28, 89, 3, 30

Renderer::Renderer(World *world, Controller *controller) : world(world), controller(controller), fps(60.0), dead_time(0.0), last_key('\0')
{
    init();
    code_editor = new Editor(89, 28);
    code_editor->move(3, 3);
    code_editor->hide();
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
    std::srand(std::time(nullptr));
    // unicode 模式
    setlocale(LC_ALL, "");

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

    // 新建 debug window
    windows[3] = newwin(28, 89, 2, 2);
    panels[3] = new_panel(windows[3]);
    hide_panel(panels[3]);
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
    update_mouse_position();
    update_mouse_world_position();
};

void Renderer::handle_code_mode_input(int c)
{
    switch (c)
    {
    // Esc
    case 27:
        code_mode_off();
        code_editor->hide();
        if(debug_mode){
            debug_mode = false;
            world->core_eval(code_editor->get_content());
        };
        break;

    default:
        code_editor->handle_char(c);
    }
};


void Renderer::handle_play_mode_input(int c)
{
    switch (c)
    {
    // Esc
    case 27:
        world->should_quit = true;
        break;

    case 'i':
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

    case '\t':
        show_debug = !show_debug;
        if (show_debug)
        {
            show_panel(panels[3]);
        }
        else
        {
            hide_panel(panels[3]);
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

    case 'o':
        controller->set_action(new ActionOpen());
        break;

    case 'c':
        controller->set_action(new ActionClose());
        break;

    case '?':
        show_code = true;
        debug_mode = true;
        code_mode_on();
        code_editor->show();
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
    werase(windows[0]);
    werase(windows[1]);
    werase(windows[3]);

    render_world();
    render_ui();

    update_panels();
    doupdate();
};

void Renderer::render_ground()
{
    world->gamemap->update_fov();
    for (Vec2 p : world->get_player()->fov)
    {
        int rx = compute_render_x(p.x);
        int ry = compute_render_y(p.y);
        mvaddch(ry, rx, '.');
    };
};

void Renderer::render_world()
{
    Character *player = world->get_player();
    Vec2 player_position = player->position;
    target_x = player_position.x;
    target_y = player_position.y;

    render_ground();

    for (Vec2 p: world->get_player()->fov){
        Entity *entity = world->gamemap->get_render_entity_at(p.x, p.y);
        if(entity != nullptr){
            render_entity(entity);
        };
    };

// DEBUG
#ifdef HATRIX_DEBUG
if(mouse_moved){
    mouse_moved = false;
    player->path = world->gamemap->get_path(player_position.x, player_position.y, mouse_world_x, mouse_world_y);
};
cchar_t c;
setcchar(&c, L"*", A_NORMAL, 0, nullptr);
std::vector<Vec2> &path = player->path;
int i = 0;
for (Vec2 &v : path)
{
    if(i == path.size()-1){
        continue;
    }
    i++;
    mvadd_wch(compute_render_y(v.y), compute_render_x(v.x), &c);
};
#endif
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

void Renderer::update_mouse_world_position() {
    int _offset_x = mouse_x - window_width / 2;
    int _offset_y = mouse_y - window_height / 2;

    mouse_world_x = target_x + _offset_x;
    mouse_world_y = target_y + _offset_y;
};

void Renderer::render_entity(Entity *entity)
{
    cchar_t* glyph = &(entity->glyph);
    int x = entity->position.x;
    int y = entity->position.y;
    if (in_viewver(y, x))
    {
        int rx = compute_render_x(x);
        int ry = compute_render_y(y);
        mvadd_wch(ry, rx, glyph);
    };
};

static const char *status_format = "time: %s\thealth: %d%%\thunger: %d%%\tmoney: %d$\tstablity: %d%%";

void Renderer::render_status_panel()
{
    Character *player = world->get_player();
    wborder(windows[0], ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);
    mvwprintw(windows[0], 0, 63, "Status");
    mvwprintw(windows[0], 1, 10, status_format, world->get_time().c_str(), player->health, player->hunger, player->money, world->get_stability());
};

void Renderer::render_inventory_panel()
{
    if (!show_inventory)
        return;
    box(windows[1], 0, 0);
    mvwprintw(windows[1], 0, 7, "Inventory");
};

void Renderer::render_debug_panel()
{
    if (!show_debug)
        return;
    box(windows[3], 0, 0);
    int i = 0;
    mvwprintw(windows[3], 0, 0, "Debug");
    mvwprintw(windows[3], ++i, 1, "window size: (%d, %d)", window_width, window_height);
    mvwprintw(windows[3], ++i, 1, "player action: [%s]", std::string(*controller->get_action()).c_str());
    mvwprintw(windows[3], ++i, 1, "player position: (%d, %d)", world->get_player()->position.x, world->get_player()->position.y);
    mvwprintw(windows[3], ++i, 1, "mouse position: (%d, %d)", mouse_x, mouse_y);
    mvwprintw(windows[3], ++i, 1, "mouse last position: (%d, %d)", last_mouse_x, last_mouse_y);
    mvwprintw(windows[3], ++i, 1, "mouse world position: (%d, %d)", mouse_world_x, mouse_world_y);
    mvwprintw(windows[3], ++i, 1, "last error: %s", world->core->last_eval_error.c_str());
    mvwprintw(windows[3], ++i, 1, "last key pressed: %c", last_key);
    mvwprintw(windows[3], ++i, 1, "code: %s", code_editor->get_content());
    mvwprintw(windows[3], ++i, 1, world->message.c_str());
    // Entity *the_entity = world->gamemap->get_first_entity_at_which(mouse_world_x, mouse_world_y, is_wall);
    // if (the_entity != nullptr){
    //     Wall *wall = (Wall *)the_entity;
    //     int mask = wall->mask;
    //     mvwprintw(windows[3], ++i, 1, "wall mask: %s", std::bitset<4>(mask).to_string().c_str());
    // };
    mvwprintw(windows[3], ++i, 1, "blocking: %s", world->gamemap->is_blocking(mouse_world_x, mouse_world_y)? "yes": "no");
    mvwprintw(windows[3], ++i, 1, "opaque: %s", world->gamemap->is_opaque(mouse_world_x, mouse_world_y)? "yes": "no");
    std::stringstream os;
    for(Vec2& v : world->get_player()->path){
        os << v.to_string() << "<-";
    }
    mvwprintw(windows[3], ++i, 1, "path: %s", os.str().c_str());
}

void Renderer::render_code_panel()
{
    if (!show_code)
        return;
    code_editor->render();
};

void Renderer::render_ui()
{
    getmaxyx(stdscr, window_height, window_width);

    box(stdscr, 0, 0);

    mvprintw(0, 0, "(%d, %d)", target_x, target_y);

    render_status_panel();
    render_inventory_panel();
    render_code_panel();
    render_debug_panel();
};

void Renderer::code_mode_on()
{
    code_mode = true;
};

void Renderer::code_mode_off()
{
    code_mode = false;
};

void Renderer::update_mouse_position(){
    mouse_moved = true;

    last_mouse_x = mouse_x;
    last_mouse_y = mouse_y;

    mouse_x = event.x;
    mouse_y = event.y;
    mouse_action = event.bstate;
};
