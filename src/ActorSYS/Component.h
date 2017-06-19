#ifndef COMPONENT_H
#define COMPONENT_H

#include "BaseActor.h"

class Component
{
    public:
        Component(BaseActor * parent);
        virtual ~Component();

        BaseActor * parent_ptr;

    protected:

    private:
};

#endif // COMPONENT_H
