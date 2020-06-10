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


#set(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH False)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False) # not include /usr/lib/libglslang.so on linux
find_library(BGFX_LIBRARY_DEBUG NAMES bgfxd PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
find_library(BGFX_LIBRARY_RELEASE NAMES bgfx PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
find_library(BIMG_LIBRARY_DEBUG NAMES bimgd PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
find_library(BIMG_LIBRARY_RELEASE NAMES bimg PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
find_library(BX_LIBRARY_DEBUG NAMES bxd PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
find_library(BX_LIBRARY_RELEASE NAMES bx PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
find_library(ASTCCODEC_LIBRARY_DEBUG NAMES astc-codecd PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
find_library(ASTCCODEC_LIBRARY_RELEASE NAMES astc-codec PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)

if(NOT USE_SHADERC_EXECUTABLE)
    find_library(SHADERC_LIBRARY_DEBUG NAMES shaderclibd PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    find_library(SHADERC_LIBRARY_RELEASE NAMES shaderclib PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    find_library(FCPP_LIBRARY_DEBUG NAMES fcppd PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    find_library(FCPP_LIBRARY_RELEASE NAMES fcpp PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    find_library(GLSLANG_LIBRARY_DEBUG NAMES glslangd PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    find_library(GLSLANG_LIBRARY_RELEASE NAMES glslang PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    #find_library(GLSLANG_LIBRARY_RELWITHDEBINFO NAMES glslang PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib NO_DEFAULT_PATH)
    find_library(GLSL_OPTIMIZER_LIBRARY_DEBUG NAMES glsl-optimizerd PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    find_library(GLSL_OPTIMIZER_LIBRARY_RELEASE NAMES glsl-optimizer PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    find_library(SPIRV_CROSS_LIBRARY_DEBUG NAMES spirv-crossd PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    find_library(SPIRV_CROSS_LIBRARY_RELEASE NAMES spirv-cross PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    find_library(SPIRV_TOOLS_LIBRARY_DEBUG NAMES spirv-toolsd PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
    find_library(SPIRV_TOOLS_LIBRARY_RELEASE NAMES spirv-tools PATHS /usr/local ${BGFX_ROOT} PATH_SUFFIXES lib)
endif()

if (USE_SHADERC_EXECUTABLE)
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
else()
    find_package_handle_standard_args(bgfx
        REQUIRED_VARS
            BGFX_INCLUDE_DIR
            BX_LIBRARY_RELEASE BX_LIBRARY_DEBUG
            BIMG_LIBRARY_RELEASE BIMG_LIBRARY_DEBUG
            BGFX_LIBRARY_RELEASE BGFX_LIBRARY_DEBUG
            SHADERC_LIBRARY_RELEASE SHADERC_LIBRARY_DEBUG
            FCPP_LIBRARY_DEBUG FCPP_LIBRARY_RELEASE
            GLSLANG_LIBRARY_DEBUG GLSLANG_LIBRARY_RELEASE
            GLSL_OPTIMIZER_LIBRARY_DEBUG GLSL_OPTIMIZER_LIBRARY_RELEASE
            SPIRV_CROSS_LIBRARY_DEBUG SPIRV_CROSS_LIBRARY_RELEASE
            SPIRV_TOOLS_LIBRARY_DEBUG SPIRV_TOOLS_LIBRARY_RELEASE
        FAIL_MESSAGE
            "Could not find all bgfx libraries. Please check BGFX_ROOT has been properly set, and both debug and release versions of bgfx have been compiled."
    )

    mark_as_advanced(BGFX_FOUND
        BGFX_INCLUDE_DIR
        BX_LIBRARY_RELEASE BX_LIBRARY_DEBUG
        BIMG_LIBRARY_RELEASE BIMG_LIBRARY_DEBUG
        BGFX_LIBRARY_RELEASE BGFX_LIBRARY_DEBUG
        SHADERC_LIBRARY_RELEASE SHADERC_LIBRARY_DEBUG
        FCPP_LIBRARY_DEBUG FCPP_LIBRARY_RELEASE
        GLSLANG_LIBRARY_DEBUG GLSLANG_LIBRARY_RELEASE
        GLSL_OPTIMIZER_LIBRARY_DEBUG GLSL_OPTIMIZER_LIBRARY_RELEASE
        SPIRV_CROSS_LIBRARY_DEBUG SPIRV_CROSS_LIBRARY_RELEASE
        SPIRV_TOOLS_LIBRARY_DEBUG SPIRV_TOOLS_LIBRARY_RELEASE
    )
endif()


if(BGFX_FOUND)
    if(NOT TARGET BGFX::BGFX)
        add_library(BGFX::BGFX INTERFACE IMPORTED)
        set_property(TARGET BGFX::BGFX PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BGFX_INCLUDE_DIR} $<$<CXX_COMPILER_ID:MSVC>:${BGFX_INCLUDE_DIR}/compat/msvc>)
        if (USE_SHADERC_EXECUTABLE)
            set_property(TARGET BGFX::BGFX PROPERTY INTERFACE_LINK_LIBRARIES
                $<$<CONFIG:Debug>:${BGFX_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${BIMG_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${BX_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${ASTCCODEC_LIBRARY_DEBUG}>
                $<$<CONFIG:Release>:${BGFX_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${BIMG_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${BX_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${ASTCCODEC_LIBRARY_RELEASE}>

    #            $<$<CONFIG:RelWithDebInfo>:${SHADERC_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${BGFX_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${BIMG_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${BX_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${ASTCCODEC_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${FCPP_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${GLSLANG_LIBRARY_RELEASE}>
    ##            $<$<CONFIG:RelWithDebInfo>:${GLSLANG_LIBRARY_RELWITHDEBINFO}>
    #            $<$<CONFIG:RelWithDebInfo>:${GLSL_OPTIMIZER_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${SPIRV_CROSS_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${SPIRV_TOOLS_LIBRARY_RELEASE}>
                )
        else()
            set_property(TARGET BGFX::BGFX PROPERTY INTERFACE_LINK_LIBRARIES
                $<$<CONFIG:Debug>:${SHADERC_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${BGFX_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${BIMG_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${BX_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${ASTCCODEC_LIBRARY_DEBUG}>
                $<$<CONFIG:Release>:${SHADERC_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${BGFX_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${BIMG_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${BX_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${ASTCCODEC_LIBRARY_RELEASE}>

                $<$<CONFIG:Debug>:${FCPP_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${GLSLANG_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${GLSL_OPTIMIZER_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${SPIRV_CROSS_LIBRARY_DEBUG}>
                $<$<CONFIG:Debug>:${SPIRV_TOOLS_LIBRARY_DEBUG}>
                $<$<CONFIG:Release>:${FCPP_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${GLSLANG_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${GLSL_OPTIMIZER_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${SPIRV_CROSS_LIBRARY_RELEASE}>
                $<$<CONFIG:Release>:${SPIRV_TOOLS_LIBRARY_RELEASE}>

    #            $<$<CONFIG:RelWithDebInfo>:${SHADERC_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${BGFX_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${BIMG_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${BX_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${ASTCCODEC_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${FCPP_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${GLSLANG_LIBRARY_RELEASE}>
    ##            $<$<CONFIG:RelWithDebInfo>:${GLSLANG_LIBRARY_RELWITHDEBINFO}>
    #            $<$<CONFIG:RelWithDebInfo>:${GLSL_OPTIMIZER_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${SPIRV_CROSS_LIBRARY_RELEASE}>
    #            $<$<CONFIG:RelWithDebInfo>:${SPIRV_TOOLS_LIBRARY_RELEASE}>
                )
        endif()

        set_property(TARGET BGFX::BGFX PROPERTY INTERFACE_COMPILE_DEFINITIONS
            $<$<CXX_COMPILER_ID:MSVC>:__STDC_LIMIT_MACROS>
            $<$<CXX_COMPILER_ID:MSVC>:__STDC_FORMAT_MACROS>
            $<$<CXX_COMPILER_ID:MSVC>:__STDC_CONSTANT_MACROS>)
    endif()
else()
    message(FATAL_ERROR "could not find bgfx, make sure you have set BGFX_ROOT")
endif()

# ----------------------------------------------- SHADER COMPILATION
# Shader compile utils
function(GET_SHADER_TYPE IN_TYPE OUT_TYPE)
    if("${IN_TYPE}" STREQUAL "vert")
        set(${OUT_TYPE} "vertex" PARENT_SCOPE)
    elseif("${IN_TYPE}" STREQUAL "frag")
        set(${OUT_TYPE} "fragment" PARENT_SCOPE)
    elseif("${IN_TYPE}" STREQUAL "geom")
        set(${OUT_TYPE} "geometry" PARENT_SCOPE)
    else()
        message(FATAL_ERROR "Shader type ${IN_TYPE} to handle")
    endif()
endfunction()


function(GET_SHADER_INFOS FILENAME OUT_SHADER_NAME OUT_SHADER_TYPE OUT_BIN_SHADER_NAME OUT_OK)
    if(${FILENAME} MATCHES "^(.*/)*([a-zA-Z0-9_]+).(vert|frag|tesc|tesv|geom|comp).sc$")
        get_shader_type(${CMAKE_MATCH_3} SHADER_TYPE)
        set(${OUT_SHADER_NAME} ${CMAKE_MATCH_2} PARENT_SCOPE)
        set(${OUT_SHADER_TYPE} ${SHADER_TYPE} PARENT_SCOPE)
        file(MAKE_DIRECTORY ${SHADER_BIN_DIR}/dx11/${CMAKE_MATCH_1})
        file(MAKE_DIRECTORY ${SHADER_BIN_DIR}/glsl/${CMAKE_MATCH_1})
        file(MAKE_DIRECTORY ${SHADER_BIN_DIR}/spirv/${CMAKE_MATCH_1})
        file(MAKE_DIRECTORY ${SHADER_BIN_DIR}/metal/${CMAKE_MATCH_1})
        file(MAKE_DIRECTORY ${SHADER_BIN_DIR}/essl/${CMAKE_MATCH_1})
        set(${OUT_BIN_SHADER_NAME} ${CMAKE_MATCH_1}${CMAKE_MATCH_2}.${CMAKE_MATCH_3}.bin PARENT_SCOPE)
        set(${OUT_OK} True PARENT_SCOPE)
    else()
        message(WARNING "${FILENAME} is not a compatible name")
        set(${OUT_OK} False PARENT_SCOPE)
    endif()
endfunction(GET_SHADER_INFOS)

macro(COMPILE_SHADER_INTERNAL SHADER_IN SHADER_OUT PLATFORM PROFILE)
#    set(DEPENDS)
#    list(APPEND DEPENDS ${SHADER_IN} ${SHADER_VARYING_DEF})

    add_custom_command(
        OUTPUT ${SHADER_OUT}
        COMMAND
        ${BGFX_ROOT}/bin/shaderc
        -f ${SHADER_IN}
        -o ${SHADER_OUT}
        --platform ${PLATFORM}
        --profile ${PROFILE}
        --type ${SHADER_TYPE}
        ${PROFILE}
        -i ${SHADER_INCLUDE_DIR}
        --varyingdef ${SHADER_VARYING_DEF}
        MAIN_DEPENDENCY ${SHADER_IN}
        DEPENDS ${SHADER_DEPENDS}
        COMMENT "Compiling ${SHADER_IN} to ${SHADER_OUT}"
    )
endmacro()

function(COMPILE_SHADER SHADER SHADER_SRC_DIR SHADER_BIN_DIR OUT_SHADERS)
#    get_filename_component(SHADER_FILENAME ${SHADER} NAME)
    file(RELATIVE_PATH SHADER_RELATIVE ${SHADER_SRC_DIR} ${SHADER})
#    message("RELATIVE_PATH: " ${SHADER_RELATIVE})
    get_shader_infos(${SHADER_RELATIVE} SHADER_NAME SHADER_TYPE BIN_SHADER_NAME OK)
#    get_shader_infos(${SHADER_FILENAME} SHADER_NAME SHADER_TYPE BIN_SHADER_NAME OK)
#        message("OUT_SHADER_NAME:" ${SHADER_NAME})
#        message("OUT_BIN_SHADER_NAME:" ${BIN_SHADER_NAME})

    if(OK)
        if(${SHADER_TYPE} STREQUAL "vertex")
            set(DXPROFILE "vs_5_0")
            set(GLPROFILE "120")
        elseif(${SHADER_TYPE} STREQUAL "fragment")
            set(DXPROFILE "ps_5_0")
            set(GLPROFILE "120")
        elseif(${SHADER_TYPE} STREQUAL "compute")
            set(DXPROFILE "cs_5_0")
            set(GLPROFILE "430")
        endif()

#        if(MSVC)
#            compile_shader_internal(${SHADER_SRC_DIR}/${SHADER} ${SHADER_BIN_DIR}/dx11/${BIN_SHADER_NAME} "windows" ${DXPROFILE}) # DX11+
#            list(APPEND ${OUT_SHADERS} ${SHADER_BIN_DIR}/dx11/${BIN_SHADER_NAME})
#        endif()

#        compile_shader_internal(${SHADER_SRC_DIR}/${SHADER} ${SHADER_BIN_DIR}/glsl/${BIN_SHADER_NAME} "linux" ${GLPROFILE}) # GLSL
#        compile_shader_internal(${SHADER_SRC_DIR}/${SHADER} ${SHADER_BIN_DIR}/spirv/${BIN_SHADER_NAME} "linux" "spirv") # Vulkan
#        compile_shader_internal(${SHADER_SRC_DIR}/${SHADER} ${SHADER_BIN_DIR}/metal/${BIN_SHADER_NAME} "osx" "metal") # Metal
#        compile_shader_internal(${SHADER_SRC_DIR}/${SHADER} ${SHADER_BIN_DIR}/essl/${BIN_SHADER_NAME} "android" ${GLPROFILE}) # GLES Android

        if(MSVC)
            compile_shader_internal(${SHADER} ${SHADER_BIN_DIR}/dx11/${BIN_SHADER_NAME} "windows" ${DXPROFILE}) # DX11+
            list(APPEND ${OUT_SHADERS} ${SHADER_BIN_DIR}/dx11/${BIN_SHADER_NAME})
        endif()

        compile_shader_internal(${SHADER} ${SHADER_BIN_DIR}/glsl/${BIN_SHADER_NAME} "linux" ${GLPROFILE}) # GLSL
        compile_shader_internal(${SHADER} ${SHADER_BIN_DIR}/spirv/${BIN_SHADER_NAME} "linux" "spirv") # Vulkan
        compile_shader_internal(${SHADER} ${SHADER_BIN_DIR}/metal/${BIN_SHADER_NAME} "osx" "metal") # Metal
        compile_shader_internal(${SHADER} ${SHADER_BIN_DIR}/essl/${BIN_SHADER_NAME} "android" ${GLPROFILE}) # GLES Android

        list(APPEND ${OUT_SHADERS} ${SHADER_BIN_DIR}/glsl/${BIN_SHADER_NAME})
        list(APPEND ${OUT_SHADERS} ${SHADER_BIN_DIR}/spirv/${BIN_SHADER_NAME})
        list(APPEND ${OUT_SHADERS} ${SHADER_BIN_DIR}/metal/${BIN_SHADER_NAME})
        list(APPEND ${OUT_SHADERS} ${SHADER_BIN_DIR}/essl/${BIN_SHADER_NAME})

        set(${OUT_SHADERS} "${${OUT_SHADERS}}" PARENT_SCOPE)
    endif()
endfunction(COMPILE_SHADER)
