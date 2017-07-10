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
#include "Base.h"

std::list<Base *> Base::liveObjects;
std::list<Base *> Base::deadObjects;
BaseFactory::map_type * BaseFactory::map;

Base::Base()
{
    liveObjects.push_back(this);

    //update the constructor to initialise refCount to zero
    refCount=0;
}

Base::~Base()
{
    //dtor
}



void Base::AddRef()
{
    ++refCount;
}

void Base::Release()
{
    --refCount;
   if(refCount<=0)
   {
      liveObjects.remove(this);
      deadObjects.push_back(this);
   }
}

void Base::CollectGarbage()
{
    for(std::list<Base *>::iterator it=deadObjects.begin();
   	it!=deadObjects.end(); it++)
    {
        delete (*it);
    }
    deadObjects.clear();
}

void Base::CollectRemainingObjects(bool bEmitWarnings)
{
    CollectGarbage();
    for(std::list<Base*>::iterator it=liveObjects.begin();
   	it!=liveObjects.end(); it++)
    {
        Base *o=(*it);
        if(bEmitWarnings)
        {
            //log some kind of error message here
            //we'll be covering how to log messages later this article
        }
        delete o;
    }
    liveObjects.clear();
}
