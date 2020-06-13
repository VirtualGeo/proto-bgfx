cmake_minimum_required(VERSION 3.5)
project(proto-bgfx VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14) # need by std::make_unique
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")
set(SHADER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/bgfx/shaders/src)
add_definitions(-DSHADER_INCLUDE_DIR=\"${SHADER_INCLUDE_DIR}\")
add_definitions(-DBGFX_ROOT=\"${BGFX_ROOT}\")


# ------------------------ USER CONFIGURATIONS ----------------------
#set(BGFX_ROOT <PATH_IN_YOUR_SYSTEM>/bgfx-install) // YOU MUST SET BGFX_ROOT TO COMPILE <----- ATTENTION
# vscode: settings.json
#{
#   "cmake.configureSettings": {
#   "BGFX_ROOT": "D:/bgfx.cmake/bgfx-install",
#   },
#}
# visual studio
# "variables": [
#        {
#          "name": "BGFX_ROOT",
#          "value": "d:/bgfx.cmake/bgfx-install/",
#          "type": "PATH"
#        }
#      ]

add_definitions(-DAUTO_GENERATE_BIN_MODEL)
add_definitions(-DMODEL_LOADER_INFO)
set(USE_SHADERC_EXECUTABLE 1)

# select graphic user interface
#    set(GUI_QT 1) #default: GLFW


# -------------------------------------------------------------------
if (USE_SHADERC_EXECUTABLE)
    add_definitions(-DUSE_SHADERC_EXECUTABLE) #avoid depend of shaderclib (to long linkage on build, shaderclibd.a is too big)
endif()

if (GUI_QT)
    add_definitions(-DGUI_QT)
else()
    add_definitions(-DGUI_GLFW)
    set(GUI_GLFW 1)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


# -------------------------------- EXTERNAL LIBS
if (GUI_GLFW)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(modules/glfw)
    #    target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
endif()
set(EXTERNAL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/external)
add_subdirectory(modules/tinyobjloader)
#add_subdirectory(modules/optick)

# add_subdirectory(Modules/stb)
#add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/stb/stb.cpp)
#target_include_directories(stb SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/)
set (SRC_DIR ${PROJECT_SOURCE_DIR}/src/)
add_subdirectory(src)
