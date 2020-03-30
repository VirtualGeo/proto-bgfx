$input a_position, a_normal, a_texcoord0
$output v_normal, v_texcoord0, v_wpos, v_view, v_weyepos

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

//#include "../common/common.sh"
//#include <common.sh>
#include <bgfx_shader.sh>


void main()
{
//    vec3 wpos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    vec3 wpos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
//    v_wpos = wpos;
    v_wpos = vec3(mul(u_modelView, vec4(a_position, 1.0)));

//    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
    gl_Position = mul(u_viewProj, vec4(wpos, 1.0) );
    //v_color0 = a_color0;

    //    v_normal = a_normal;
    //vec3 normal = a_normal.xyz*2.0 - 1.0;
//        vec3 normal = a_normal;
//    vec3 normal = a_normal.xyz * 2.0 - 1.0;
//    v_normal = mul(u_modelView, vec4(normal, 0.0)).xyz;
        v_normal = mul(u_model[0], vec4(a_normal, 0.0)).xyz;
//        v_normal = mul(transpose(inv(mat3(u_model[0]))), a_normal);


//    v_pos = gl_Position.xyz;
//    v_view = mul(u_modelView, vec4(a_normal, 0.0)).xyz;

    v_weyepos = mul(vec4(0.0, 0.0, 0.0, 1.0), u_view).xyz;
    v_view = v_weyepos - wpos;
    //    v_fragPos = mul(u_model[0], vec4(a_position, 1.0));
    v_texcoord0 = a_texcoord0;
}
