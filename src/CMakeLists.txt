# cmake_minimum_required(VERSION 3.0)
# project(src VERSION 1.0 LANGUAGES CXX)

find_package(bgfx REQUIRED)

# -------------------------------- SHADERS
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)

set(IN_SHADERS
    material.vert.sc
    material.frag.sc
    rendered.vert.sc
    rendered.frag.sc
    normal.vert.sc
    normal.frag.sc
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

if(GUI_QT)
    if(CMAKE_VERSION VERSION_LESS "3.7.0")
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
    endif()
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    file(GLOB_RECURSE UI_SOURCE *.ui)
#    add_executable(${PROJECT_NAME} ${UI_SOURCE})
endif()


set(VARYING_DEF_SHADER ${CMAKE_SOURCE_DIR}/shaders/src/varying.def.sc)
file(GLOB_RECURSE LIB_SHADER ${CMAKE_SOURCE_DIR}/external/bgfx/shaders/src/*.sh)
add_executable(${PROJECT_NAME}
    ${SOURCE} ${OUT_SHADERS} ${LIB_SHADER} ${VARYING_DEF_SHADER} ${UI_SOURCE}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${EXTERNAL_INCLUDE_DIRS}
    ${SRC_DIR}
    )

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall -pedantic -Wextra -O0 -Wfatal-errors")
#elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4 /WX")
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DEBUG")
endif()

#---------------------------- TARGET_LINK_LIBRARIES ---------------------------
#set(SYSTEM_DEPEND_LIBS "")
if (UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${X11_LIBRARIES})

#    #        set(OpenGL_GL_PREFERENCE "GLVND")
    set(OpenGL_GL_PREFERENCE "LEGACY")
    find_package(OpenGL REQUIRED) # need by bgfx renderer_gl
    target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES})

#    set(PLATFORM_SPECIFIC_LIBS "-lpthread")
    find_package(Threads REQUIRED) #pthread_getspecific glibc
    target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})

##    set(SYSTEM_DEPEND_LIBS ${SYSTEM_DEPEND_LIBS} ${X11_LIBRARIES} ${OPENGL_LIBRARIES})
#    target_link_libraries(${PROJECT_NAME} PUBLIC ${X11_LIBRARIES} ${OPENGL_LIBRARIES})
    #else()
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
#    ${SYSTEM_DEPEND_LIBS}
#    $<$<CONFIG:Debug>:${BGFX_LIBRARY_DEBUG}>
#    $<$<CONFIG:Release>:${BGFX_LIBRARY_RELEASE}>
#    $<$<CONFIG:Debug>:${BIMG_LIBRARY_DEBUG}>
#    $<$<CONFIG:Release>:${BIMG_LIBRARY_RELEASE}>
#    $<$<CONFIG:Debug>:${BX_LIBRARY_DEBUG}> # BX not before BIMG
#    $<$<CONFIG:Release>:${BX_LIBRARY_RELEASE}>
#    $<$<CONFIG:Debug>:${ASTCCODEC_LIBRARY_DEBUG}>
#    $<$<CONFIG:Release>:${ASTCCODEC_LIBRARY_RELEASE}>
    BGFX::BGFX
    tinyobjloader
    ${CMAKE_DL_LIBS} #dlclose glibc
)

target_compile_options(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/W3>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS> $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>)


if (GUI_GLFW)
    target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
elseif(GUI_QT)
#if(GUI_QT)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
endif()
