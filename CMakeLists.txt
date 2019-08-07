cmake_minimum_required(VERSION 2.8)

project(vulkan-spy)

find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

if (WIN32)
    option(CONSOLE_MODE "Use console mode" OFF)
    if (NOT CONSOLE_MODE)
        add_definitions(-DWINDOWS_MODE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    endif()
endif()

add_executable(vulkan-spy main.cpp)
target_link_libraries(vulkan-spy ${Vulkan_LIBRARIES})
