/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */
$input v_position

//#include "../common/common.sh"
#include <bgfx_shader.sh>
#include <shaderlib.sh>

//float near = 0.1;
//float far  = 5.0;

//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // back to NDC
//    return (2.0 * near * far) / (far + near - z * (far - near));
//}

void main()
{
//    gl_FragColor = vec4_splat(0.0);
//    return;
//    gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
//    gl_FragColor.xyz = vec3_splat(gl_FragCoord.z);
//    return;
//    gl_FragColor.w = 1.0;

//    float depth = v_position.z / v_position.w * 0.5 + 0.5;
//    float depth = v_position.z / v_position.w;
//    gl_FragColor = vec4(vec3_splat(v_position.z), 1.0);
//    gl_FragColor = vec4(vec3_splat(v_position.z), 1.0);
//    float depth = (v_position.z - 2.0) / 5.0;
//    float z = v_position.z * 2.0 - 1.0;
//    float depth = (v_position.z - near) / (far - near);
    float depth = v_position.z / v_position.w * 0.5 + 0.5;
    gl_FragColor = packFloatToRgba(depth);
//    float depth = v_position.z;
//    depth = 1.1;
//    float depth = LinearizeDepth(v_position.z);
//    float depth = v_position.w / 100.0;
//    if (v_position.z > v_position.w) {
//        gl_FragColor = packFloatToRgba(0.0);
//    }
//    else {
//        gl_FragColor = packFloatToRgba(1.0);

//    }
//    if (depth >= 0.9) {
//        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
////        gl_FragColor = packFloatToRgba(0.0);
//        return;
//    }
//    if (depth <= 0.1) {
//        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
////        gl_FragColor = packFloatToRgba(1.0);
//        return;
//    }
//    gl_FragColor = vec4(vec3_splat(depth), 1.0);
//    gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
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
