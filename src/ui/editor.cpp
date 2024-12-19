#include "hatrix/ui/editor.hpp"

Editor::Editor(int width, int height, std::string name) : width(width), height(height), name(name), code_width(width - 2), code_height(height - 2)
{
    buffer = new char[BUFFER_SIZE];
    window = newwin(height, width, window_y, window_x);
    keypad(window, true);
    panel = new_panel(window);
    reset_render_rc();
};

Editor::~Editor()
{
    delete buffer;
}

void Editor::erase()
{
    werase(window);
}

void Editor::insert_char(const char c)
{
    if (end_pos == BUFFER_SIZE)
    {
        return;
    }
    for (int p = end_pos; p > curse_pos; p--)
    {
        buffer[p] = buffer[p - 1];
    };
    buffer[curse_pos] = c;
    curse_pos++;
    end_pos++;
};

void Editor::delete_char()
{
    if (curse_pos <= 0)
    {
        return;
    }

    buffer[curse_pos - 1] = 0;
    for (int p = curse_pos; p < end_pos; p++)
    {
        buffer[p] = buffer[p + 1];
    };
    end_pos--;
    curse_pos--;
};

void Editor::handle_char(char c)
{
    switch (c)
    {
    // // 左方向键
    // case KEY_LEFT:
    //     move_cur_left();
    //     break;

    // // 右方向键
    // case KEY_RIGHT:
    //     move_cur_right();
    //     break;

    // // 上方向键
    // case KEY_UP:
    //     move_cur_up();
    //     break;

    // // 下方向键
    // case KEY_DOWN:
    //     move_cur_down();
    //     break;

    // 删除
    case 127:
        delete_char();
        break;

    default:
        insert_char(c);
    }
};

void Editor::hide()
{
    hide_panel(panel);
};

void Editor::show()
{
    show_panel(panel);
};

void Editor::render()
{
    erase();

    box(window, 0, 0);
    mvwprintw(window, 0, width / 2, "%s", name.c_str());

    reset_render_rc();
    for (char *p = buffer; ((int)(p - buffer) < BUFFER_SIZE && (*p != '\0')); p++)
    {
        char c = *p;
        if (' ' <= c && c <= '~')
        {
            mvwaddch(window, render_row, render_col, c);
        }
        update_render_rc(c);
    };

    // render cursor
    int cur_row = curse_pos / code_width + 1;
    int cur_col = curse_pos % code_width + 1;
    mvwaddch(window, cur_row, cur_col, '_');
};

void Editor::move_cur_left()
{
    if (curse_pos <= 0)
    {
        return;
    };
    curse_pos -= 1;
};

void Editor::move_cur_right()
{
    if (curse_pos >= end_pos)
    {
        return;
    }
    curse_pos += 1;
};

void Editor::move_cur_up()
{
    if (curse_pos - code_width < 0)
    {
        return;
    }
    curse_pos -= code_width;
};

void Editor::move_cur_down()
{
    if (curse_pos + code_width > end_pos)
    {
        return;
    }
    curse_pos += code_width;
};

char *Editor::get_content()
{
    return buffer;
};

void Editor::move(int row, int col)
{
    move_panel(panel, row, col);
};

void Editor::reset_render_rc()
{
    render_row = 1;
    render_col = 1;
};

void Editor::update_render_rc(char c)
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
    if (c == '\t')
    {
        render_col += 4;
        if (render_col - 1 >= code_width)
        {
            render_col = 1;
            render_row += 1;
        };
        return;
    };
    if (c == '\n')
    {
        render_col = 1;
        render_row += 1;
        return;
    };
};