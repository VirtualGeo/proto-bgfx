$input a_position // vec3
$input a_normal // vec3
$input a_texcoord0 // vec2

$output v_pos // vec3
$output v_normal // vec3
$output v_texcoord0 // vec2
//$output v_view // vec3


//$output v_posLightSpace
//#if N_SPOT_LIGHT > 0
//#if N_LIGHT > 0
$output v_posLightSpace_0
//#if N_LIGHT > 1
$output v_posLightSpace_1
//#endif
//#endif
//$output v_posLightSpace_2
//$output v_shadowCoord

#include <bgfx_shader.sh>

//#include "uniforms.sc"
#include "light.sc"

//#define spotLights_fragPos(i) (v_posLightSpace_##i)
    //#define fragPosLightSpaces(i) (v_posLightSpace_ ## i)
    //#define dirLights_fragPos(i) v_posLightSpace_ ## i

    //void spotLights_setFragPos(int iLight, vec4 pos) {
    //}

void main()
{
    // vec3 pos = a_position;

    v_pos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    gl_Position = mul(u_viewProj, vec4(v_pos, 1.0));
//    v_normal = mul(mat3(u_model[0]), a_normal); // question: hlsl failed
    v_normal = mul(u_model[0], vec4(a_normal, 1.0)).xyz;

    //    vec4 normal = a_normal * 2.0 - 1.0;
    //    v_normal = normalize(mul(u_modelview, vec4(normal.xyz, 0.0)).xyz)
    //    v_normal = mul(mat3(transpose(u_invModel)), a_normal);
    v_texcoord0 = a_texcoord0;

//        SpotLight spotLight_0 = spotLights(0);
    //    SpotLight spotLight_1 = spotLights(1);
#if N_DIR_LIGHT > 0
    dirLights(dirLight0, 0);
    v_posLightSpace_0 = mul(dirLight0.lightSpaceMatrix, vec4(v_pos, 1.0));
#endif

#if N_SPOT_LIGHT > 0
    spotLights(spotLight0, 0);
    v_posLightSpace_0 = mul(spotLight0.lightSpaceMatrix, vec4(v_pos, 1.0));

#if N_SPOT_LIGHT > 1
    spotLights(spotLight1, 1);
    v_posLightSpace_1 = mul(spotLight1.lightSpaceMatrix, vec4(v_pos, 1.0));
#endif
#endif

//    v_posLightSpace_0 = mul(spotLights(0).lightSpaceMatrix, vec4(v_pos, 1.0));
//    v_posLightSpace_1 = mul(spotLights(1).lightSpaceMatrix, vec4(v_pos, 1.0));

    //    SpotLight spotLight_2 = spotLights(2);
    //    int nbLight = 0;
    //#if N_DIR_LIGHT > 0
    //    for(int i = 0; i < N_DIR_LIGHT; i++) {
    //        dirLights_fragPos(i) = mul(dirLights(i).lightSpaceMatrix, vec4(v_pos, 1.0));
    //        ++nbLight;
    //    }
    //#endif
    //    // phase 2: point lights
    //#if N_POINT_LIGHT > 0
    //    for(int i = 0; i < N_POINT_LIGHT; i++) {
    //        dirLights_fragPos(i) = mul(dirLights(i).lightSpaceMatrix, vec4(v_pos, 1.0));
    //        ++nbLight;
    //    }
    //#endif
    //    // phase 3: spot light
    //#if N_SPOT_LIGHT > 0
    //    for(int i = 0; i < N_SPOT_LIGHT; i++) {
    //        //        spotLights_fragPos(i) = mul(spotLights(i).lightSpaceMatrix, vec4(v_pos, 1.0));
    //        if (i == 0) {
    //            spotLights_fragPos(0) = mul(spotLights(0).lightSpaceMatrix, vec4(v_pos, 1.0));
    //        }
    //        else if (i == 1) {
    //            spotLights_fragPos(1) = mul(spotLights(1).lightSpaceMatrix, vec4(v_pos, 1.0));
    //        }
    //        ++nbLight;
    //    }

    //#endif

    //    int i =0;
    //    spotLights_setFragPos(i, mul(spotLights(0).lightSpaceMatrix, vec4(v_pos, 1.0)));
    //    v_posLightSpace_0 = vec4_splat(0.0);
    //    fragPosLightSpaces(0) = vec4_splat(0.0);
    //        v_posLightSpace_1 = mul(spotLight_0.lightSpaceMatrix, vec4(v_pos, 1.0));
    //        v_posLightSpace_1 = mul(spotLight_0.lightSpaceMatrix, vec4(v_pos, 1.0));
    //    fragPosLightSpaces(1) = mul(spotLights(1).lightSpaceMatrix, vec4(v_pos, 1.0));

    //    v_posLightSpace_2 = mul(spotLight_2.lightSpaceMatrix, vec4(v_pos, 1.0));

    //    u_spotLights[0] = vec4_splat(1.0);

    //    const float shadowMapOffset = 0.001;
    //    vec3 posOffset = a_position + v_normal.xyz * shadowMapOffset;
    //    v_shadowCoord = mul(spotLight.lightSpaceMatrix, vec4(posOffset, 1.0));

    //    v_posLightSpace = mul(vec4(v_pos, 1.0), spotLight.lightSpaceMatrix);

    //    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    //    v_pos = gl_Position.xyz;

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
    //    v_view = eyepos - v_pos;
    //    v_view = vec3(0.0, 1.0, 1.0);
    //    v_normal = mul(u_modelView, vec4(a_normal, 0.0)).xyz;
    //    v_normal = mul(u_model[0], vec4(a_normal, 0.0)).xyz;
}
