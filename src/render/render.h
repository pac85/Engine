#ifndef RENDER_H
#define RENDER_H

#include"../common.h"
#include"StaticModelComponent.h"


class render
{
    public:
        render();
        virtual ~render();

    protected:

    private:

        vector<StaticModelComponent> StaticModelsList;
};

#endif // RENDER_H
