#ifndef COMPONENT_H
#define COMPONENT_H

#include<Transform.h>

class Actor;

class Component
{
    public:
        Component(Actor * parent);
        virtual ~Component();

        Actor * parent_ptr;

        virtual void update(float delta_time) = 0;

    protected:

    private:
};

#endif // COMPONENT_H
