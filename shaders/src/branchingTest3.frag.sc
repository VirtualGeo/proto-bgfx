$input v_fragPos, v_normal, v_texcoord0

#include <bgfx_shader.sh>
#include "shaderlib.sh"

//#include "uniforms.sc"


//void main()
//{
//    vec3 result = vec3_splat(0.0); // vec3_splat != vec3 for direct3D

//    vec3 color;
//    if (material_hasDiffuseTexture > -0.5) {
//        color = texture2D(s_diffuse, v_texcoord0).xyz;
//        // color = toLinear(texture2D(s_diffuse, v_texcoord0) ).xyz;
//    }
//    else {
//        color = material_diffuse;
//    }

//    gl_FragColor.xyz = color;
//    gl_FragColor.w = 1.0;
//}
//#define HAS_DIFFUSE_TEXTURE
//#define HAS_SPECULAR_TEXTURE

#ifdef HAS_DIFFUSE_TEXTURE
//uniform vec4 hasDiffuseTexture;
SAMPLER2D(diffuseTexture, 0);
#endif
#ifdef HAS_SPECULAR_TEXTURE
//uniform vec4 hasSpecularTexture;
SAMPLER2D(specularTexture, 1);
#endif

uniform vec4 diffuseColor;

void main() {

    vec4 diffColor;
    vec4 specColor;

//    if (hasDiffuseTexture.x > 0.5) {
#ifdef HAS_DIFFUSE_TEXTURE
        diffColor = texture2D(diffuseTexture, v_texcoord0) * vec4(diffuseColor.xyz, 1.0);
#else
//    }
//    else {
        diffColor = vec4(diffuseColor.xyz, 1.0);
//    }
#endif

#ifdef HAS_SPECULAR_TEXTURE
//    if (hasSpecularTexture.x > 0.5) {
        specColor = texture2D(specularTexture, v_texcoord0);
#else
//    }
//    else {
        specColor = vec4(0.0, 0.0, 0.0, 1.0);
//    }
#endif


    gl_FragColor = diffColor + specColor;
    gl_FragColor.w = 1.0;
}
