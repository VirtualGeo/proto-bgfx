
cmake_minimum_required(VERSION 3.0)
project(main)
set(SOURCE main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(bgfx REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${BGFX_INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME} PUBLIC __STDC_LIMIT_MACROS __STDC_FORMAT_MACROS __STDC_CONSTANT_MACROS)


target_link_libraries(${PROJECT_NAME} PUBLIC 
    $<$<CONFIG:Debug>:${BGFX_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${BGFX_LIBRARY_RELEASE}>
    $<$<CONFIG:Debug>:${BX_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${BX_LIBRARY_RELEASE}>
    $<$<CONFIG:Debug>:${BIMG_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${BIMG_LIBRARY_RELEASE}>
    $<$<CONFIG:Debug>:${ASTCCODEC_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${ASTCCODEC_LIBRARY_RELEASE}>
)