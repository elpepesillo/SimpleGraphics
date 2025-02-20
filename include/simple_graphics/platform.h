#ifndef SG_PLATFORM_H_
#define SG_PLATFORM_H_

#include <vulkan/vulkan.h>
#include <stdbool.h>

extern uint32_t INSTANCE_EXTENSIONS_COUNT;

extern const char* INSTANCE_EXTENSIONS[];

typedef struct SGwindow_T* SGwindow;

bool sgCreateWindow(const char* title, int width, int height, bool resizable, SGwindow* pWindow);

void sgDestroyWindow(SGwindow window);

bool sgIsWindowClosed(SGwindow window);

void sgProcessWindow(SGwindow window);

void sgShowWindow(SGwindow window);

bool sgGetInstanceFunctionPointer(VkInstance vkInstance, const char* pName, PFN_vkVoidFunction* pFunction);

bool sgGetDeviceFunctionPointer(VkInstance vkInstance, VkDevice vkDevice, const char* pName, PFN_vkVoidFunction* pFunction);

VkResult sgVkCreateSurface(VkInstance vkInstance, SGwindow window, VkSurfaceKHR* vkSurface);

#endif // !SG_PLATFORM_H_