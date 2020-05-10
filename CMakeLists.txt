    cmake_minimum_required(VERSION 3.5)
    project(proto-bgfx VERSION 1.0 LANGUAGES CXX)

    # set(CMAKE_INCLUDE_CURRENT_DIR ON)

    #set(CMAKE_AUTOUIC ON)
    #set(CMAKE_AUTOMOC ON)
    #set(CMAKE_AUTORCC ON)

    set(CMAKE_CXX_STANDARD 14) # need by std::make_unique
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    #option(AUTO_GENERATE_BIN_MODEL "Generate bin file of model for fast loading" ON)

    #message("CMAKE_SOURCE_DIR = " ${CMAKE_SOURCE_DIR})
    #set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
    add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")

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

    #if (AUTO_GENERATE_BIN_MODEL)
    add_definitions(-DAUTO_GENERATE_BIN_MODEL)
    add_definitions(-DMODEL_LOADER_INFO)
    #endif()

    # select graphic user interface
#    set(GUI_QT 1) #default: GLFW

    # -------------------------------------------------------------------
    if (GUI_QT)
        add_definitions(-DGUI_QT)
    else()
        add_definitions(-DGUI_GLFW)
        set(GUI_GLFW 1)
    endif()


    # message("BGFX_ROOT = " ${BGFX_ROOT})
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
    # set(BGFX_ROOT /home/gauthier/tmp/bgfx.cmake/bgfx-install)

    # find_package(glfw3 3.3 REQUIRED)
    # ------------------------------ GLFW
    if (GUI_GLFW)
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(modules/glfw)
    #    target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
    endif()


    # -------------------------------- EXTERNAL LIBS
    set(EXTERNAL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/external)
    add_subdirectory(modules/tinyobjloader)
    # add_subdirectory(Modules/glm)

    # add_subdirectory(Modules/stb)
    #add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/stb/stb.cpp)
    #target_include_directories(stb SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/)
#    set(SHADER_SOURCES)

#    set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)
#    add_subdirectory(shaders)
    set (SRC_DIR ${PROJECT_SOURCE_DIR}/src/)
    add_subdirectory(src)
