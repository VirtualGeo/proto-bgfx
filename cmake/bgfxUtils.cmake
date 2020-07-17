#project(bgfxUtils)


set(PROJECT_NAME bgfxUtils)

set(BGFX_UTILS_DIR "${PROJECT_SOURCE_DIR}/external/bgfx/examples/common/")

file (GLOB_RECURSE SOURCES ${BGFX_UTILS_DIR}/*.cpp ${BGFX_UTILS_DIR}/*.h)
add_library(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} PUBLIC
    src
    BGFX::BGFX
    meshoptimizer
)

#target_compile_options(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/W3>)
#target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS> $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>)

