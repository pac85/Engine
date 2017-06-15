#ifndef COMPONENT_H
#define COMPONENT_H

#include "BaseActor.h"

class Component
{
    public:
        Component(BaseActor * parent);
        virtual ~Component();

        BaseActor * parent_ptr;

        virtual void update(float delta_time) = 0;

    protected:

    private:
};

#endif // COMPONENT_H
