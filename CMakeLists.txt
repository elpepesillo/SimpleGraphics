# CMakeList.txt : CMake project for SimpleGraphics, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project (
	SimpleGraphics
	VERSION 1.0
	DESCRIPTION "Do graphics easily"
	LANGUAGES C
)

set(CMAKE_C_STANDARD 99)

find_package(Vulkan REQUIRED)

add_library(
	${PROJECT_NAME}
	"include/simple_graphics/graphics.h"
	"include/simple_graphics/platform.h"
	"src/graphics.c"
)

if (WIN32)
	target_sources(${PROJECT_NAME} PRIVATE "src/platform_win32.c")
	target_link_libraries(${PROJECT_NAME} PRIVATE User32)
endif()


target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		${Vulkan_INCLUDE_DIRS}
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)