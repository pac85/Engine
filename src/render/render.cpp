#include "render.h"

render::render()
{
    //ctor
}

render::~render()
{
    //dtor
}


unsigned int render::add_model(Actor * parent)
{
    model_list.push_back(ModelComponent(parent));
    return model_list.size()-1;
}

void render::remove_model(unsigned int index)
{
    model_list.erase(model_list.begin()+index);
}
