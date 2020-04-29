$input a_position
$input a_normal
$input a_texcoord0

$output v_texcoord0

/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

//#include "../common/common.sh"
#include <bgfx_shader.sh>

void main()
{
    v_texcoord0 = a_texcoord0;
//        gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
        gl_Position = vec4(a_position, 1.0);
}
