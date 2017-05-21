#include "engine_settings.h"

settings engine_settings::g_engine_settings;

void engine_settings::load_settings(string filename)
{
        g_engine_settings.load_settings(filename);
}
