#ifndef VKQUEUEFAMILIES_H
#define VKQUEUEFAMILIES_H


class vkQueueFamilies
{
    public:
        vkQueueFamilies();
        virtual ~vkQueueFamilies();
        struct family
        {
            int index;
        };
        family graphics, present, transfer;

    protected:

    private:
};

#endif // VKQUEUEFAMILIES_H
