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
#include "settings.h"

map<string, string> settings::s_defs::def_value;

settings::settings()
{

}

settings::settings(string file_name)
{
    load_settings(file_name);
}


settings::~settings()
{
    //dtor
}

inline bool is_preprocessor_keyword(string line)
{
    for(unsigned int i = 0;i < line.size();i++)
    {
        //ignore spaces
        if(line[i] == ' ')
            continue;

        if(line[i] == '#')
            return true;
        else
            break;
    }
    return false;
}

void settings::load_settings(string file_name)
{
    fstream settings_file;

    settings_file.open(file_name, ios::in);

    if(!settings_file.is_open())
    {
        cerr << "unable to find file " << file_name << endl;

        return;
    }

    string line;

    while(getline(settings_file, line))
    {
        if(is_preprocessor_keyword(line))
        {
            parse_preprocessor_keyword(line, file_name);
            continue;
        }

        line = subst_defs(line); //substitutes any def with the actual value

        stringstream line_stream(line);

        string l_type, l_idef;      //data type and identifier

        char dummy_ch;              //used to pull the = character from the string stream

        line_stream >> l_type >> l_idef >> dummy_ch;

        setting temp_setting;

        if(!l_type.compare("int"))
        {
            temp_setting.s_type = setting::INT;
            line_stream >> temp_setting.s_value.i;
        }
        else if(!l_type.compare("float"))
        {
            temp_setting.s_type = setting::FLOAT;
            line_stream >> temp_setting.s_value.f;
        }
        else if(!l_type.compare("bool"))
        {
            temp_setting.s_type = setting::BOOL;

            //converts the string value "true" or "false" to an actual boolean value
            string str_bool;
            line_stream >> str_bool;
            if(!str_bool.compare("true"))
            {
                temp_setting.s_value.b = true;
            }
            else if(!str_bool.compare("false"))
            {
                temp_setting.s_value.b = false;
            }
        }
        else if(!l_type.compare("string"))
        {
            temp_setting.s_type = setting::STRING;
            temp_setting.s_value.s = new string;
            line_stream >> *temp_setting.s_value.s;
        }
        else
        {
            cerr << "error while loading settings file " << file_name << ", unknown type " << l_type << endl;
        }

        id_val_map[l_idef] = temp_setting;
    }

    settings_file.close();
}

bool settings::get_setting(string identifier, setting &out_s)
{
    //if the setting doesn't exist return false
    if(id_val_map.count(identifier) == 0)
    {
        return false;
    }

    //otherwise give the corresponding value and return true
    out_s = id_val_map[identifier];
    return true;
}

void settings::parse_preprocessor_keyword(string in_line, string settings_file_name)
{
    const int KWLEN = 8; //length of the keyword
    unsigned int k_pos = in_line.find("#include");
    //does nothing if the keyword is not found
    if(k_pos == string::npos)
        return;

    //filename with quotation marks and other garbage
    string qfilename;
    qfilename.resize(in_line.size() - (k_pos+KWLEN));
    try
    {
        in_line.copy((char*)qfilename.c_str(), in_line.size() - (k_pos+KWLEN), k_pos+KWLEN);
    }
    catch(const out_of_range er)
    {
        cerr << "1 " << er.what() << endl;
    }

    //finds the two quotation marks delimiting the file name
    unsigned int first_quot, second_quot;
    first_quot = qfilename.find('"', 0);
    second_quot = qfilename.find('"', first_quot + 1);

    if(first_quot == string::npos || second_quot == string::npos)
        return;

    int filename_length = second_quot-first_quot-1;

    //finds the actual filename
    string filename;
    filename.resize(filename_length);
    try
    {
        qfilename.copy((char*)filename.c_str(), filename_length, first_quot + 1);
    }
    catch(const out_of_range er)
    {
        cerr << "2 "<<er.what() << endl;
    }

    fstream header_file;
    header_file.open(filename);

    if(!header_file.is_open())
    {
        cerr << "unable to find file " << filename << endl;
        return;
    }

    string line;
    while(getline(header_file, line))
    {
        parse_keyword(line);
    }
}

void settings::parse_keyword(string line)
{
    stringstream line_stream(line);

    string keyword;
    line_stream >> keyword;

    if(keyword == "#define")
    {
        string idef, value;

        line_stream >> idef >> value;

        if(line_stream.fail())
            return;

        if(s_defs::is_def(idef))
            cout << "redefinition of " << idef << endl;

        s_defs::def_value[idef] = value;
    }
}

string settings::subst_defs(string in_line)
{
    stringstream in_line_stream(in_line);
    string type, idef, value;
    char dummy;
    in_line_stream >> type >> idef >> dummy >> value;

    if(s_defs::is_def(value))
    {
        stringstream out_line_stream;
        out_line_stream << type << " " << idef  << " = " << s_defs::def_value[value];

        return out_line_stream.str();
    }
    else
    {
        return in_line;
    }
}
