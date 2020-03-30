$input v_normal, v_texcoord0

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

//#include <common.sh>
#include <bgfx_shader.sh>

SAMPLER2D(s_diffuse, 0);
uniform vec4 u_diffuse;
#define u_diffuseColor u_diffuse.xyz
#define u_hasDiffuseTexture u_diffuse.w
//uniform vec4 u_hasDiffuseTexture;

SAMPLER2D(s_opacity, 1);
uniform vec4 u_texturesEnable;
#define u_hasOpacityTexture u_texturesEnable.x

void main()
{

	//gl_FragColor = v_color0;

	vec3 normal = normalize(v_normal);
//    gl_FragColor.xyz = (normal + 1.0) * 0.5;
//    gl_FragColor.w = 1.0;
//    return;

	// vec4 color = vec4(v_texcoord0.x, v_texcoord0.y, 1.0, 1.0);



    vec4 color;
    if (u_hasDiffuseTexture > 0.5) {
        color = texture2D(s_diffuse, v_texcoord0);


//        if (color.w < 0.1) {
//            discard;
//        }
        if (u_hasOpacityTexture > 0.5) {
            if (texture2D(s_opacity, v_texcoord0).x < 0.5) {
                discard;
            }
        }

    }
    else {
        color = vec4(u_diffuseColor, 1.0);
    }

//    color = vec4(u_diffuseColor.xyz, 1.0);
    gl_FragColor = color;
	
}
