#include <simple_graphics/graphics.h>
#include <simple_graphics/platform.h>

VkResult sgVkCreateInstance(VkInstance* vkInstance) {
	const VkApplicationInfo appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = NULL,
		.pApplicationName = NULL,
		.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
		.pEngineName = NULL,
		.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
		.apiVersion = VK_MAKE_API_VERSION(0, 1, 0, 0)
	};

	const VkInstanceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = INSTANCE_EXTENSIONS_COUNT,
		.ppEnabledExtensionNames = INSTANCE_EXTENSIONS
	};

	PFN_vkCreateInstance createInstance = NULL;
	if (!sgGetInstanceProcAddr(NULL, "vkCreateInstance", &(PFN_vkVoidFunction)createInstance)) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	return createInstance(&createInfo, NULL, vkInstance);
}

void sgVkDestroyInstance(VkInstance vkInstance) {
	PFN_vkDestroyInstance destroyInstance = NULL;
	if (!sgGetInstanceProcAddr(vkInstance, "vkDestroyInstance", &(PFN_vkVoidFunction)destroyInstance)) {
		destroyInstance(vkInstance, NULL);
	}
}

void sgVkDestroySurface(VkInstance vkInstance, VkSurfaceKHR vkSurface) {
	PFN_vkDestroySurfaceKHR destroySurface = NULL;
	if (sgGetInstanceProcAddr(vkInstance, "vkDestroySurfaceKHR", &(PFN_vkVoidFunction)destroySurface)) {
		destroySurface(vkInstance, vkSurface, NULL);
	}
}