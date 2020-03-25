
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

macro(COMPILE_SHADER SHADER)
    get_shader_infos(${SHADER} SHADER_NAME SHADER_TYPE BIN_SHADER_NAME OK)
    if(OK)
        # ----------------------- WINDOWS
        if(WIN32)
             message("WIN32")
            set(PLATFORM "windows")
            if(RENDERER_OpenGL)
                set(PROFILE "")
                set(OPTIMIZATION "")
                set(SHADER_BIN_REP "glsl")

            elseif(RENDERER_Vulkan)
                set(PROFILE "spirv")
                set(OPTIMIZATION "")
                set(SHADER_BIN_REP "spirv")


            else()
                #            if(RENDERER_DirectX)
                if(${SHADER_TYPE} STREQUAL "vertex")
                    set(PROFILE "vs_5_0")
                elseif(${SHADER_TYPE} STREQUAL "fragment")
                    set(PROFILE "ps_5_0")
                elseif(${SHADER_TYPE} STREQUAL "compute")
                    set(PROFILE "cs_5_0")
                endif()
                # set(PROFILE "")
                set(OPTIMIZATION "-O 3 --Werror")
                # set(OPTIMIZATION "")
                set(SHADER_BIN_REP "dx11")

            endif()

        # ------------------------- LINUX
        else()
            message("LINUX")
            set(PLATFORM "linux") # TODO
            if(RENDERER_Vulkan)
                message("Vulkan")
                set(PROFILE "spirv")
                set(OPTIMIZATION "")
                set(SHADER_BIN_REP "spirv")
            else()
                message("OpenGL")
                # set(PROFILE "glsl")
                set(PROFILE "")
                set(OPTIMIZATION "")
                set(SHADER_BIN_REP "glsl")
            endif()
        endif()

#        message("
#            OUTPUT ${SHADER_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME}
#            COMMAND
#            ${BGFX_ROOT}/bin/shaderc
#            -f ${SHADER_SRC_DIR}/${SHADER}
#            -o ${SHADER_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME}
#            --platform ${PLATFORM}
#            --type ${SHADER_TYPE}
#            --verbose
#            --profile ${PROFILE}
#            -i ${CMAKE_SOURCE_DIR}/external/bgfx/shaders/src
#            ${OPTIMIZATION}
#            MAIN_DEPENDENCY ${SHADER_SRC_DIR}/${SHADER}
#            COMMENT \"Compiling ${SHADER_TYPE} shader ${SHADER_NAME}\"
#            ")


        add_custom_command(
            OUTPUT ${SHADER_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME}
            COMMAND
            ${BGFX_ROOT}/bin/shaderc
            -f ${SHADER_SRC_DIR}/${SHADER}
            -o ${SHADER_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME}
            --platform ${PLATFORM}
            --type ${SHADER_TYPE}
            --verbose
            --profile ${PROFILE}
            -i ${CMAKE_SOURCE_DIR}/external/bgfx/shaders/src
            ${OPTIMIZATION}
            MAIN_DEPENDENCY ${SHADER_SRC_DIR}/${SHADER}
            COMMENT "Compiling ${SHADER_TYPE} shader ${SHADER_NAME}"
            )
        message(${SHADER_SRC_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME})
        # add_custom_target(src DEPENDS ${SHADER_DIR}/bin/${BIN_SHADER_NAME})

        list(APPEND OUT_SHADERS ${SHADER_DIR}/bin/${SHADER_BIN_REP}/${BIN_SHADER_NAME})
    endif()
endmacro(COMPILE_SHADER)
