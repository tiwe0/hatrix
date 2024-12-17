#ifndef HATRIX_ACTION
#define HATRIX_ACTION
#include <string>

class Entity;

class Action {
    public:
        Action();
        virtual ~Action();
        virtual float perform(Entity *entity) = 0;
        virtual operator std::string() const = 0;
};
#endif