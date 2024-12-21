#include "hatrix/entities/character.hpp"

void Character::mark_as_visited(int x, int y) {
    Vec2 v{x, y};
    // 检查是否已经在记忆中
    if(visited.find(v) != visited.end()){
        return;
    }

    if (visited.size() >= memory) // 记忆过载，需要删除，最靠前的记忆最久
    {
        Vec2 vd = _visited.front();
        _visited.pop_front();

        visited.erase(vd);
    };
    _visited.push_back(v);
    visited.insert(v);
};
