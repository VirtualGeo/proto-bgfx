
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
if(${FILENAME} MATCHES "^([a-zA-Z0-9_]+).(vert|frag|tesc|tesv|geom|comp).sc$")
    get_shader_type(${CMAKE_MATCH_2} SHADER_TYPE)
    set(${OUT_SHADER_NAME} ${CMAKE_MATCH_1} PARENT_SCOPE)
    set(${OUT_SHADER_TYPE} ${SHADER_TYPE} PARENT_SCOPE)
    set(${OUT_BIN_SHADER_NAME} ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.bin PARENT_SCOPE)
    set(${OUT_OK} True PARENT_SCOPE)
else()
    message(WARNING "${FILENAME} is not a compatible name")
    set(${OUT_OK} False PARENT_SCOPE)
endif()
endfunction(GET_SHADER_INFOS)

# file(GLOB_RECURSE IN_SHADERS *.sc)

set(OUT_SHADERS )
set(VARYING_DEF_SHADER ${CMAKE_SOURCE_DIR}/shaders/src/varying.def.sc)
set(BGFX_SHADER ${CMAKE_SOURCE_DIR}/external/bgfx/shaders/src/bgfx_shader.sh)
set(SHADER_LIB ${CMAKE_SOURCE_DIR}/external/bgfx/shaders/src/shaderlib.sh)

macro(COMPILE_SHADER SHADER)
get_shader_infos(${SHADER} SHADER_NAME SHADER_TYPE BIN_SHADER_NAME OK)
if(OK)
    if (RENDERER_Direct3D9)
        set(PLATFORM "windows")
        if(${SHADER_TYPE} STREQUAL "vertex")
            set(PROFILE "vs_3_0")
        elseif(${SHADER_TYPE} STREQUAL "fragment")
            set(PROFILE "ps_3_0")
        # elseif(${SHADER_TYPE} STREQUAL "compute")
            # set(PROFILE "cs_5_0")
        endif()
        # set(OPTIONS "-O 3 --Werror --debug") // question : --debug why failed on running
        set(OPTIONS "-O 3 --Werror")
        set(SHADER_BIN_REP "dx9")

    elseif(RENDERER_Direct3D11 OR RENDERER_Direct3D12 OR (RENDERER_Auto AND WIN32))
        set(PLATFORM "windows")
        if(${SHADER_TYPE} STREQUAL "vertex")
            set(PROFILE "vs_5_0")
        elseif(${SHADER_TYPE} STREQUAL "fragment")
            set(PROFILE "ps_5_0")
        elseif(${SHADER_TYPE} STREQUAL "compute")
            set(PROFILE "cs_5_0")
        endif()
        set(OPTIONS "-O 3 --Werror")
        set(SHADER_BIN_REP "dx11")

    elseif(RENDERER_OpenGL OR (RENDERER_Auto AND UNIX))
        set(PLATFORM "linux")
        if(${SHADER_TYPE} STREQUAL "vertex")
            set(PROFILE "120")
        elseif(${SHADER_TYPE} STREQUAL "fragment")
            set(PROFILE "120")
        elseif(${SHADER_TYPE} STREQUAL "compute")
            set(PROFILE "430")
        endif()
        set(OPTIONS "")
        set(SHADER_BIN_REP "glsl")
    
    elseif(RENDERER_OpenGLES)
        set(PLATFORM "android")
        # set(PLATFORM "nacl") // question : nacl ?
        set(PROFIL "")
        set(OPTIONS "")
        set(SHADER_BIN_REP "essl")

    elseif(RENDERER_Vulkan)
        set(PLATFORM "linux")
        set(PROFILE "spirv")
        set(OPTIONS "")
        set(SHADER_BIN_REP "spirv")

    elseif(RENDERER_Metal)
        set(PLATFORM "osx")
        set(PROFILE "metal")
        set(OPTIONS "")
        set(SHADER_BIN_REP "metal")

    # elseif(RENDERER_Auto)
    #     if(WIN32))
    #     elseif(UNIX)
    #     else()
    #         message(FATAL_ERROR "unknown Operating System")
    #     endif()

    else()
        message(FATAL_ERROR "[shader.cmake] unknown RENDERER type")
    endif()



    if("${PLATFORM}" STREQUAL "")
        message(FATAL_ERROR "[shader.cmake] PLATFORM not defined")
    endif()


    if("${PROFILE}" STREQUAL "")
            # message ("###### SHADER ###### Compiling '${SHADER_TYPE}', shader '${SHADER_NAME}', profile '${PROFILE}', platform '${PLATFORM}', options '${OPTIONS}', binDir '${SHADER_BIN_REP}")
        add_custom_command(
            OUTPUT ${SHADER_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME}
            COMMAND
            ${BGFX_ROOT}/bin/shaderc
            -f ${SHADER_SRC_DIR}/${SHADER}
            -i ${CMAKE_SOURCE_DIR}/external/bgfx/shaders/src
            -o ${SHADER_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME}
            --type ${SHADER_TYPE}
            --platform ${PLATFORM}
            ${OPTIONS}
            --verbose
            MAIN_DEPENDENCY ${SHADER_SRC_DIR}/${SHADER}
            COMMENT "###### SHADER ###### Compiling '${SHADER_TYPE}', shader '${SHADER_NAME}', profile '${PROFILE}', platform '${PLATFORM}', options '${OPTIONS}', binDir '${SHADER_BIN_REP}'"
        )

    else()

            # message ("###### SHADER ###### Compiling '${SHADER_TYPE}', shader '${SHADER_NAME}', profile '${PROFILE}', platform '${PLATFORM}', options '${OPTIONS}', binDir '${SHADER_BIN_REP}")
        add_custom_command(
            OUTPUT ${SHADER_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME}
            COMMAND
            ${BGFX_ROOT}/bin/shaderc
            -f ${SHADER_SRC_DIR}/${SHADER}
            -i ${CMAKE_SOURCE_DIR}/external/bgfx/shaders/src
            -o ${SHADER_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME}
            --type ${SHADER_TYPE}
            --platform ${PLATFORM}
            --profile ${PROFILE} #only ${PROFILE} with PROFILE = --profile ${PROFILE} not worked
            ${OPTIONS}
            --verbose
            MAIN_DEPENDENCY ${SHADER_SRC_DIR}/${SHADER}
            COMMENT "###### SHADER ###### Compiling '${SHADER_TYPE}', shader '${SHADER_NAME}', profile '${PROFILE}', platform '${PLATFORM}', options '${OPTIONS}', binDir '${SHADER_BIN_REP}'"
        )

    endif()
#        message(${SHADER_SRC_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME})
    # add_custom_target(src DEPENDS ${SHADER_DIR}/bin/${BIN_SHADER_NAME})

    list(APPEND OUT_SHADERS ${SHADER_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME})
endif()
endmacro(COMPILE_SHADER)
