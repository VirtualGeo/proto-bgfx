$input a_position
$input a_normal
//$input a_texcoord0 // vec2

$output v_normal


/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

//#include "../common/common.sh"
//#include <common.sh>

#include <bgfx_shader.sh>
void main()
{

    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
    //v_color0 = a_color0;


    //vec3 normal = a_normal.xyz*2.0 - 1.0;
//    vec3 normal = a_normal;
    v_normal = a_normal;

//    v_normal = mul(u_modelView, vec4(a_normal, 0.0) ).xyz;

//    v_normal = mul(u_model[0], vec4(a_normal, 0.0)).xyz;

//    v_texcoord0 = a_texcoord0;
}

