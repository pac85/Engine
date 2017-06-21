#ifndef RENDER_H
#define RENDER_H

#include"../common.h"
#include"ModelComponent.h"
#include"../ActorSYS/Actor.h"


class render
{
    public:
        render();
        virtual ~render();

        unsigned int add_model(Actor * parent);
        void remove_model(unsigned int index);

    protected:

    private:

        vector<ModelComponent> model_list;
};

#endif // RENDER_H
