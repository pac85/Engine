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
#include "Profiler.h"

map<string, Profiler::sample*> Profiler::samples;

Profiler::Profiler(string _name)
{
    name = _name;
}

Profiler::~Profiler()
{
    //dtor
}

Profiler::sample::sample(int time, int _spp_count)
{
    avg_time = time;
    spp_count = _spp_count;
}

void Profiler::start()
{
    start_time = glfwGetTime();
}

void Profiler::stop()
{
    map<string, sample*>::iterator it;
    it = samples.find(name);
    if(it == samples.end())
    {
        samples[name] = new sample(glfwGetTime() - start_time, 0);
    }
    else
    {
        sample prev_spp = *it->second;
        float fac = (1 + prev_spp.spp_count) / prev_spp.spp_count;
        it->second->avg_time = prev_spp.avg_time * (1-fac) + (glfwGetTime() - start_time) * fac;
        it->second->spp_count = prev_spp.spp_count + 1;
    }
}
