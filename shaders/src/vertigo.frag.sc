$input v_pos, v_normal, v_texcoord0

#include <bgfx_shader.sh>
#include "shaderlib.sh"


//uniform vec4 hasDiffuseTexture;
//uniform vec4 hasSpecularTexture;


SAMPLER2D(s_diffuse, 0);
//SAMPLER2D(specularTexture, 1);

uniform vec4 u_diffuse;

void main() {

//    vec4 diffColor;
//    vec4 specColor;
    vec4 color = vec4_splat(0.0);
    color = texture2D(s_diffuse, v_texcoord0);
//    color = vec4(u_diffuse.xyz, 1.0);

//    if (hasDiffuseTexture.x > 0.5) {
//        diffColor = texture2D(diffuseTexture, v_texcoord0);
//    }
//    else {
//        diffColor = vec4(diffuseColor.xyz, 1.0);
//    }

//    if (hasSpecularTexture.x > 0.5) {
//        specColor = texture2D(specularTexture, v_texcoord0);
//    }
//    else {
//        specColor = vec4(0.0, 0.0, 0.0, 1.0);
//    }


//    gl_FragColor = diffColor + specColor;
    gl_FragColor = color;
    gl_FragColor.w = 1.0;
}
