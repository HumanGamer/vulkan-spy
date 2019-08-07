#ifdef WINDOWS_MODE
#include <Windows.h>
#else
#include <iostream>
#endif

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

/*
 * Display a message to the user
 */
void display_message(std::string msg, bool error = false)
{
#ifdef WINDOWS_MODE
    if (error)
        MessageBox(nullptr, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
    else
        MessageBox(nullptr, msg.c_str(), "Information", MB_OK | MB_ICONINFORMATION);
#else
    if (error)
        std::cerr << msg << std::endl;
    else
        std::cout << msg << std::endl;
#endif
}

/*
 * Get GPU name and version as a string
 */
std::string get_gpu_and_version(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    std::string name = deviceProperties.deviceName;

    uint32_t versionMajor = VK_VERSION_MAJOR(deviceProperties.apiVersion);
    uint32_t versionMinor = VK_VERSION_MINOR(deviceProperties.apiVersion);
    uint32_t versionPatch = VK_VERSION_PATCH(deviceProperties.apiVersion);

    std::string msg = name + " - Vulkan ";
    char buffer[256];
    msg += _itoa(versionMajor, buffer, 10);
    msg += '.';
    msg += _itoa(versionMinor, buffer, 10);
    msg += '.';
    msg += _itoa(versionPatch, buffer, 10);

    return msg;
}

/*
 * Main entry point
 */
int main(int argc, const char** argv)
{
    // Attempt to create a vulkan instance
    VkInstance instance;
    VkInstanceCreateInfo createInfo = {};
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (instance == nullptr || result != VK_SUCCESS)
    {
        display_message("Failed to initialize vulkan.", true);
        return 1;
    }

    // Get number of GPUs supporting vulkan
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        display_message("Could not find any GPUs with Vulkan support.", true);
    }

    // Put all GPUs supporting vulkan into a list
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Start the output message
    std::string msg = "The following GPUs support vulkan:\n\n";

    // Loop through all devices...
    for (const auto& device : devices)
    {
        // .. and append them to the message
        msg += get_gpu_and_version(device) + "\n";
    }

    // Display the output message
    display_message(msg.c_str());

    return 0;
}
