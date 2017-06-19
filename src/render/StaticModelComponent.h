#ifndef STATICMODELCOMPONENT_H
#define STATICMODELCOMPONENT_H

#include"../ActorSYS/Component.h"
#include"StaticMesh.h"


class StaticModelComponent : Component
{
    public:
        StaticModelComponent(BaseActor * parent);
        virtual ~StaticModelComponent();

        StaticMesh mesh;


    protected:

    private:

        Transform transform;
        void render();
};

#endif // STATICMODELCOMPONENT_H
