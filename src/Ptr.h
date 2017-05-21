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
