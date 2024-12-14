#ifndef HATRIX_ACTION
#define HATRIX_ACTION
class Entity;

class Action {
    public:
        Action();
        virtual ~Action();
        virtual float perform(Entity *entity) = 0;
};
#endif