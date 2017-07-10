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
#ifndef VK_DELETER_H_INCLUDED
#define VK_DELETER_H_INCLUDED
#include <../common.h>


template <typename T>
class VDeleter
{
    public:
        VDeleter() : VDeleter([](T, VkAllocationCallbacks*) {}) {}

        VDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef)
        {
            this->deleter = [=](T obj) { deletef(obj, nullptr); };
        }

        VDeleter(const VDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef)
        {
            this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
        }

        VDeleter(const VDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef)
        {
            this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
        }

        VDeleter(VDeleter<VkDevice>* device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef)
        {
            this->deleter = [device, deletef](T obj) { deletef(*device, obj, nullptr); };
        }

        ~VDeleter()
        {
            cleanup();
        }

        const T* operator &() const
        {
            return &object;
        }

        T* replace()
        {
            cleanup();
            return &object;
        }

        operator T() const
        {
            return object;
        }

        void operator=(T rhs)
        {
            if (rhs != object) {
                cleanup();
                object = rhs;
            }
        }

        template<typename V>
        bool operator==(V rhs)
        {
            return object == T(rhs);
        }

    private:
        T object{VK_NULL_HANDLE};
        std::function<void(T)> deleter;

        void cleanup() {
            if (object != VK_NULL_HANDLE) {
                deleter(object);
            }
            object = VK_NULL_HANDLE;
        }
};

#endif // VK_DELETER_H_INCLUDED
