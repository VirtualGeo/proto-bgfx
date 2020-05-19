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

uniform vec4 hasDiffuseTexture;
uniform vec4 hasSpecularTexture;


SAMPLER2D(diffuseTexture, 0);
SAMPLER2D(specularTexture, 1);

uniform vec4 diffuseColor;

//#define N_ADDITIONAL_TEXTURE 2
SAMPLER2D(additionalTexture0, 2);
SAMPLER2D(additionalTexture1, 3);
SAMPLER2D(additionalTexture2, 4);
SAMPLER2D(additionalTexture3, 5);
SAMPLER2D(additionalTexture4, 6);
SAMPLER2D(additionalTexture5, 7);
SAMPLER2D(additionalTexture6, 8);
SAMPLER2D(additionalTexture7, 9);
//SAMPLER2D(additionalTexture3, 5);
//SAMPLER2D(additionalTexture4, 6);
uniform vec4 hasAdditionalTexture[8];
uniform vec4 nAdditionalTexture;
#define N_ADDITIONAL_TEXTURE nAdditionalTexture.x
//#define has_additional_texture(i)

void main() {

    vec4 diffColor = vec4_splat(0.0);
    vec4 specColor = vec4_splat(0.0);

    if (hasDiffuseTexture.x > 0.5) {
//        diffColor = texture2D(diffuseTexture, v_texcoord0);
        diffColor = texture2D(diffuseTexture, v_texcoord0) * vec4(diffuseColor.xyz, 1.0);
    }
    else {
        diffColor = vec4(diffuseColor.xyz, 1.0);
    }

    if (hasSpecularTexture.x > 0.5) {
        specColor = texture2D(specularTexture, v_texcoord0);
    }
    else {
        specColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

//    specColor /= 2.0;
//    diffColor /= 2.0;
//    for (int i =0; i < N_ADDITIONAL_TEXTURE; ++i) {
//        if (hasAdditionalTexture.x > 0.5) {
//            specColor += texture2D(additionalTexture, v_texcoord0);
//        else {
//            specColor += vec4(0.0, 0.0, 0.0, 1.0);
//        }
//    }
    int i =-1;
    if (++i < N_ADDITIONAL_TEXTURE) {
        if (hasAdditionalTexture[i].x > 0.5) {
            specColor = (texture2D(additionalTexture0, v_texcoord0) + specColor);
        }
    }
    if (++i < N_ADDITIONAL_TEXTURE) {
        if (hasAdditionalTexture[i].x > 0.5) {
            specColor = (texture2D(additionalTexture1, v_texcoord0) + specColor);
        }
    }
    if (++i < N_ADDITIONAL_TEXTURE) {
        if (hasAdditionalTexture[i].x > 0.5) {
            specColor = (texture2D(additionalTexture2, v_texcoord0) + specColor);
        }
    }
    if (++i < N_ADDITIONAL_TEXTURE) {
        if (hasAdditionalTexture[i].x > 0.5) {
            specColor = (texture2D(additionalTexture3, v_texcoord0) + specColor);
        }
    }
    if (++i < N_ADDITIONAL_TEXTURE) {
        if (hasAdditionalTexture[i].x > 0.5) {
            specColor = (texture2D(additionalTexture4, v_texcoord0) + specColor);
        }
    }
    if (++i < N_ADDITIONAL_TEXTURE) {
        if (hasAdditionalTexture[i].x > 0.5) {
            specColor = (texture2D(additionalTexture5, v_texcoord0) + specColor);
        }
    }


    specColor /= N_ADDITIONAL_TEXTURE;

    gl_FragColor = diffColor + specColor;
    gl_FragColor.w = 1.0;
}
