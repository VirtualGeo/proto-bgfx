include(FindPackageHandleStandardArgs)

# find_path(BX_INCLUDE_DIR bx/bx.h
#     PATHS
#         /usr
#         /usr/local
#     ${BX_ROOT}

#     PATH_SUFFIXES
#     include
# )


find_path(BGFX_INCLUDE_DIR bgfx/bgfx.h
    PATHS
    /usr
    /usr/local
    ${BGFX_ROOT}

    PATH_SUFFIXES
    include
    )

if (UNIX)
    set(BGFX_INCLUDE_DIRS ${BGFX_INCLUDE_DIR})
elseif(MSVC)
    set(BGFX_INCLUDE_DIRS ${BGFX_INCLUDE_DIR} ${BGFX_INCLUDE_DIR}/compat/msvc)
endif()

find_library(BGFX_LIBRARY_DEBUG NAMES bgfxd
    PATHS
    /usr
    /usr/local
    ${BGFX_ROOT}

    PATH_SUFFIXES
    lib
    )
find_library(BGFX_LIBRARY_RELEASE NAMES bgfx
    PATHS
    /usr
    /usr/local
    ${BGFX_ROOT}

    PATH_SUFFIXES
    lib
    )

find_library(BIMG_LIBRARY_DEBUG NAMES bimgd
    PATHS
    /usr
    /usr/local
    ${BGFX_ROOT}

    PATH_SUFFIXES
    lib
    )
find_library(BIMG_LIBRARY_RELEASE NAMES bimg
    PATHS
    /usr
    /usr/local
    ${BGFX_ROOT}

    PATH_SUFFIXES
    lib
    )

find_library(BX_LIBRARY_DEBUG NAMES bxd
    PATHS
    /usr
    /usr/local
    ${BGFX_ROOT}

    PATH_SUFFIXES
    lib
    )
find_library(BX_LIBRARY_RELEASE NAMES bx
    PATHS
    /usr
    /usr/local
    ${BGFX_ROOT}

    PATH_SUFFIXES
    lib
    )

find_library(ASTCCODEC_LIBRARY_DEBUG NAMES astc-codecd
    PATHS
    /usr
    /usr/local
    ${BGFX_ROOT}

    PATH_SUFFIXES
    lib
    )
find_library(ASTCCODEC_LIBRARY_RELEASE NAMES astc-codec
    PATHS
    /usr
    /usr/local
    ${BGFX_ROOT}

    PATH_SUFFIXES
    lib
    )

find_package_handle_standard_args(bgfx
    REQUIRED_VARS
        BGFX_INCLUDE_DIR
        BX_LIBRARY_RELEASE BX_LIBRARY_DEBUG
        BIMG_LIBRARY_RELEASE BIMG_LIBRARY_DEBUG
        BGFX_LIBRARY_RELEASE BGFX_LIBRARY_DEBUG
    FAIL_MESSAGE
        "Could not find all bgfx libraries. Please check BGFX_ROOT has been properly set, and both debug and release versions of bgfx have been compiled."
)

mark_as_advanced(BGFX_FOUND
    BGFX_INCLUDE_DIR
    BX_LIBRARY_RELEASE BX_LIBRARY_DEBUG
    BIMG_LIBRARY_RELEASE BIMG_LIBRARY_DEBUG
    BGFX_LIBRARY_RELEASE BGFX_LIBRARY_DEBUG
)

if(BGFX_FOUND)
    if(NOT TARGET BGFX::BGFX)
        add_library(BGFX::BGFX INTERFACE IMPORTED)
        set_property(TARGET BGFX::BGFX PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BGFX_INCLUDE_DIR} $<$<CXX_COMPILER_ID:MSVC>:${BGFX_INCLUDE_DIR}/compat/msvc>)
        set_property(TARGET BGFX::BGFX PROPERTY INTERFACE_LINK_LIBRARIES
            $<$<CONFIG:Debug>:${BGFX_LIBRARY_DEBUG}>
            $<$<CONFIG:Debug>:${BIMG_LIBRARY_DEBUG}>
            $<$<CONFIG:Debug>:${BX_LIBRARY_DEBUG}>
            $<$<CONFIG:Debug>:${ASTCCODEC_LIBRARY_DEBUG}>
            $<$<CONFIG:Release>:${BGFX_LIBRARY_RELEASE}>
            $<$<CONFIG:Release>:${BIMG_LIBRARY_RELEASE}>
            $<$<CONFIG:Release>:${BX_LIBRARY_RELEASE}>
            $<$<CONFIG:Release>:${ASTCCODEC_LIBRARY_RELEASE}>)
        set_property(TARGET BGFX::BGFX PROPERTY INTERFACE_COMPILE_DEFINITIONS
            $<$<CXX_COMPILER_ID:MSVC>:__STDC_LIMIT_MACROS>
            $<$<CXX_COMPILER_ID:MSVC>:__STDC_FORMAT_MACROS>
            $<$<CXX_COMPILER_ID:MSVC>:__STDC_CONSTANT_MACROS>)
    endif()
else()
    message(FATAL_ERROR "could not find bgfx, make sure you have set BGFX_ROOT")
endif()
