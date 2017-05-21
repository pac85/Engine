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
