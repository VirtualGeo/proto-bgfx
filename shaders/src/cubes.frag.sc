$input v_normal, v_texcoord0

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

//#include <common.sh>
#include <bgfx_shader.sh>

SAMPLER2D(s_texColor, 0);

uniform vec4 u_diffuse;
// uniform bool u_hasDiffuseTexture = false;
#define u_diffuseColor u_diffuse.xyz
#define u_hasDiffuseTexture u_diffuse.w

void main()
{

	//gl_FragColor = v_color0;

	vec3 normal = normalize(v_normal);
	//gl_FragColor.xyz = (normal + 1.0) * 0.5;
	//gl_FragColor.w = 1.0;
	//return;
	// vec4 color = vec4(v_texcoord0.x, v_texcoord0.y, 1.0, 1.0);
	vec4 color;
	if (u_hasDiffuseTexture > 0.5) {
		color = texture2D(s_texColor, v_texcoord0);

	}
	else {
		color = vec4(u_diffuseColor, 1.0);

	}
		// color = texture2D(s_texColor, v_texcoord0);

	gl_FragColor = color;
	
}
