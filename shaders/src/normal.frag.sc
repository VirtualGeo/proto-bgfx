$input v_normal

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bgfx_shader.sh>

void main()
{
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//    return;
    vec3 normal = normalize(v_normal);
    gl_FragColor.xyz = (normal + 1.0) * 0.5;
    gl_FragColor.w = 1.0;
//    return;

}
