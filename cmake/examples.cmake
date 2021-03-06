find_package(bgfx REQUIRED)

function( add_example ARG_NAME )
    set( EXAMPLE_DIR "${PROJECT_SOURCE_DIR}/examples/${ARG_NAME}")
    set( EXAMPLE_PROJECT_NAME example-${ARG_NAME})
    #    set( SOURCES "")
    file( GLOB SOURCES ${EXAMPLE_DIR}/*.cpp ${EXAMPLE_DIR}/*.h)
    add_executable(${EXAMPLE_PROJECT_NAME} ${SOURCES})
#    target_include_directories(${EXAMPLE_PROJECT_NAME} PUBLIC
#        ${EXTERNAL_INCLUDE_DIRS}
#        ${SRC_DIR}
#        )

    target_link_libraries(${EXAMPLE_PROJECT_NAME} PUBLIC
        src
        bgfxUtils
    )

endfunction()


set(EXAMPLES
    00-helloworld
    01-drawQuad
    02-sponza
    03-branchingTests
    04-nothingToDo
    05-shadow
    06-sanMiguel-fastBlinn
    07-sanMiguel
)

foreach(EXAMPLE ${EXAMPLES})
    if (${EXAMPLE} STREQUAL "06-sanMiguel")
        add_definitions("-DFAST_BLINN")
    endif()
    add_example(${EXAMPLE})
endforeach()
