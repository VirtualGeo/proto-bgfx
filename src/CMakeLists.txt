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

if(GUI_QT)
#    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    file(GLOB_RECURSE UI_SOURCE *.ui)
#    add_executable(${PROJECT_NAME} ${UI_SOURCE})
endif()


#set(VARYING_DEF_SHADER ${CMAKE_SOURCE_DIR}/shaders/src/varying.def.sc)
add_executable(${PROJECT_NAME}
    ${SOURCE} ${OUT_SHADERS} ${LIB_SHADER} ${UI_SOURCE}
)

find_package(bgfx REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${BGFX_INCLUDE_DIRS}
    ${EXTERNAL_INCLUDE_DIRS} # libdl
    ${SRC_DIR}
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
#    $<INSTALL_INTERFACE:include>
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    __STDC_LIMIT_MACROS __STDC_FORMAT_MACROS __STDC_CONSTANT_MACROS
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall -pedantic -Wextra -O0 -Wfatal-errors")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4 /WX")
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
    $<$<CONFIG:Debug>:${BGFX_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${BGFX_LIBRARY_RELEASE}>
    $<$<CONFIG:Debug>:${BIMG_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${BIMG_LIBRARY_RELEASE}>
    $<$<CONFIG:Debug>:${BX_LIBRARY_DEBUG}> # BX not before BIMG
    $<$<CONFIG:Release>:${BX_LIBRARY_RELEASE}>
    $<$<CONFIG:Debug>:${ASTCCODEC_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${ASTCCODEC_LIBRARY_RELEASE}>
    tinyobjloader
    ${CMAKE_DL_LIBS} #dlclose glibc
)

if (GUI_GLFW)
    target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
elseif(GUI_QT)
#if(GUI_QT)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
endif()
