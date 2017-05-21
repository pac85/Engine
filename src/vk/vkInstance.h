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
