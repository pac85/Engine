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
#ifndef SETTINGS_H
#define SETTINGS_H

#include"common.h"


class settings
{
    public:
        settings();
        settings(string file_name);

        virtual ~settings();

        void load_settings(string file_name);

        struct setting
        {

            enum _s_type
            {
                INT,
                FLOAT,
                BOOL,
                STRING
            } s_type;

            union _s_value
            {
                int i;
                float f;
                bool b;
                string * s;
            }s_value;

            /**temp*/
            template<typename T> T inline get_val()   //this is not type safe, but i couldn't find a better way
            {
                return (T)s_value.i;                //just uses the biggest (32 bit) type so no data is lost
            }
        };

        struct s_defs
        {
            static map<string, string> def_value; //maps definitions to values

            static inline string get_val(string name)
            {
                return def_value[name];
            }

            static inline bool is_def(string name)
            {
                return def_value.find(name) != def_value.end();
            }

            static inline bool has_any()
            {
                return !def_value.empty();
            }
        };

        bool get_setting(string identifier, setting &out_s);

    protected:

    private:

        map<string, setting> id_val_map;
        void parse_preprocessor_keyword(string line, string settings_file_name);
        void parse_keyword(string line);
        string subst_defs(string in_line); //does the substitution
};

#endif // SETTINGS_H
