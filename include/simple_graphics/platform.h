#ifndef SG_PLATFORM_H_
#define SG_PLATFORM_H_

#include <vulkan/vulkan.h>
#include <stdbool.h>

/**
* @brief After initialization, this will point to the vulkan dll vkGetInstanceProcAddr function.
**/
extern PFN_vkGetInstanceProcAddr instanceProcAddr;

/**
* @brief Number of extensions required by a vulkan instance.
**/
extern uint32_t INSTANCE_EXTENSIONS_COUNT;

/**
* @brief Extensions required by a vulkan instance.
**/
extern const char* INSTANCE_EXTENSIONS[];

/**
* @brief True if the platform has already been initialized, false otherwise.
**/
extern bool isSgInitialized;

/**
* @brief Opaque handler for platform dependant windows.
**/
typedef struct SGwindow_T* SGwindow;

/**
* @brief Initializes platform dependant variables.
* @return True if the initialization was successful, false otherwise.
**/
bool sgInitialize();

/**
* @brief Creates a cross platform window.
* @return True if the window was created successfully, false otherwise.
**/
bool sgCreateWindow(const char* title, int width, int height, bool resizable, SGwindow* pWindow);

/**
* @brief Destroys a SimpleGraphics window.
**/
void sgDestroyWindow(SGwindow window);

/**
* @return True if the passed window is closed, false otherwise.
**/
bool sgIsWindowClosed(SGwindow window);

/**
* @brief Processes platform dependant window events.
**/
void sgProcessWindow(SGwindow window);

/**
* @brief Creates a vulkan surface appropiate for your platform
**/
VkResult sgVkCreateSurface(VkInstance vkInstance, SGwindow window, VkSurfaceKHR* vkSurface);

#endif // !SG_PLATFORM_H_