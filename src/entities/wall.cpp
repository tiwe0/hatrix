#include "hatrix/entities/wall.hpp"

#include <typeinfo>
#include "hatrix/actions/idle.hpp"
#include "hatrix/world.hpp"
#include "hatrix/gamemap.hpp"

std::map<int, std::vector<chtype>> Wall::mask_to_glyph = {
    {0b0000, {'#'}},
    {0b1000, {'|', L'└', L'┘'}},
    {0b0100, {L'─', L'┐', L'┘'}},
    {0b0010, {'|', L'┐', L'┌'}},
    {0b0001, {L'─', L'┌', L'└'}},

    {0b1010, {'|'}},
    {0b0101, {L'─'}},
    {0b0011, {L'┌'}},
    {0b0110, {L'┐'}},
    {0b1001, {L'└'}},
    {0b1100, {L'┘'}},

    {0b0111, {L'┬'}},
    {0b1101, {L'┴'}},
    {0b1011, {L'├'}},
    {0b1110, {L'┤'}},

    {0b1111, {L'┼'}},
};

Wall::Wall(){
    glyph = '#';
    opaque = true;
    blocking = true;
};

Action *Wall::make_decision_as_npc() {
    return new ActionIdle();
};

void Wall::update_glyph() {
    mask = compute_mask();
    std::vector<chtype> &glyphs = Wall::mask_to_glyph[mask];
    int random_index = std::rand() % glyphs.size(); // 获取随机索引
    glyph = glyphs[random_index];
};

int Wall::compute_mask() {
    int x = position.x;
    int y = position.y;

    int mask = 0b0000;

    auto has_wall = [](Entity *entity)
    { return typeid(*entity) == typeid(Wall); };

    if (world->gamemap->has_entity_at_which(x, y-1, has_wall)){
        mask |= 0b1000;
    }
    if (world->gamemap->has_entity_at_which(x-1, y, has_wall)){
        mask |= 0b0100;
    }
    if (world->gamemap->has_entity_at_which(x, y+1, has_wall)){
        mask |= 0b0010;
    }
    if (world->gamemap->has_entity_at_which(x+1, y, has_wall)){
        mask |= 0b0001;
    }
    return mask;
};
