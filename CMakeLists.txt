cmake_minimum_required(VERSION 3.5)
project(proto-bgfx VERSION 1.0 LANGUAGES CXX)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#option(AUTO_GENERATE_BIN_MODEL "Generate bin file of model for fast loading" ON)

#message("CMAKE_SOURCE_DIR = " ${CMAKE_SOURCE_DIR})
#set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")

# ------------------------ USER CONFIGURATIONS ----------------------
#set(BGFX_ROOT <PATH_IN_YOUR_SYSTEM>/bgfx-install) // YOU MUST SET BGFX_ROOT TO COMPILE <----- ATTENTION

#if (AUTO_GENERATE_BIN_MODEL)
add_definitions(-DAUTO_GENERATE_BIN_MODEL)
#endif()

# select graphic user interface
set(GUI_QT 1)
#set(GUI_GLFW 1) #default

# default renderer linux:opengl, window:DirectX11
# set(RENDERER_Direct3D9 1)
# set(RENDERER_Direct3D11 1) #default on win
# set(RENDERER_Direct3D12 1)
# set(RENDERER_OpenGL 1) #default on linux
# set(RENDERER_OpenGLES 1) #only on android
# set(RENDERER_Vulkan 1)
# set(RENDERER_Metal 1) #only on ios
# -------------------------------------------------------------------
if (GUI_QT)
    add_definitions(-DGUI_QT)
else()
    add_definitions(-DGUI_GLFW)
    set(GUI_GLFW 1)
endif()

if(RENDERER_Direct3D9)
    add_definitions(-DRENDERER_Direct3D9)
elseif(RENDERER_Direct3D11)
    add_definitions(-DRENDERER_Direct3D11)
elseif(RENDERER_Direct3D12)
    add_definitions(-DRENDERER_Direct3D12)
elseif (RENDERER_OpenGL)
    add_definitions(-DRENDERER_OpenGL)
elseif(RENDERER_OpenGLES)
    add_definitions(-DRENDERER_OpenGLES)
elseif(RENDERER_Vulkan)
    add_definitions(-DRENDERER_Vulkan)
elseif(RENDERER_Metal)
    add_definitions(-DRENDERER_Metal)
else()
    add_definitions(-DRENDERER_Auto)
    set(RENDERER_Auto 1) #for shader.cmake
endif()

# message("BGFX_ROOT = " ${BGFX_ROOT})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# set(BGFX_ROOT /home/gauthier/tmp/bgfx.cmake/bgfx-install)

# find_package(glfw3 3.3 REQUIRED)
# ------------------------------ GLFW
#if (GUI_GLFW)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(modules/glfw)
#    target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
#endif()



# -------------------------------- SHADERS
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)
# MAIN_DEPENDENCY ${SHADER_DIR}/src/${SHADER}
set(SHADER_SRC_DIR ${SHADER_DIR}/src)
# set(SHADERS test.vert.sc)

set(IN_SHADERS
#    cubes.vert.sc
#    cubes.frag.sc
    material.vert.sc
    material.frag.sc
    rendered.vert.sc
    rendered.frag.sc
    normal.vert.sc
    normal.frag.sc
    # blinnphong_vert.sc
    # blinnphong_frag.sc
    # mesh_vert.sc
    # mesh_frag.sc
    )

include (${CMAKE_MODULE_PATH}/shader.cmake)
#include(${CMAKE_MODULE_PATH}/Findbgfx.cmake)
# file(GLOB shaders shaders/src/*.sc)

# -------------------------------- EXTERNAL LIBS
set(EXTERNAL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/external)
add_subdirectory(modules/tinyobjloader)
# add_subdirectory(Modules/glm)

# add_subdirectory(Modules/stb)
#add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/stb/stb.cpp)
#target_include_directories(stb SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/)

set (SRC_DIR ${PROJECT_SOURCE_DIR}/src/)
add_subdirectory(src)
