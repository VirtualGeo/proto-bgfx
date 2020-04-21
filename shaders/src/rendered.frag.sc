$input v_pos, v_normal, v_texcoord0

#include <bgfx_shader.sh>
#include "shaderlib.sh" // do not include shaderlib.sh before bgfx_shader.sh


SAMPLER2D(s_diffuse, 0);
SAMPLER2D(s_opacity, 1);

struct DirLight {
    vec3 dir;
    vec3 intensity;
};

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};

uniform vec4 u_params[7];
#define u_diffuse u_params[0].xyz
#define u_hasDiffuseTexture u_params[0].w
#define u_specular u_params[1].xyz
#define u_hasOpacityTexture u_params[1].w
#define u_ambient u_params[2].xyz
#define u_shininess u_params[2].w
#define u_dir_light_0_dir u_params[3].xyz
#define u_dir_light_0_color u_params[4].xyz
#define u_viewPos u_params[5].xyz
//#define u_dir_light_1_dir   u_params[5].xyz
//#define u_dir_light_1_color u_params[6].xyz

vec3 calculateLambertDiffuse(vec3 normal, vec3 light_dir, vec3 diffuse_color)
{
    return max(dot(normal, light_dir), 0.0) * diffuse_color;
}

vec3 calculateBlinnSpecular(vec3 normal, vec3 view_dir, vec3 light_dir, vec3 specular_color, float shininess)
{
    vec3 half_dir = normalize(light_dir + view_dir);
    float angle = max(dot(half_dir, normal), 0.0);
    float strength = pow(angle, shininess);
    return strength * specular_color;
}

vec3 calculateSingleLightShading(DirLight dir_light, Material material, vec3 color, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-dir_light.dir);
    //    vec3 diffuse = dir_light.intensity * calculateLambertDiffuse(normal, light_dir, material.diffuse);
    //    vec3 specular = dir_light.intensity * calculateBlinnSpecular(normal, view_dir, light_dir, material.specular, material.shininess);

    vec3 diffuse = dir_light.intensity * calculateLambertDiffuse(normal, light_dir, color);
    vec3 specular = dir_light.intensity * calculateBlinnSpecular(normal, view_dir, light_dir, color, material.shininess);
    //    return diffuse;
    //    return specular;

    return diffuse + specular;
}

void main()
{
    vec3 color;
    if (u_hasOpacityTexture > 0.5) {
        color = texture2D(s_opacity, v_texcoord0).xyz;
        if (color.r < 0.1) {
            discard;
        }
    }

    vec3 result = vec3_splat(0.0); // vec3_splat != vec3 for direct3D

#ifdef BGFX_SHADER_LANGUAGE_PSSL
    Material material;
    material.diffuse = u_diffuse;
    material.specular = u_specular;
    material.ambient = u_ambient;
    material.shininess = u_shininess;

    DirLight dirLight;
    dirLight.dir = u_dir_light_0_dir;
    dirLight.intensity = u_dir_light_0_color;
#else
    Material material = Material(
        u_diffuse,
        u_specular,
        u_ambient,
        u_shininess);
    DirLight dirLight = DirLight(
        u_dir_light_0_dir,
        u_dir_light_0_color);
#endif

    // When the triangle is back-facing, the normal direction will be flipped
    //    vec3 view_dir = normalize(v_view);
    vec3 view_dir = normalize(u_viewPos - v_pos);
    //    vec3 normal = dot(v_normal, view_dir) > 0.0 ? normalize(v_normal) : normalize(- v_normal);
    vec3 normal = normalize(v_normal);

    //        vec3 color = texture2D(s_diffuse, v_texcoord0).xyz;
    if (u_hasDiffuseTexture > 0.5) {
        //        color = texture2D(s_diffuse, v_texcoord0).xyz;
        color = toLinear(texture2D(s_diffuse, v_texcoord0)).xyz;
    } else {
        color = material.diffuse;
    }

    //        if (color.w < 0.1) {
    //            discard;
    //        }
    //    vec3 color = toLinear(texture2D(s_diffuse, v_texcoord0) ).xyz;

    result += calculateSingleLightShading(dirLight, material, color, normal, view_dir);
    //    result += calculateSingleLightShading(DirLight(u_dir_light_1_dir, u_dir_light_1_color), material, normal, view_dir);

    //    //    vec4 color = texture2D(s_diffuse, v_texcoord0) ;
    //    //    gl_FragColor.xyz = max(vec3_splat(0.05), lightColor.xyz)*color.xyz;

    //    result += material.ambient;
    //        result += color * 0.01;
    //    result += material.specular;

    //const float gamma = 2.0;
    //const float gamma = 5.0;

    //    const float gamma = 0.1;
    //    const float exposure = 30.0;
    //    result = vec3_splat(1.0) - exp(-result * exposure);
    //    result = pow(result, vec3_splat(1.0 / gamma));

    //    gl_FragColor.xyz = result;
    gl_FragColor.xyz = result;
    gl_FragColor.w = 1.0;
    gl_FragColor = toGamma(gl_FragColor);
}
