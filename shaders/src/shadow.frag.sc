/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

//#include "../common/common.sh"
#include <bgfx_shader.sh>

void main()
{
    gl_FragColor = vec4_splat(0.0);
//    gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
//    gl_FragColor.xyz = vec3_splat(gl_FragCoord.z);
//    gl_FragColor.w = 1.0;
}


//float near = 1.0;
//float far  = 50.0;

//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // back to NDC
//    return (2.0 * near * far) / (far + near - z * (far - near));
//}

//void main()
//{
//    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
//    gl_FragColor = vec4(vec3(depth), 1.0);
//}
