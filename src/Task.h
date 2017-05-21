#ifndef TASK_H
#define TASK_H
#include"Base.h"
class ITask : public Base
{
public:


    bool canKill;
    long priority;

    enum _ex_mode
    {
        SERVER,
        CLIENT,
        EDITOR
    }ex_mode;

    ITask(){canKill=false;priority=5000;}
    virtual bool Start(_ex_mode mode)=0;
    virtual void OnSuspend(){};
    virtual void Update()=0;
    virtual void OnResume(){};
    virtual void Stop()=0;
};
#endif
