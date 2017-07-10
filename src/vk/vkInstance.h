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
#ifndef VKINSTANCE_H
#define VKINSTANCE_H
#include "../common.h"
#include "vkDeleter.hpp"

class vkInstance
{
    public:
        vkInstance(VkApplicationInfo app_info, bool _use_validation_layers, const std::vector<const char*> * _validationLayers);
        virtual ~vkInstance();

        VDeleter<VkInstance> instance {vkDestroyInstance};

        static VkResult create_debug_report_callbackEXT
        (
            VkInstance instance,
            const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugReportCallbackEXT* pCallback
        );

        static void DestroyDebugReportCallbackEXT
        (
            VkInstance instance,
            VkDebugReportCallbackEXT callback,
            const VkAllocationCallbacks* pAllocator
        );

        bool using_validation_layers();
        const std::vector<const char*> * get_validation_layers();

    protected:

    private:

        bool bUsing_validation_layers;
        const std::vector<const char*> * validationLayers;

        VDeleter<VkDebugReportCallbackEXT> callback{instance, DestroyDebugReportCallbackEXT};

        /*********functions**********/

        std::vector<const char*> getRequiredExtensions();

        bool check_validation_layer_support();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
        (
            VkDebugReportFlagsEXT flags,
            VkDebugReportObjectTypeEXT objType,
            uint64_t obj,
            size_t location,
            int32_t code,
            const char* layerPrefix,
            const char* msg,
            void* userData
        );
};

#endif // VKINSTANCE_H
