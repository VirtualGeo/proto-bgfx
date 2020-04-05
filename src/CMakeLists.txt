# cmake_minimum_required(VERSION 3.0)
# project(src VERSION 1.0 LANGUAGES CXX)

find_package(bgfx REQUIRED)

# -------------------------------- SHADERS
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)

set(IN_SHADERS
    cubes.vert.sc
    cubes.frag.sc
    # blinnphong_vert.sc
    # blinnphong_frag.sc
    # mesh_vert.sc
    # mesh_frag.sc
    )

set(OUT_SHADERS )
foreach(SHADER ${IN_SHADERS})
    compile_shader(${SHADER} ${SHADER_DIR}/src ${SHADER_DIR}/bin ${CMAKE_SOURCE_DIR}/external/bgfx/shaders/src LOCAL_OUT_SHADERS)
    list(APPEND OUT_SHADERS ${LOCAL_OUT_SHADERS})
endforeach()

# set(SOURCE main.cpp)
file(GLOB_RECURSE SOURCE *.cpp *.h)

# set(IN_SHADERS_FULL_PATH )
# foreach(IN_SHADER ${IN_SHADERS})
#     list(APPEND IN_SHADERS_FULL_PATH ${SHADER_SRC_DIR}/${IN_SHADER} ${OUT_SHADERS})
# endforeach()

#set(VARYING_DEF_SHADER ${CMAKE_SOURCE_DIR}/shaders/src/varying.def.sc)
add_executable(${PROJECT_NAME} ${SOURCE} ${OUT_SHADERS} ${VARYING_DEF_SHADER} ${BGFX_SHADER})

# set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#message("BGFX_LIBRARY_RELEASE" ${BGFX_LIBRARY_RELEASE})

target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_INCLUDE_DIRS})

# if (UNIX)
# message(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG}) # -g
# message(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE}) # -03 -DNDEBUG
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # default debug flags : -g
    #default release flags : -03 -DNDEBUG
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall -pedantic -Wextra -O0 -Wfatal-errors")
    #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    #    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wpedantic -Werror -DDEBUG -O0 -g -Wfatal-errors)
    #    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wpedantic -Wextra -DDEBUG -O0 -g -Wfatal-errors)
    # endif()
    # elseif (MSVC)
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
    BGFX::BGFX
    glfw
    tinyobjloader
#    stb
)

target_compile_options(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/W3>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS> $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>)


