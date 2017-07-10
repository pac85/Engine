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
