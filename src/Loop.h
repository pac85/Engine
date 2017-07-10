/**************************************************************************/
/*
  Copyright (C) 2017 Antonino Maniscalco (alias pac85)

  This file is part of Engine.

  Engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Engine.  If not, see http://www.gnu.org/licenses/

*/
/**************************************************************************/
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
