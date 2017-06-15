#include "Component.h"

Component::Component(BaseActor * parent)
{
    parent_ptr = parent;
}

Component::~Component()
{
    //dtor
}

