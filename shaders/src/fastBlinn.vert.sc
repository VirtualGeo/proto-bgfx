$input a_position, a_normal, a_texcoord0
$output v_pos, v_view, v_normal, v_texcoord0

#include <bgfx_shader.sh>

void main()
{
    vec3 pos = a_position;

    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_pos = gl_Position.xyz;

    v_view = mul(u_modelView, vec4(a_position, 1.0)).xyz;
    v_normal = mul(u_modelView, vec4(a_normal, 0.0)).xyz;

    v_texcoord0 = a_texcoord0;
}

//$input a_position, a_normal
//$output v_pos, v_view, v_normal, v_color0

///*
// * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
// * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
// */

//#include <bgfx_shader.sh>
//#include "shaderlib.sh"

//uniform vec4 u_time;

//void main()
//{
//    vec3 pos = a_position;

//    float sx = sin(pos.x*32.0+u_time.x*4.0)*0.5+0.5;
//    float cy = cos(pos.y*32.0+u_time.x*4.0)*0.5+0.5;
//    vec3 displacement = vec3(sx, cy, sx*cy);
//    vec3 normal = a_normal.xyz*2.0 - 1.0;

//    pos = pos + normal*displacement*vec3(0.06, 0.06, 0.06);

//    gl_Position = mul(u_modelViewProj, vec4(pos, 1.0) );
//    v_pos = gl_Position.xyz;
//    v_view = mul(u_modelView, vec4(pos, 1.0) ).xyz;

//    v_normal = mul(u_modelView, vec4(normal, 0.0) ).xyz;

//    float len = length(displacement)*0.4+0.6;
//    v_color0 = vec4(len, len, len, 1.0);
//}