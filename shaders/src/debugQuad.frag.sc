$input v_texcoord0

//#include "../common/common.sh"
#include <bgfx_shader.sh>

//void main()
//{
//    gl_FragColor = vec4_splat(0.0);
////    gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
////    gl_FragColor.xyz = vec3_splat(gl_FragCoord.z);
////    gl_FragColor.w = 1.0;
//}

//SAMPLER2D(s_shadowMap, 0);
SAMPLER2DSHADOW(s_shadowMap, 3);
//#define Sampler sampler2DShadow

float near = 1.0;
float far  = 50.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    return;
//    float depth = texture2D(s_shadowMap, v_texcoord0).r;
    float depth = shadow2D(s_shadowMap, vec3(v_texcoord0, 0.0));
//    float bias = 0.005;
//    vec3 texCoord = v_texcoord0.xy / v_texcoord0.w;
//    vec2 texCoord = v_texcoord0.xy;
//    float depth = shadow2D(s_shadowMap, vec3(texCoord.xy, texCoord.z -bias))
//    float depth = shadow2D(s_shadowMap, texCoord);
    gl_FragColor = vec4(vec3_splat(LinearizeDepth(depth) / far), 1.0); // divide by far for demonstration
//    gl_FragColor = vec4(vec3_splat(depth), 1.0);
}
