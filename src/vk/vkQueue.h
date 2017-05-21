#ifndef VKQUEUE_H
#define VKQUEUE_H

#include "../common.h"
#include "vkDeleter.hpp"


class vkQueue
{
    public:
        vkQueue();
        virtual ~vkQueue();

        VkQueue queue;

    protected:

    private:
};

#endif // VKQUEUE_H
