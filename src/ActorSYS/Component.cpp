#include "Component.h"
#include "Actor.h"

Component::Component(Actor * parent)
{
    parent_ptr = parent;
}

Component::~Component()
{
    //dtor
}

