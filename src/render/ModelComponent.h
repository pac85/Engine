#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include"../ActorSYS/Component.h"
#include"StaticMesh.h"


class ModelComponent : Component
{
    public:
        ModelComponent(BaseActor * parent);
        virtual ~ModelComponent();

        StaticMesh mesh;


    protected:

    private:

        Transform transform;
};

#endif // MODELCOMPONENT_H
