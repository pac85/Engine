#ifndef VKEXCEPTIONS_H_INCLUDED
#define VKEXCEPTIONS_H_INCLUDED

#include"../common.h"
#include<exception>

class vkUnavailableFeature : public exception
{
    string msg;
    public:
        vkUnavailableFeature (string _msg)
        {
            msg = _msg;
        }
        virtual const char* what() const throw()
        {
            return msg.c_str();
        }
};

#endif // VKEXCEPTIONS_H_INCLUDED
