
function( add_example ARG_NAME )
    set( EXAMPLE_DIR "${PROJECT_SOURCE_DIR}/examples/${ARG_NAME}")
#    set( SOURCES "")
    file( GLOB SOURCES ${EXAMPLE_DIR}/*.cpp ${EXAMPLE_DIR}/*.h)
    add_executable( example-${ARG_NAME} ${SOURCES})

endfunction()


set(
    EXAMPLES
    00-helloworld
)

foreach(EXAMPLE ${EXAMPLES})
    add_example(${EXAMPLE})
endforeach()
