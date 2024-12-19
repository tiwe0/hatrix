#ifndef HATRIX_ENTITIES_WALL
#define HATRIX_ENTITIES_WALL

#include "hatrix/entities/static_entity.hpp"
#include <ncurses.h>
#include <map>
#include <vector>

// ACS_VLINE：垂直线（通常为 │）
// ACS_HLINE：水平线（通常为 ─）

class Wall : public StaticEntity
{
public:
    static std::map<int, std::vector<chtype>> mask_to_glyph;
    Wall();
    Action *make_decision_as_npc() override;
    void update_glyph();
    int mask = 0b0000; // top left down right

private:
    int compute_mask();
};

#endif