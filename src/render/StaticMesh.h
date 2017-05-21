#ifndef MODEL_H
#define MODEL_H

#include "../common.h"
#include "vk/vkUtils.h"
#include "vk/vkDeleter.hpp"
#include "vk/vkDevice.h"
#include "vk/vkPhysicalDevice.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


class StaticMesh
{
    public:
        StaticMesh();
        virtual ~StaticMesh();

        /*//loads a model from the file specified in the first parameter, the second optional parameter specifies assimp flags
        void load_model(const string &file, unsigned int ai_flags = 0aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);*/
        void load_model(aiMesh mesh);

        static VkCommandPool * command_pool;
        static vkDevice * device;

        VDeleter<VkBuffer> vertex_buffer{device->device, vkDestroyBuffer};
        VDeleter<VkDeviceMemory> vertex_buffer_memory{device->device, vkFreeMemory};

        VDeleter<VkBuffer> normal_buffer{device->device, vkDestroyBuffer};
        VDeleter<VkDeviceMemory> normal_buffer_memory{device->device, vkFreeMemory};

        vector<VDeleter<VkBuffer>> uv_buffers;
        vector<VDeleter<VkDeviceMemory>> uv_buffers_memory;

        VDeleter<VkBuffer> index_buffer{device->device, vkDestroyBuffer};
        VDeleter<VkDeviceMemory> index_buffer_memory{device->device, vkFreeMemory};

        /*enum attributes_modes
        {
            NO_NORM_UV_0    = 0,
            NO_NORM_UV_1    = 1,
            NO_NORM_UV_2    = 2,
            UV_0            = 3,
            UV_1            = 4,
            UV_2            = 5,

        }attributes_mode;

        enum instancing_modes
        {
            NON_INSTANCED_ONLY,
            INSTANCED_ONLY,
            INSTANCEABLE
        }instancing_mode;


        const VkVertexInputBindingDescription binding_description[2][6] =
        {
            {
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX)
            },

            {
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX)
            }
        };*/

    protected:

    private:

};

#endif // MODEL_H
