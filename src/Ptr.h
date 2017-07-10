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
#ifndef PTR_H
#define PTR_H
#include "common.h"
#include "Base.h"

template<class T>
class Ptr
{
    protected:
        T* obj;

    private:

    public:
        Ptr()
        {
            obj = 0;
        }

        Ptr(T* o)
        {
            obj = 0;
            *this = o;
        }

        Ptr(const Ptr<T> &p)
        {
            obj = 0;
            *this = p;
        }

        ~Ptr()
        {
            if(obj) {obj->Release();}
        }

        //Assignement operators - assigning a plain pointer
        inline operator =(T *o)
        {
            if(obj)obj->Release();
            obj=o;
            if(obj)obj->AddRef();
            return *this;
        }

        //Assigning another smart pointer
        inline operator =(const Ptr<T> &p)
        {
            if(obj)obj->Release();
            obj=p.obj;
            if(obj)obj->AddRef();
            //return *this;
        }

        //Creates an instance from instance name
        operator =(const string s){
            if(obj)obj->Release();
            obj= BaseFactory::createInstance(s);
            if(obj)obj->AddRef();
            return *this;
        }

        //Access as a reference
        inline T& operator *() const
        {
            assert(obj!=0 && "Tried to * on a NULL smart pointer");
            return *obj;
        }
        //Access as a pointer
        inline T* operator ->() const
        {
            assert(obj!=0 && "Tried to -> on a NULL smart pointer");
            return obj;
        }

        inline operator T*() const
        {
            return obj;
        }

        inline bool isValid() const
        {
            return (obj!=0);
        }
        inline bool operator !()
        {
            return !(obj);
        }
        inline bool operator ==(const Ptr<T> &p) const
        {
            return (obj==p.obj);
        }
        inline bool operator ==(const T* o) const
        {
            return (obj==o);
        }
};

#endif // PTR_H
