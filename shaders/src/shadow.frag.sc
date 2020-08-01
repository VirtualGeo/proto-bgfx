/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */
$input v_position
$input v_texcoord0

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
#include "light.sc"
#include "material.sc"


void main()
{
//    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
//    gl_FragColor = packFloatToRgba(0.0);
//    return;
    if (material_hasOpacityTexture > -0.5) {
        float opacity = texture2D(s_opacity, v_texcoord0).r;
        if (opacity < 0.1) {
            discard;
        }
    }


    if (material_hasDiffuseTexture > -0.5) {
        vec4 tex = texture2D(s_diffuse, v_texcoord0);
        if (tex.w < 0.5)
            discard;
    }

//        color = toLinear(tex).xyz;

        //                color = texture2D(s_diffuse, v_texcoord0).xyz;
//        color = toLinear(texture2D(s_diffuse, v_texcoord0)).xyz;
//    } else {
//        color = material_diffuse;


//    gl_FragColor = vec4_splat(0.0);
//    return;
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
//    float depth = v_position.z / v_position.w;
    gl_FragColor = packFloatToRgba(depth);
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//    gl_FragColor = packFloatToRgba(0.0);
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
