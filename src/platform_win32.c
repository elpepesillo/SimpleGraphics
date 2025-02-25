#define VK_USE_PLATFORM_WIN32_KHR
#include <simple_graphics/platform.h>

PFN_vkGetInstanceProcAddr instanceProcAddr = NULL;
const char* className = "simple_window";
HINSTANCE hInstance = NULL;

uint32_t INSTANCE_EXTENSIONS_COUNT = 2;
const char* INSTANCE_EXTENSIONS[] = {
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
	VK_KHR_SURFACE_EXTENSION_NAME
};

struct SGwindow {
	bool exists;
	HWND hWnd;
};

static LRESULT CALLBACK sgWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	struct SGwindow* ptrWindow = (struct SGwindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (msg) {

	case WM_NCCREATE: {
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		ptrWindow = pCreate->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ptrWindow);
		break;
	}

	case WM_CLOSE: {
		DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY: {
		ptrWindow->exists = false;
		PostQuitMessage(0);
		break;
	}

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

PFN_vkVoidFunction sgGetVulkanInstanceFunction(VkInstance vkInstance, const char* pName) {
	if (instanceProcAddr == NULL) {
		HMODULE vulkanDll = LoadLibrary("vulkan-1.dll");
		if (vulkanDll == NULL) {
			return NULL;
		}
		instanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vulkanDll, "vkGetInstanceProcAddr");
	}
	return instanceProcAddr(vkInstance, pName);
}

SGwindow* sgCreateWindow(const char* title, int width, int height, bool resizable) {
	if (hInstance == NULL) {
		hInstance = GetModuleHandle(NULL);
		if (hInstance == NULL) {
			return NULL;
		}

		const WNDCLASSEX wc = {
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_OWNDC,
		.lpfnWndProc = sgWndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = LoadIcon(NULL, IDI_APPLICATION),
		.hCursor = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground = 0,
		.lpszClassName = className,
		.hIconSm = LoadIcon(NULL, IDI_APPLICATION)
		};

		if (!RegisterClassEx(&wc)) {
			return NULL;
		}
	}

	struct SGwindow* ptrWindow = malloc(sizeof(struct SGwindow));
	if (!ptrWindow) {
		return NULL;
	}

	RECT screenRect;
	GetWindowRect(GetDesktopWindow(), &screenRect);

	DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION;
	if (resizable) {
		style |= WS_MAXIMIZEBOX | WS_THICKFRAME;
	}

	ptrWindow->hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		className,
		title,
		style,
		(screenRect.right - screenRect.left - width) / 2,
		(screenRect.bottom - screenRect.top - height) / 2,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		ptrWindow
	);

	if (ptrWindow->hWnd == NULL) {
		free(ptrWindow);
		return NULL;
	}
	ShowWindow(ptrWindow->hWnd, SW_SHOW);
	return ptrWindow;
}

void sgDestroyWindow(SGwindow* window) {
	if (window == NULL) {
		return;
	}
	DestroyWindow(window->hWnd);
}

bool sgWindowExists(SGwindow* window) {
	if (!window->exists) {
		free(window);
		return true;
	}
	return false;
}

void sgProcessWindow(SGwindow* window) {
	if (window == NULL) {
		return;
	}
	MSG msg;
	while (GetMessage(&msg, window->hWnd, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

VkResult sgVkCreateSurface(VkInstance vkInstance, SGwindow* window, VkSurfaceKHR* vkSurface) {
	if (hInstance == NULL || window == NULL) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	
	const VkWin32SurfaceCreateInfoKHR createInfo = {
		.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
		.pNext = NULL,
		.flags = 0,
		.hinstance = hInstance,
		.hwnd = window->hWnd
	};

	PFN_vkCreateWin32SurfaceKHR createSurface = (PFN_vkCreateWin32SurfaceKHR)sgGetVulkanInstanceFunction(vkInstance, "vkCreateWin32SurfaceKHR");
	return createSurface(vkInstance, &createInfo, NULL, vkSurface);
}