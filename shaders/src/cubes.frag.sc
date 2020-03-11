$input v_normal, v_texcoord0

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

//#include <common.sh>
#include <bgfx_shader.sh>

SAMPLER2D(s_texColor, 0);

void main()
{
	//gl_FragColor = v_color0;

	vec3 normal = normalize(v_normal);
	gl_FragColor.xyz = (normal + 1.0) * 0.5;
	gl_FragColor.w = 1.0;
	return;
	vec4 color = texture2D(s_texColor, v_texcoord0);

	gl_FragColor = color;
	
}
