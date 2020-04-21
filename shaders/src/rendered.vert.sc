$input a_position, a_normal, a_texcoord0
$output v_pos, v_normal, v_texcoord0

#include <bgfx_shader.sh>


void main()
{
    // vec3 pos = a_position;

    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
//    v_pos = gl_Position.xyz;
    v_pos = mul(u_model[0], vec4(a_position, 1.0)).xyz;

//    v_view = mul(u_modelView, vec4(a_position, 1.0)).xyz;
//    vec3 eyepos = mul(vec4(0.0, 0.0, 0.0, 1.0), u_modelView).xyz;
//    vec3 eyepos = mul(u_modelView, vec4(a_position, 1.0)).xyz;
//    v_view = eyepos - v_pos;
//    v_view = vec3(0.0, 1.0, 1.0);
//    v_normal = mul(u_modelView, vec4(a_normal, 0.0)).xyz;
    v_normal = mul(u_model[0], vec4(a_normal, 0.0)).xyz;
    v_texcoord0 = a_texcoord0;
}
