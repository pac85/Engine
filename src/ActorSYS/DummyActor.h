#ifndef DUMMYACTOR_H
#define DUMMYACTOR_H

#include "BaseActor.h"

class DummyActor : public BaseActor
{
    public:
        DummyActor();
        virtual ~DummyActor();


        void update()
        {

        }
        void tick(float delta)
        {

        }
        void GameStart()
        {

        }

    protected:

    private:
};

#endif // DUMMYACTOR_H
