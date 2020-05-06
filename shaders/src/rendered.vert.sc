$input a_position // vec3
$input a_normal // vec3
$input a_texcoord0 // vec2

$output v_fragPos // vec3
$output v_normal // vec3
$output v_texcoord0 // vec2
$output v_view // vec3
//$output v_fragPosLightSpace
$output v_fragPosLightSpace_0
$output v_fragPosLightSpace_1
$output v_fragPosLightSpace_2
//$output v_shadowCoord

#include <bgfx_shader.sh>

// ------------------------------------ SPOT_LIGHT
#define N_SPOT_LIGHT 2
#if N_SPOT_LIGHT > 0
struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
    vec3 position;
    float cutOff;
    float outerCutOff;
//    vec3 color;
    float constant;
    float _linear; // linear bgfx exist
    float quadratic;

    mat4 lightSpaceMatrix;
};
#define N_SPOT_LIGHT_VEC4 9
uniform vec4 u_spotLights[N_SPOT_LIGHT_VEC4 * N_SPOT_LIGHT];
#define spotLights(i) SpotLight(vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4]), \
                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 1]), \
                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 2]), \
                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 3]), \
                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 4]), \
                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 0].w, \
                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 1].w, \
                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 2].w, \
                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 3].w, \
                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 4].w, \
                mat4(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 5], \
                     u_spotLights[i * N_SPOT_LIGHT_VEC4 + 6], \
                     u_spotLights[i * N_SPOT_LIGHT_VEC4 + 7], \
                     u_spotLights[i * N_SPOT_LIGHT_VEC4 + 8]))

//#define nbSpotLights u_spotLights[0].x
#endif


uniform mat4 u_invModel;


void main()
{
    // vec3 pos = a_position;

    v_fragPos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    gl_Position = mul(u_viewProj, vec4(v_fragPos, 1.0));
    v_normal = mul(mat3(u_model[0]), a_normal);
//    vec4 normal = a_normal * 2.0 - 1.0;
//    v_normal = normalize(mul(u_modelview, vec4(normal.xyz, 0.0)).xyz)
//    v_normal = mul(mat3(transpose(u_invModel)), a_normal);
    v_texcoord0 = a_texcoord0;

    SpotLight spotLight_0 = spotLights(0);
//    SpotLight spotLight_1 = spotLights(1);
//    SpotLight spotLight_2 = spotLights(2);

    v_fragPosLightSpace_0 = mul(spotLight_0.lightSpaceMatrix, vec4(v_fragPos, 1.0));
//    v_fragPosLightSpace_1 = mul(spotLight_1.lightSpaceMatrix, vec4(v_fragPos, 1.0));
//    v_fragPosLightSpace_2 = mul(spotLight_2.lightSpaceMatrix, vec4(v_fragPos, 1.0));

//    u_spotLights[0] = vec4_splat(1.0);

//    const float shadowMapOffset = 0.001;
//    vec3 posOffset = a_position + v_normal.xyz * shadowMapOffset;
//    v_shadowCoord = mul(spotLight.lightSpaceMatrix, vec4(posOffset, 1.0));

//    v_fragPosLightSpace = mul(vec4(v_fragPos, 1.0), spotLight.lightSpaceMatrix);

//    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
//    v_fragPos = gl_Position.xyz;


//    vec3 normal = a_normal * 2.0 - 1.0;
//    v_normal = normalize(mul(u_modelView, vec4(normal.xyz, 0.0) ).xyz);
//    v_view = mul(u_modelView, vec4(a_position, 1.0)).xyz;

//    const float shadowMapOffset = 0.001;
//    vec3 posOffset = a_position + normal.xyz * shadowMapOffset;
//    v_shadowcoord = mul(u_lightMtx, vec4(posOffset, 1.0) );

//}
//    v_view = mul(u_modelView, vec4(a_position, 1.0)).xyz;
//    vec3 eyepos = mul(vec4(0.0, 0.0, 0.0, 1.0), u_modelView).xyz;
//    vec3 eyepos = mul(u_modelView, vec4(a_position, 1.0)).xyz;
//    v_view = eyepos - v_fragPos;
//    v_view = vec3(0.0, 1.0, 1.0);
//    v_normal = mul(u_modelView, vec4(a_normal, 0.0)).xyz;
//    v_normal = mul(u_model[0], vec4(a_normal, 0.0)).xyz;

}
