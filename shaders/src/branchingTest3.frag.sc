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
//#define HAS_DIFFUSE_TEXTURE
//#define HAS_SPECULAR_TEXTURE

////#if HAS_DIFFUSE_TEXTURE > 0
//#if HAS_ADDITIONAL_TEXTURE_0 > 0
////uniform vec4 hasDiffuseTexture;
//SAMPLER2D(diffuseTexture, 0);
//#endif
////#if HAS_SPECULAR_TEXTURE > 0
//#if HAS_ADDITIONAL_TEXTURE_1 > 0
////uniform vec4 hasSpecularTexture;
//SAMPLER2D(specularTexture, 1);
//#endif

//uniform vec4 diffuseColor;

//#if HAS_ADDITIONAL_TEXTURE_0 > 0
//SAMPLER2D(additionalTexture0, 2);
//#endif
//#if HAS_ADDITIONAL_TEXTURE_1 > 0
//SAMPLER2D(additionalTexture1, 3);
//#endif
//#if HAS_ADDITIONAL_TEXTURE_2 > 0
//SAMPLER2D(additionalTexture0, 4);
//#endif
//#if HAS_ADDITIONAL_TEXTURE_3 > 0
//SAMPLER2D(additionalTexture1, 5);
//#endif
//#if HAS_ADDITIONAL_TEXTURE_4 > 0
//SAMPLER2D(additionalTexture2, 6);
//#endif
//#if HAS_ADDITIONAL_TEXTURE_5 > 0
//SAMPLER2D(additionalTexture3, 7);
//#endif
//#if HAS_ADDITIONAL_TEXTURE_6 > 0
//SAMPLER2D(additionalTexture4, 8);
//#endif
//#if HAS_ADDITIONAL_TEXTURE_7 > 0
//SAMPLER2D(additionalTexture5, 9);
//#endif
#ifdef HAS_TEXTURE_0
SAMPLER2D(texture0, 0);
#endif
#ifdef HAS_TEXTURE_1
SAMPLER2D(texture1, 1);
#endif
#ifdef HAS_TEXTURE_2
SAMPLER2D(texture2, 2);
#endif
#ifdef HAS_TEXTURE_3
SAMPLER2D(texture3, 3);
#endif
#ifdef HAS_TEXTURE_4
SAMPLER2D(texture4, 4);
#endif
#ifdef HAS_TEXTURE_5
SAMPLER2D(texture5, 5);
#endif
#ifdef HAS_TEXTURE_6
SAMPLER2D(texture6, 6);
#endif
#ifdef HAS_TEXTURE_7
SAMPLER2D(texture7, 7);
#endif
#ifdef HAS_TEXTURE_8
SAMPLER2D(texture8, 8);
#endif
#ifdef HAS_TEXTURE_9
SAMPLER2D(texture9, 9);
#endif
//#ifdef HAS_TEXTURE_10
//SAMPLER2D(texture10, 10);
//#endif
//#ifdef HAS_TEXTURE_11
//SAMPLER2D(texture11, 11);
//#endif
//#ifdef HAS_TEXTURE_12
//SAMPLER2D(texture12, 12);
//#endif
//SAMPLER2D(additionalTexture3, 5);

//uniform vec4 nAdditionalTexture;
//#define N_ADDITIONAL_TEXTURE

void main()
{

    //    vec4 diffColor;
    //    vec4 specColor;

    ////    if (hasDiffuseTexture.x > 0.5) {
    ////#if HAS_DIFFUSE_TEXTURE > 0
    //#if HAS_ADDITIONAL_TEXTURE_0 > 0
    //        diffColor = texture2D(diffuseTexture, v_texcoord0) * vec4(diffuseColor.xyz, 1.0);
    //#else
    ////    }
    ////    else {
    //        diffColor = vec4(diffuseColor.xyz, 1.0);
    ////    }
    //#endif

    ////#if HAS_SPECULAR_TEXTURE > 0
    //#if HAS_ADDITIONAL_TEXTURE_1 > 0
    ////    if (hasSpecularTexture.x > 0.5) {
    //        specColor = texture2D(specularTexture, v_texcoord0);
    //#else
    ////    }
    ////    else {
    //        specColor = vec4(0.0, 0.0, 0.0, 1.0);
    ////    }
    //#endif

    vec4 color = vec4_splat(0.0);

#ifdef HAS_TEXTURE_0
    color += texture2D(texture0, v_texcoord0);
#endif
#ifdef HAS_TEXTURE_1
    color += texture2D(texture1, v_texcoord0);
#endif
#ifdef HAS_TEXTURE_2
    color += texture2D(texture2, v_texcoord0);
#endif
#ifdef HAS_TEXTURE_3
    color += texture2D(texture3, v_texcoord0);
#endif
#ifdef HAS_TEXTURE_4
    color += texture2D(texture4, v_texcoord0);
#endif
#ifdef HAS_TEXTURE_5
    color += texture2D(texture5, v_texcoord0);
#endif
#ifdef HAS_TEXTURE_6
    color += texture2D(texture6, v_texcoord0);
#endif
#ifdef HAS_TEXTURE_7
    color += texture2D(texture7, v_texcoord0);
#endif
#ifdef HAS_TEXTURE_8
    color += texture2D(texture8, v_texcoord0);
#endif
#ifdef HAS_TEXTURE_9
    color += texture2D(texture9, v_texcoord0);
#endif
//#ifdef HAS_TEXTURE_10
//    color += texture2D(texture10, v_texcoord0);
//#endif
//#ifdef HAS_TEXTURE_11
//    color += texture2D(texture11, v_texcoord0);
//#endif
//#ifdef HAS_TEXTURE_12
//    color += texture2D(texture12, v_texcoord0);
//#endif

#ifdef N_TEXTURE
    color /= N_TEXTURE;
#endif


    //    gl_FragColor = diffColor + specColor;
    //fuck
    gl_FragColor = color;
    gl_FragColor.w = 1.0;
    gl_FragColor = toGamma(gl_FragColor);
}
