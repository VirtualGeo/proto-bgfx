$input a_position
$input a_texcoord0
$output v_position
$output v_texcoord0

/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

//#include "../common/common.sh"
#include <bgfx_shader.sh>

//#include "uniforms.sc"
#include "light.sc"


void main()
{
//    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
//    gl_Position = mul(u_lightSpaceMatrix, mul(u_lightSpaceMatrix, vec4(a_position, 1.0)));
//    mat4 id = mat4(0.0, 0.0, 0.0, 5.0,
//                   0.0, 0.0, 0.0, 5.0,
//                   0.0, 0.0, 0.0, 5.0,
//                   0.0, 0.0, 0.0, 0.0);
//    gl_Position = mul(u_model[0], mul(id + u_lightSpaceMatrix, vec4(a_position, 1.0)));
    gl_Position = mul(u_model[0], mul(u_lightSpaceMatrix, vec4(a_position, 1.0)));
//    gl_Position = mul(u_model[0], vec4(a_position, 1.0));

    v_position = gl_Position;
    v_texcoord0 = a_texcoord0;
}

//void main()
//{
////	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
//    gl_Position = mul(u_model[0], mul(u_lightSpaceMatrix, vec4(a_position, 1.0)));
//}

