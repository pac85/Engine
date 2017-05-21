#ifndef GAME_LOOP_H
#define GAME_LOOP_H
#include "common.h"
#include "Ptr.h"
#include "Task.h"

class Loop
{
    struct loop_task
    {
        Ptr<ITask> _task;
        operator > (loop_task b)
        {
            return _task->priority > b._task->priority;
        }

        operator < (loop_task b)
        {
            return _task->priority < b._task->priority;
        }
    };

    Ptr<ITask> init;
    vector<loop_task> tasks;

    void  Process_loop()
    {
        sort(tasks.begin(), tasks.end());

        for(int i = tasks.size(); i >= 0;i--)
        {
            if(tasks[i]._task->canKill)
                tasks.erase(tasks.begin() + i);
            else
                tasks[i]._task->Update();
        }
    }


};




#endif // GAME_LOOP_H
