#ifndef BASE_H
#define BASE_H
#include"common.h"

class Base
{
    private:
        static std::list<Base *> liveObjects;
        static std::list<Base *> deadObjects;
        long refCount;
    protected:
        Base();
        virtual ~Base();
    public:
        void AddRef();
        void Release();
        static void CollectGarbage();
        static void CollectRemainingObjects(bool bEmitWarnings=false);
        virtual unsigned long size()=0;
};

template<typename T> Base * createT() { return new T; }

struct BaseFactory
{

        typedef std::map<std::string, Base*(*)()> map_type;

        static Base * createInstance(std::string const& s)
        {
            map_type::iterator it = getMap()->find(s);
            if(it == getMap()->end())
                return 0;
            return it->second();
        }

    protected:

        static map_type * getMap()
        {
            // never delete'ed. (exist until program termination)
            // because we can't guarantee correct destruction order
            if(!map) { map = new map_type; }
            return map;
        }

    private:

        static map_type * map;
};


template<typename T>
struct DerivedRegister : BaseFactory
{
    DerivedRegister(std::string const& s)
    {
        getMap()->insert(std::make_pair(s, &createT<T>));
    }
};

#define REGISTER_DEC_TYPE(NAME) \
    static DerivedRegister<NAME> reg

#define REGISTER_DEF_TYPE(NAME) \
    DerivedRegister<NAME> NAME::reg(#NAME)



#define AUTO_SIZE unsigned long size(){return sizeof(*this);}
#endif // BASE_H
