#ifndef ENGINEMAIN_H
#define ENGINEMAIN_H

#include "common.h"
#include "logger.h"

class EngineMain
{
    public:
        EngineMain();
        virtual ~EngineMain();

        logger slog;  //the standard log output

    protected:

    private:
};

#endif // ENGINEMAIN_H
