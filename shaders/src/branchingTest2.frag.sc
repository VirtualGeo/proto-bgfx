$input v_pos, v_normal, v_texcoord0

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

//SAMPLER2D(diffuseTexture, 0);
//SAMPLER2D(specularTexture, 1);

//uniform vec4 diffuseColor;
#if N_TEXTURE > 0
SAMPLER2D(texture0, 0);
#endif
#if N_TEXTURE > 1
SAMPLER2D(texture1, 1);
#endif
#if N_TEXTURE > 2
SAMPLER2D(texture2, 2);
#endif
#if N_TEXTURE > 3
SAMPLER2D(texture3, 3);
#endif
#if N_TEXTURE > 4
SAMPLER2D(texture4, 4);
#endif
#if N_TEXTURE > 5
SAMPLER2D(texture5, 5);
#endif
#if N_TEXTURE > 6
SAMPLER2D(texture6, 6);
#endif
#if N_TEXTURE > 7
SAMPLER2D(texture7, 7);
#endif
#if N_TEXTURE > 8
SAMPLER2D(texture8, 8);
#endif
#if N_TEXTURE > 9
SAMPLER2D(texture9, 9);
#endif
//SAMPLER2D(additionalTexture3, 5);
//SAMPLER2D(additionalTexture4, 6);
//uniform vec4 nTexture;

//uniform vec4 nAdditionalTexture;
//#define N_ADDITIONAL_TEXTURE nAdditionalTexture.x

void main()
{

//    vec4 diffColor;
//    vec4 specColor;

////        diffColor = texture2D(diffuseTexture, v_texcoord0) * vec4(diffuseColor.xyz, 1.0);
//        diffColor = vec4(texture2D(diffuseTexture, v_texcoord0).xyz, 1.0) * vec4(diffuseColor.xyz, 1.0);

////        specColor = texture2D(specularTexture, v_texcoord0);
//        specColor = vec4(texture2D(specularTexture, v_texcoord0).xyz, 1.0);

//    specColor /= 2.0;
//    diffColor /= 2.0;

//    int i =-1;
//    if (++i < N_ADDITIONAL_TEXTURE) {
//            specColor = (texture2D(additionalTexture0, v_texcoord0) + specColor);
//    }
//    if (++i < N_ADDITIONAL_TEXTURE) {
//            specColor = (texture2D(additionalTexture1, v_texcoord0) + specColor);
//    }
//    if (++i < N_ADDITIONAL_TEXTURE) {
//            specColor = (texture2D(additionalTexture2, v_texcoord0) + specColor);
//    }
//    if (++i < N_ADDITIONAL_TEXTURE) {
//            specColor = (texture2D(additionalTexture3, v_texcoord0) + specColor);
//    }
//    if (++i < N_ADDITIONAL_TEXTURE) {
//            specColor = (texture2D(additionalTexture4, v_texcoord0) + specColor);
//    }
//    if (++i < N_ADDITIONAL_TEXTURE) {
//            specColor = (texture2D(additionalTexture5, v_texcoord0) + specColor);
//    }
    vec4 color = vec4_splat(0.0);

#if N_TEXTURE > 0
    color += texture2D(texture0, v_texcoord0);
#endif
#if N_TEXTURE > 1
    color += texture2D(texture1, v_texcoord0);
#endif
#if N_TEXTURE > 2
    color += texture2D(texture2, v_texcoord0);
#endif
#if N_TEXTURE > 3
    color += texture2D(texture3, v_texcoord0);
#endif
#if N_TEXTURE > 4
    color += texture2D(texture4, v_texcoord0);
#endif
#if N_TEXTURE > 5
    color += texture2D(texture5, v_texcoord0);
#endif
#if N_TEXTURE > 6
    color += texture2D(texture6, v_texcoord0);
#endif
#if N_TEXTURE > 7
    color += texture2D(texture7, v_texcoord0);
#endif
#if N_TEXTURE > 8
    color += texture2D(texture8, v_texcoord0);
#endif
#if N_TEXTURE > 9
    color += texture2D(texture9, v_texcoord0);
#endif

#ifdef N_TEXTURE
    color /= N_TEXTURE;
#endif

    gl_FragColor = color;
    gl_FragColor.w = 1.0;
    gl_FragColor = toGamma(gl_FragColor);
}
