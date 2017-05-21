#ifndef VKQUEUES_H
#define VKQUEUES_H

#include "../common.h"

#include "vkGraphicsQueue.h"
#include "vkPresentQueue.h"
#include "vkTransferQueue.h"


class vkQueues
{
    public:
        vkQueues();
        virtual ~vkQueues();

        vkGraphicsQueue graphics;
        vkPresentQueue present;
        vkTransferQueue transfer;

    protected:

    private:
};

#endif // VKQUEUES_H
