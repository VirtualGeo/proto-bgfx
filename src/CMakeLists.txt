project(src VERSION 1.0 LANGUAGES CXX)

find_package(bgfx REQUIRED)

# -------------------------------- SHADERS
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(SHADER_SOURCE_DIR ${SHADER_DIR}/src)


set(SHADER_VARYING_DEF ${SHADER_SOURCE_DIR}/varying.def.sc)
set(SHADER_INCLUDES
    ${SHADER_SOURCE_DIR}/light.sc
    ${SHADER_SOURCE_DIR}/material.sc
    )
file(GLOB_RECURSE SHADER_SOURCES ${SHADER_SOURCE_DIR}/*.sc)
set(SHADER_DEPENDS ${SHADER_VARYING_DEF} ${SHADER_INCLUDES})
list(REMOVE_ITEM SHADER_SOURCES ${SHADER_DEPENDS})

#set(IN_SHADERS)
##     list(APPEND IN_SHADERS_FULL_PATH ${SHADER_SRC_DIR}/${IN_SHADER} ${OUT_SHADERS})
#        get_filename_component(VAR ${SHADER_SOURCE} NAME)
#        message(${VAR})

#set(IN_SHADERS
#    material.vert.sc
#    material.frag.sc
#    rendered.vert.sc
#    rendered.frag.sc
#    normal.vert.sc
#    normal.frag.sc
#    )

set(OUT_SHADERS )
foreach(SHADER ${SHADER_SOURCES})
    compile_shader(${SHADER} ${SHADER_DIR}/src ${SHADER_DIR}/bin LOCAL_OUT_SHADERS)
    list(APPEND OUT_SHADERS ${LOCAL_OUT_SHADERS})
endforeach()

file(GLOB_RECURSE SOURCE *.cpp *.h)

if(GUI_QT)
    if(CMAKE_VERSION VERSION_LESS "3.7.0")
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
    endif()
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    file(GLOB_RECURSE UI_SOURCE *.ui)
endif()


file(GLOB_RECURSE LIB_SHADER ${CMAKE_SOURCE_DIR}/external/bgfx/shaders/src/*.sh)
file(GLOB_RECURSE SHADER_FILES ${SHADER_DIR}/*)
#add_executable(${PROJECT_NAME}
#    ${SOURCE} ${OUT_SHADERS} ${LIB_SHADER}
#    ${UI_SOURCE} ${SHADER_FILES}
#    "${CMAKE_SOURCE_DIR}/examples/04-nothingToDo/nothingToDo.cpp"
#)
add_library(${PROJECT_NAME} STATIC
    ${SOURCE} ${OUT_SHADERS} ${LIB_SHADER} ${UI_SOURCE} ${SHADER_FILES}
    )
#add_library(${PROJECT_NAME} SHARED ${SOURCE})

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
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
#    ${SYSTEM_DEPEND_LIBS}
    BGFX::BGFX
#    bgfx::bgfx
    tinyobjloader
#    meshoptimizer
#    OptickCore
    ${CMAKE_DL_LIBS} #dlclose glibc
)

target_compile_options(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/W3>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS> $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>)


if (GUI_GLFW)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
elseif(GUI_QT)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
endif()
