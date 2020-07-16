$input v_pos, v_view, v_normal

#include <bgfx_shader.sh>

struct DirLight
{
    vec3 dir;
    vec3 intensity;
};

struct Material
{
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};

uniform vec4 u_params[7];
#define u_diffuse           u_params[0].xyz
#define u_specular          u_params[1].xyz
#define u_ambient           u_params[2].xyz
#define u_shininess         u_params[2].w
//#define u_dir_light_0_dir   u_params[3].xyz
//#define u_dir_light_0_color u_params[4].xyz
//#define u_dir_light_1_dir   u_params[5].xyz
//#define u_dir_light_1_color u_params[6].xyz
#define u_dir_light_0_dir vec3(0.0, 1.0, 0.0)
#define u_dir_light_0_color vec3(1.0, 0.9, 0.9)
//#define u_dir_light_0_dir vec3(0.5, 0.5, 1.0)
//#define u_dir_light_0_color vec3(1.0, 0.9, 0.9)
#define u_dir_light_1_dir   vec3(-1.0, 0.0, 0.0)
#define u_dir_light_1_color vec3(0.2, 0.2, 0.5)

const float gamma = 2.2;

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

vec3 calculateSingleLightShading(DirLight dir_light, Material material, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(dir_light.dir);

    vec3 diffuse = dir_light.intensity * calculateLambertDiffuse(normal, light_dir, material.diffuse);
    vec3 specular = dir_light.intensity * calculateBlinnSpecular(normal, view_dir, light_dir, material.specular, material.shininess);

    return diffuse + specular;
}

void main()
{
    vec3 linear_color = vec3(0.0, 0.0, 0.0);

//    Material material = Material(
//        u_diffuse,
//        u_specular,
//        u_ambient,
//        u_shininess
//    );
    Material material = Material(
        vec3(0.5, 0.5, 0.5),
        vec3(1.0, 1.0, 1.0),
        vec3(0.0, 0.0, 0.0),
        128.0
    );


    // When the triangle is back-facing, the normal direction will be flipped
    vec3 view_dir = normalize(- v_view);
    vec3 normal = dot(v_normal, view_dir) > 0.0 ? normalize(v_normal) : normalize(- v_normal);

    linear_color += calculateSingleLightShading(DirLight(u_dir_light_0_dir, u_dir_light_0_color), material, normal, view_dir);
//    linear_color += calculateSingleLightShading(DirLight(u_dir_light_1_dir, u_dir_light_1_color), material, normal, view_dir);

    linear_color += material.ambient;

    vec3 corrected_color = pow(linear_color, vec3_splat(1.0 / gamma));

    gl_FragColor.xyz = corrected_color;
    gl_FragColor.w = 1.0;
}

//$input v_pos, v_view, v_normal, v_color0

///*
// * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
// * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
// */

//#include <bgfx_shader.sh>
//#include "shaderlib.sh"


//uniform vec4 u_time;

//vec2 blinn(vec3 _lightDir, vec3 _normal, vec3 _viewDir)
//{
//    float ndotl = dot(_normal, _lightDir);
//    vec3 reflected = _lightDir - 2.0*ndotl*_normal; // reflect(_lightDir, _normal);
//    float rdotv = dot(reflected, _viewDir);
//    return vec2(ndotl, rdotv);
//}

//float fresnel(float _ndotl, float _bias, float _pow)
//{
//    float facing = (1.0 - _ndotl);
//    return max(_bias + (1.0 - _bias) * pow(facing, _pow), 0.0);
//}

//vec4 lit(float _ndotl, float _rdotv, float _m)
//{
//    float diff = max(0.0, _ndotl);
//    float spec = step(0.0, _ndotl) * max(0.0, _rdotv * _m);
//    return vec4(1.0, diff, spec, 1.0);
//}

//void main()
//{
//    vec3 lightDir = vec3(0.0, 0.0, -1.0);
//    vec3 normal = normalize(v_normal);
//    vec3 view = normalize(v_view);
//    vec2 bln = blinn(lightDir, normal, view);
//    vec4 lc = lit(bln.x, bln.y, 1.0);
//    float fres = fresnel(bln.x, 0.2, 5.0);

//    float index = ( (sin(v_pos.x*3.0+u_time.x)*0.3+0.7)
//                + (  cos(v_pos.y*3.0+u_time.x)*0.4+0.6)
//                + (  cos(v_pos.z*3.0+u_time.x)*0.2+0.8)
//                )*M_PI;

//    vec3 color = vec3(sin(index*8.0)*0.4 + 0.6
//                    , sin(index*4.0)*0.4 + 0.6
//                    , sin(index*2.0)*0.4 + 0.6
//                    ) * v_color0.xyz;

//    gl_FragColor.xyz = pow(vec3(0.07, 0.06, 0.08) + color*lc.y + fres*pow(lc.z, 128.0), vec3_splat(1.0/2.2) );
////    gl_FragColor.xyz = (normal +1.0) * 0.5;
//    gl_FragColor.w = 1.0;
//}
