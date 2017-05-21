#ifndef FUNCTION_H
#define FUNCTION_H

#include"common.h"
#include"Base.h"

template<class RT>
class Function : Base
{
    public:

        enum arg_types
        {
            INT, LONG, UINT, ULONG, FLOAT, DOUBLE, CHAR
        };

        struct arg_type
        {
            int num_args;
            arg_types types;
        };


        Function();
        virtual ~Function();

        void Reg_function(string name, RT(*func_ptr)(arg_type, ...))
        {
            func_name_map[name] = func_ptr;
        }

        RT operator() (string name, arg_type argt, ...)
        {
            void * args;
            return func_name_map[name](argt, args);
        }

    protected:

    private:

        static map<string, void (*)(arg_type, ...)>func_name_map;
};

#endif // FUNCTION_H
