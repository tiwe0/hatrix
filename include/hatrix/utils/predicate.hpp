#ifndef HATRIX_UTILS_PREDICATE
#define HATRIX_UTILS_PREDICATE

#include <functional>
#include <typeinfo>
#include "hatrix/utils/predicate.hpp"
#include "hatrix/entities/entity.hpp"
#include "hatrix/entities/door.hpp"
#include "hatrix/entities/wall.hpp"
#include "hatrix/entities/character.hpp"

template<typename T>
inline bool is_(Entity * entity){
    return typeid(*entity) == typeid(T);
};

inline bool is_wall(Entity * entity){
    return is_<Wall>(entity);
};

inline bool is_door(Entity * entity){
    return is_<Door>(entity);
};

inline bool is_character(Entity *entity){
    return is_<Character>(entity);
};
#endif