# cmake_minimum_required(VERSION 3.0)
# project(src VERSION 1.0 LANGUAGES CXX)

foreach(SHADER ${IN_SHADERS})
    compile_shader(${SHADER})
endforeach()

# set(SOURCE main.cpp)
file(GLOB_RECURSE SOURCE *.cpp *.h)

# set(IN_SHADERS_FULL_PATH )
# foreach(IN_SHADER ${IN_SHADERS})
#     list(APPEND IN_SHADERS_FULL_PATH ${SHADER_SRC_DIR}/${IN_SHADER} ${OUT_SHADERS})
# endforeach()

#set(VARYING_DEF_SHADER ${CMAKE_SOURCE_DIR}/shaders/src/varying.def.sc)
add_executable(${PROJECT_NAME} ${SOURCE} ${OUT_SHADERS} ${VARYING_DEF_SHADER})

# set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#message("BGFX_LIBRARY_RELEASE" ${BGFX_LIBRARY_RELEASE})

find_package(bgfx REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${BGFX_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PUBLIC __STDC_LIMIT_MACROS __STDC_FORMAT_MACROS __STDC_CONSTANT_MACROS)

# if (UNIX)
#message(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG}) # -g
#message(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE}) # -03 -DNDEBUG
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
     set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall -pedantic -Wextra -O0 -Wfatal-errors")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    #    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wpedantic -Werror -DDEBUG -O0 -g -Wfatal-errors)
#    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wpedantic -Wextra -DDEBUG -O0 -g -Wfatal-errors)
    # endif()
    # elseif (MSVC)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # target_compile_options(${PROJECT_NAME} PUBLIC /Wall)
#    target_compile_options(${PROJECT_NAME} PUBLIC /W4 /WX)
endif()
#target_compile_options(${PROJECT_NAME} PRIVATE
#    "$<$<CONFIG:Debug>:${CMAKE_CXX_FLAGS_DEBUG}>"
#    "$<$<CONFIG:Release>:${CMAKE_CXX_FLAGS_RELEASE}>"
#    )

set(SYSTEM_DEPEND_LIBS "")
if (UNIX)

    find_package(X11 REQUIRED)
    #        set(OpenGL_GL_PREFERENCE "GLVND")
    set(OpenGL_GL_PREFERENCE "LEGACY")
    find_package(OpenGL REQUIRED)

    set(SYSTEM_DEPEND_LIBS ${SYSTEM_DEPEND_LIBS} ${X11_LIBRARIES} ${OPENGL_LIBRARIES})
    #else()
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${SYSTEM_DEPEND_LIBS}
    #    ${X11_LIBRARIES}
    #    ${OPENGL_LIBRARIES}
    $<$<CONFIG:Debug>:${BGFX_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${BGFX_LIBRARY_RELEASE}>
    $<$<CONFIG:Debug>:${BIMG_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${BIMG_LIBRARY_RELEASE}>
    $<$<CONFIG:Debug>:${BX_LIBRARY_DEBUG}> # BX not before BIMG
    $<$<CONFIG:Release>:${BX_LIBRARY_RELEASE}>
    $<$<CONFIG:Debug>:${ASTCCODEC_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${ASTCCODEC_LIBRARY_RELEASE}>
    glfw
    tinyobjloader
    stb
    )

#        OpenGL::GL
#        ${X11_LIBRARIES}
#    )

