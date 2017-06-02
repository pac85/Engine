#include <vulkan/vulkan.h>

#include "Ptr.h"
#include "Base.h"
#include "Function.h"
#include "settings.h"
#include "logger.h"
#include "engine_settings.h"
#include "vk/vkInstance.h"
#include "vk/vkSurface.h"
#include "vk/vkPhysicalDevice.h"
#include "vk/vkDevice.h"
#include "vk/vkSwapChain.h"
#include "vk/vkShaderModule.h";
#include "render/StaticMesh.h"
#include "vk/vkMain.h"
#include "ActorSYS/World.h"
#include "ActorSYS/DummyActor.h"

char* work_dir;

class GameProgram
{
    public:
        void run() {
            init();
            mainLoop();
        }

    private:
        void init()
        {

        }

        void mainLoop()
        {

        }
};

void start_game()
{

}

GLFWwindow* window;
void Init(string app_name)
{
    engine_settings::load_settings("./cfg/basic_settings.cfg");
    settings base_settings("./cfg/basic_settings.cfg");

    /*logger test_log("test.txt");

    test_log << msg("test msg");
    test_log << warn("test warn");
    test_log << err("test err");*/


    //initializes glfw
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    //reads resolution from config file
    settings::setting height, width, b_full_screen;
    base_settings.get_setting("height", height);
    base_settings.get_setting("width", width);
    base_settings.get_setting("full_screen", b_full_screen);

    //creates the windows
    window = glfwCreateWindow(width.get_val<int>(), height.get_val<int>(), app_name.c_str(), b_full_screen.get_val<bool>()? glfwGetPrimaryMonitor() : nullptr, nullptr);

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = app_name.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    const std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

    /*cout << "creating instance" << endl;
    vkInstance instance(appInfo, true, &validationLayers);

    cout << "creating surface" << endl;
    vkSurface surface(window, instance);

    cout << "creating physical device" << endl;
    vector<const char*> device_extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    vkPhysicalDevice physicaldevice(instance, surface, &device_extensions);

    cout << "creating logical device" << endl;
    vkDevice device(instance, physicaldevice, &device_extensions);


    cout << "creating swapchain" << endl;
    VkSurfaceFormatKHR sf{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    VkPresentModeKHR pm= VK_PRESENT_MODE_FIFO_KHR;
    VkExtent2D extent = {width.get_val<int>(), height.get_val<int>()};

    vkSwapChainDesiredProperties dprops(sf, pm, extent);

    vkSurfaceInfo si(physicaldevice.physical_device, surface);

    vkSwapChain swapchain(physicaldevice, device, surface, si, dprops, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);

    cout << "creating test shader module" << endl;
    vkShaderModule testmodule(device, "shaders/frag.spv");*/

    /*cout << "creating test staic mesh" << endl;
    StaticMesh::device = &device;
    StaticMesh test;*/

    vector<const char*> device_extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    VkSurfaceFormatKHR sf{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    VkPresentModeKHR pm= VK_PRESENT_MODE_FIFO_KHR;
    VkExtent2D extent = {width.get_val<int>(), height.get_val<int>()};

    vkMain testmain(appInfo, window, device_extensions, sf, pm, extent);

}

int main(int argc, char *argv[]) {
    //HelloTriangleApplication app;

    work_dir = argv[0];

    /*#ifdef DEBUG
        use_validation_layers = true;
    #else
        use_validation_layers = false;
    #endif // DEBUG*/

    bool game_editor = false;   //if false game mode will be initialized, otherwise editor mode

    for(int i = 1;i<argc;i++)
    {
        string arg(argv[i]);
        if(!arg.compare("-edit"))
        {
            game_editor = true;
        }
    }

    Init("test");

    world test_world;

    for(int i = 0; i < 1;i++)
    {
        test_world.add_actor(new DummyActor);
    }
    double currentFrame = glfwGetTime();
    double lastFrame = currentFrame;
    double deltaTime;

    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        test_world.UpdateWorld(deltaTime);
        cout << "\r" << 1.0 / deltaTime << "       ";
    }

    cout << endl;

   /*Ptr<Base> test;

    test = "test";


    try {
        start_game();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }*/
    if(!game_editor)
    {
        GameProgram CGame;
    }

    return EXIT_SUCCESS;
}
