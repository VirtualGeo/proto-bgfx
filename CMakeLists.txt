cmake_minimum_required(VERSION 3.0)
project(proto-bgfx VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#option(AUTO_GENERATE_BIN_MODEL "Generate bin file of model for fast loading" ON)



#message("CMAKE_SOURCE_DIR = " ${CMAKE_SOURCE_DIR})
#set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")

#if (AUTO_GENERATE_BIN_MODEL)
    add_definitions(-DAUTO_GENERATE_BIN_MODEL)
#endif()

# set(RENDERER_OpenGL 1)
# set(RENDERER_DirectX 1)
# set(RENDERER_Vulkan 1)

if (RENDERER_OpenGL)
    add_definitions(-DRENDERER_OpenGL)
    # add_definitions(-DRENDERER_DirectX 0)
elseif(RENDERER_DirectX)
        # add_definitions(-DRENDERER_OpenGL 0)
       add_definitions(-DRENDERER_DirectX)
elseif(RENDERER_Vulkan)
    add_definitions(-DRENDERER_Vulkan)
else()
    add_definitions(-DRENDERER_Auto)
endif()

# message("BGFX_ROOT = " ${BGFX_ROOT})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# set(BGFX_ROOT /home/gauthier/tmp/bgfx.cmake/bgfx-install)

# find_package(glfw3 3.3 REQUIRED)
# ------------------------------ GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)



# -------------------------------- SHADERS
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)
# MAIN_DEPENDENCY ${SHADER_DIR}/src/${SHADER}
set(SHADER_SRC_DIR ${SHADER_DIR}/src)
# set(SHADERS test.vert.sc)

set(IN_SHADERS
    cubes.vert.sc
    cubes.frag.sc
    # blinnphong_vert.sc
    # blinnphong_frag.sc
    # mesh_vert.sc
    # mesh_frag.sc
    )

include (${CMAKE_MODULE_PATH}/Shader.cmake)
# file(GLOB shaders shaders/src/*.sc)

# -------------------------------- EXTERNAL LIBS
add_subdirectory(Modules/glfw)
add_subdirectory(Modules/tinyobjloader)
# add_subdirectory(Modules/glm)

# add_subdirectory(Modules/stb)
add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/stb/stb_image.cpp)
target_include_directories(stb SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/)

add_subdirectory(src)
