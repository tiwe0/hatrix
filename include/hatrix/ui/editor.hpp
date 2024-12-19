#ifndef HATRIX_UI_EDITOR
#define HATRIX_UI_EDITOR

#include <string>
#include <ncurses.h>
#include <panel.h>

#define BUFFER_SIZE 1024*1024

class Editor
{
public:
    Editor(int width, int height, std::string name="Code");
    ~Editor();

    char *buffer;
    WINDOW *window;
    PANEL *panel;

    int window_x, window_y;
    int curse_pos=0, end_pos=0;
    int code_width, code_height;
    int width, height;

    int render_row, render_col;

    // buffer
    void insert_char(const char c);
    void delete_char();

    void move_cur_left();
    void move_cur_right();
    void move_cur_up();
    void move_cur_down();

    void handle_char(char c);

    // panel
    void hide();
    void show();
    void move(int row, int col);

    void render();
    void erase();

    char *get_content();

private:
    std::string name;
    void reset_render_rc();
    void update_render_rc(char c);
};

#endif