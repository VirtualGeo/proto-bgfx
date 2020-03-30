$input v_normal, v_texcoord0, v_pos, v_view

#include <bgfx_shader.sh>

struct DirLight {
    vec3 m_dir;
    vec3 m_intensity;
};

struct Material {
    vec3 m_diffuse;
    vec3 m_specular;
    vec3 m_ambient;
    float m_shininess;
};


SAMPLER2D(s_diffuse, 0);
//uniform vec4 u_diffuse;
//#define u_diffuseColor u_diffuse.xyz
//#define u_hasDiffuseTexture u_diffuse.w
//uniform vec4 u_hasDiffuseTexture;

SAMPLER2D(s_opacity, 1);
//uniform vec4 u_texturesEnable;
//#define u_hasOpacityTexture u_texturesEnable.x

uniform vec4 u_params[6];
//#define u_diffuseColor 			u_params[0].xyz
//#define u_hasDiffuseTexture 	u_params[0].w
//#define u_hasOpacityTexture		u_params[1].x
#define u_mat_diffuse          		u_params[0].xyz
#define u_mat_hasDiffuseTexture 	u_params[0].w
#define u_mat_specular          	u_params[1].xyz
#define u_mat_ambient           	u_params[2].xyz
#define u_mat_shininess         	u_params[2].w
#define u_mat_hasOpacityTexture 	u_params[3].x

#define u_dirLight_0_dir   		u_params[4].xyz
#define u_dirLight_0_color 		u_params[5].xyz

//#define u_dir_light_1_dir   	u_params[5].xyz
//#define u_dir_light_1_color 	u_params[6].xyz

//const float gamma = 2.2;

//vec3 calculateLambertDiffuse(vec3 normal, vec3 light_dir, vec3 diffuse_color)
//{
//    return max(dot(normal, light_dir), 0.0) * diffuse_color;
//}

//vec3 calculateBlinnSpecular(vec3 normal, vec3 view_dir, vec3 light_dir, vec3 specular_color, float shininess)
//{
//    vec3 half_dir = normalize(light_dir + view_dir);
//    float angle = max(dot(half_dir, normal), 0.0);
//    float strength = pow(angle, shininess);
//    return strength * specular_color;
//}

//vec3 calculateSingleLightShading(DirLight dir_light, Material material, vec3 normal, vec3 view_dir)
//{
//    vec3 light_dir = normalize(dir_light.dir);

//    vec3 diffuse = dir_light.intensity * calculateLambertDiffuse(normal, light_dir, material.diffuse);
//    vec3 specular = dir_light.intensity * calculateBlinnSpecular(normal, view_dir, light_dir, material.specular, material.shininess);

//    return diffuse + specular;
//}

//void main()
//{
//    vec3 linear_color = vec3(0.0, 0.0, 0.0);

//    Material material = Material(
//        u_diffuse,
//        u_specular,
//        u_ambient,
//        u_shininess
//    );

//    // When the triangle is back-facing, the normal direction will be flipped
//    vec3 view_dir = normalize(- v_view);
//    vec3 normal = dot(v_normal, view_dir) > 0.0 ? normalize(v_normal) : normalize(- v_normal);

//    linear_color += calculateSingleLightShading(DirLight(u_dir_light_0_dir, u_dir_light_0_color), material, normal, view_dir);
//    linear_color += calculateSingleLightShading(DirLight(u_dir_light_1_dir, u_dir_light_1_color), material, normal, view_dir);

//    linear_color += material.ambient;

//    vec3 corrected_color = pow(linear_color, vec3_splat(1.0 / gamma));

//    gl_FragColor.xyz = corrected_color;
//    gl_FragColor.w = 1.0;
//}


void main()
{

	//gl_FragColor = v_color0;

	vec3 normal = normalize(v_normal);
//    gl_FragColor.xyz = (normal + 1.0) * 0.5;
//    gl_FragColor.w = 1.0;
//    return;

//     vec4 color;
//    = vec4(v_texcoord0.x, v_texcoord0.y, 1.0, 1.0);
//        color = texture2D(s_diffuse, v_texcoord0);
//    gl_FragColor = color;
//    return;



    vec4 color;
    if (u_mat_hasDiffuseTexture > 0.5) {
        color = texture2D(s_diffuse, v_texcoord0);


//        if (color.w < 0.1) {
//            discard;
//        }
        if (u_mat_hasOpacityTexture > 0.5) {
            if (texture2D(s_opacity, v_texcoord0).x < 0.5) {
                discard;
            }
        }

    }
    else {
        color = vec4(u_mat_diffuse, 1.0);
    }

//    color = vec4(u_diffuseColor.xyz, 1.0);
    gl_FragColor = color;
//    vec3 lightDir = vec3(0.0, 0.0, -1.0);
//    float ndotl = dot(normalize(v_normal), lightDir);
//    float spec = pow(ndotl, 30.0);
//    gl_FragColor = vec4(pow(pow(color.xyz, vec3_splat(2.2) ) * ndotl + spec, vec3_splat(1.0/2.2) ), 1.0);

	
}
