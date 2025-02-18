#include <simple_graphics/graphics.h>
#include <simple_graphics/platform.h>
#include <stdlib.h>

VkResult sgVkCreateInstance(VkInstance* vkInstance) {
	const VkApplicationInfo appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = NULL,
		.pApplicationName = NULL,
		.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
		.pEngineName = NULL,
		.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
		.apiVersion = VK_MAKE_API_VERSION(0, 1, 4, 0)
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
	if (!sgGetInstanceFunctionPointer(NULL, "vkCreateInstance", &(PFN_vkVoidFunction)createInstance)) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	return createInstance(&createInfo, NULL, vkInstance);
}

void sgVkDestroyInstance(VkInstance vkInstance) {
	PFN_vkDestroyInstance destroyInstance = NULL;
	if (!sgGetInstanceFunctionPointer(vkInstance, "vkDestroyInstance", &(PFN_vkVoidFunction)destroyInstance)) {
		destroyInstance(vkInstance, NULL);
	}
}

void sgVkDestroySurface(VkInstance vkInstance, VkSurfaceKHR vkSurface) {
	PFN_vkDestroySurfaceKHR destroySurface = NULL;
	if (sgGetInstanceFunctionPointer(vkInstance, "vkDestroySurfaceKHR", &(PFN_vkVoidFunction)destroySurface)) {
		destroySurface(vkInstance, vkSurface, NULL);
	}
}

VkResult sgVkGetRecommendedGpu(VkInstance vkInstance, VkSurfaceKHR vkSurface, uint32_t* pQueueFamilyIndex, VkPhysicalDevice* pGpu) {
	PFN_vkEnumeratePhysicalDevices enumerateGpus = NULL;
	if (!sgGetInstanceFunctionPointer(vkInstance, "vkEnumeratePhysicalDevices", &(PFN_vkVoidFunction)enumerateGpus)) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	uint32_t gpuCount = 0;
	if (enumerateGpus(vkInstance, &gpuCount, NULL) != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	if (gpuCount == 0) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	VkPhysicalDevice* gpus = malloc(sizeof(VkPhysicalDevice) * gpuCount);
	if (!gpus) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	if (enumerateGpus(vkInstance, &gpuCount, gpus) != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	PFN_vkGetPhysicalDeviceProperties getGpuProperties = NULL;
	if (!sgGetInstanceFunctionPointer(vkInstance, "vkGetPhysicalDeviceProperties", &(PFN_vkVoidFunction)getGpuProperties)) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	PFN_vkGetPhysicalDeviceQueueFamilyProperties getGpuQueueFamilies = NULL;
	if (!sgGetInstanceFunctionPointer(vkInstance, "vkGetPhysicalDeviceQueueFamilyProperties", &(PFN_vkVoidFunction)getGpuQueueFamilies)) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR getGpuSurfaceSupport = NULL;
	if (!sgGetInstanceFunctionPointer(vkInstance, "vkGetPhysicalDeviceSurfaceSupportKHR", &(PFN_vkVoidFunction)getGpuSurfaceSupport)) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	VkPhysicalDevice choosenGpu = VK_NULL_HANDLE;
	uint32_t queueFamilyIndex = 0;
	for (uint32_t i = 0; i < gpuCount; i++) {
		VkPhysicalDevice gpu = gpus[i];

		VkPhysicalDeviceProperties gpuProperties;
		getGpuProperties(gpu, &gpuProperties);

		if (gpuProperties.apiVersion < VK_API_VERSION_1_4) {
			continue;
		}

		uint32_t gpuQueueFamilyCount = 0;
		getGpuQueueFamilies(gpu, &gpuQueueFamilyCount, NULL);

		VkQueueFamilyProperties* gpuQueueFamilies = malloc(sizeof(VkQueueFamilyProperties) * gpuQueueFamilyCount);
		if (!gpuQueueFamilies) {
			continue;
		}
		getGpuQueueFamilies(gpu, &gpuQueueFamilyCount, gpuQueueFamilies);

		bool gpuHasSuitableQueueFamily = false;
		for (uint32_t j = 0; j < gpuQueueFamilyCount; j++) {
			VkQueueFamilyProperties queueFamily = gpuQueueFamilies[j];

			VkBool32 queueFamilySupportsSurface = VK_FALSE;
			if (getGpuSurfaceSupport(gpu, j, vkSurface, &queueFamilySupportsSurface) != VK_SUCCESS) {
				continue;
			}
			bool queueFamilySupportsGraphics = queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT;

			if (queueFamilySupportsGraphics && queueFamilySupportsSurface) {
				gpuHasSuitableQueueFamily = true;
				queueFamilyIndex = j;
				choosenGpu = gpu;
				break;
			}

		}
		free(gpuQueueFamilies);

		// Prioritize dedicated GPUs
		bool gpuIsDedicated = gpuProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		if (gpuHasSuitableQueueFamily && gpuIsDedicated) {
			break;
		}

	}
	free(gpus);

	bool noSuitableGpuFound = (choosenGpu == VK_NULL_HANDLE);
	if (noSuitableGpuFound) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	return VK_SUCCESS;
}