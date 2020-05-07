$input a_position
$output v_position
$outputwb v_texcoord0

/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

//#include "../common/common.sh"
#include <bgfx_shader.sh>

#include "uniforms.sc"


void main()
{
//    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
//    gl_Position = mul(u_lightSpaceMatrix, mul(u_lightSpaceMatrix, vec4(a_position, 1.0)));
    gl_Position = mul(u_model[0], mul(u_lightSpaceMatrix, vec4(a_position, 1.0)));

    v_position = gl_Position;
}

//void main()
//{
////	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
//    gl_Position = mul(u_model[0], mul(u_lightSpaceMatrix, vec4(a_position, 1.0)));
//}

