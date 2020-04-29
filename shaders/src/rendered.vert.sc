$input a_position // vec3
$input a_normal // vec3
$input a_texcoord0 // vec2

$output v_fragPos // vec3
$output v_normal // vec3
$output v_texcoord0 // vec2
$output v_view // vec3

#include <bgfx_shader.sh>

uniform mat4 u_invModel;


void main()
{
    // vec3 pos = a_position;

    v_fragPos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    v_normal = mul(mat3(u_model[0]), a_normal);
//    v_normal = mul(mat3(transpose(u_invModel)), a_normal);
    v_texcoord0 = a_texcoord0;
    gl_Position = mul(u_viewProj, vec4(v_fragPos, 1.0));

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
