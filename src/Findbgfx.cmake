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
    

set(BGFX_INCLUDE_DIRS ${BGFX_INCLUDE_DIR}; ${BGFX_INCLUDE_DIR}/compat/msvc)

    # message(FATAL_ERROR "ERROR: ${BGFX_INCLUDE_DIR}")
    


find_library(BGFX_LIBRARY_DEBUG NAMES bgfxd
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

find_library(BX_LIBRARY_DEBUG NAMES bxd
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
find_library(ASTCCODEC_LIBRARY_DEBUG NAMES astc-codecd
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

find_library(BX_LIBRARY_RELEASE NAMES bx
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

# set (BGFX_LIBRARIES_DEBUG "${BX_LIBRARY_DEBUG};${BGFX_LIBRARY_DEBUG}")
# set (BGFX_LIBRARIES_RELEASE "${BX_LIBRARY_RELEASE};${BGFX_LIBRARY_RELEASE}")


find_package_handle_standard_args(BGFX DEFAULT_MSG BGFX_LIBRARY_RELEASE BGFX_INCLUDE_DIR)
mark_as_advanced(BGFX_FOUND BGFX_INCLUDE_DIR BGFX_LIBRARY_RELEASE)


# message(FATAL_ERROR "toto: ${BGFX_FOUND}")
if (NOT BGFX_FOUND) 
    message(FATAL_ERROR "could not find bgfx, make sure you have set BGFX_ROOT")
    endif()