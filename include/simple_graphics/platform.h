#ifndef SG_PLATFORM_H_
#define SG_PLATFORM_H_

#include <vulkan/vulkan.h>
#include <stdbool.h>

/**
* @brief Number of extensions required by a vulkan instance.
**/
extern uint32_t INSTANCE_EXTENSIONS_COUNT;

/**
* @brief Extensions required by a vulkan instance.
**/
extern const char* INSTANCE_EXTENSIONS[];

/**
* @brief Opaque handler for cross-platform windows.
**/
typedef struct SGwindow SGwindow;

/**
* @return Vulkan instance function compatible with the passed instance.
**/
PFN_vkVoidFunction sgGetVulkanInstanceFunction(VkInstance vkInstance, const char* pName);

/**
* @brief Creates a SimpleGraphics window.
* @return True if the window was created successfully, false otherwise.
**/
SGwindow* sgCreateWindow(const char* title, int width, int height, bool resizable);

/**
* @brief Destroys a SimpleGraphics window.
**/
void sgDestroyWindow(SGwindow* window);

/**
* @return True if the passed window exists, false otherwise.
**/
bool sgWindowExists(SGwindow* window);

/**
* @brief Processes platform dependant window events.
**/
void sgProcessWindow(SGwindow* window);

/**
* @brief Creates a vulkan surface appropiate for your platform
**/
VkResult sgVkCreateSurface(VkInstance vkInstance, SGwindow* window, VkSurfaceKHR* vkSurface);

#endif // !SG_PLATFORM_H_