#ifndef ENGINE_SETTINGS_H_INCLUDED
#define ENGINE_SETTINGS_H_INCLUDED

#include"common.h"
#include"settings.h"

namespace engine_settings
{
    extern settings g_engine_settings;

    extern void load_settings(string filename);

}

#endif // BASIC_SETTINGS_H_INCLUDED
