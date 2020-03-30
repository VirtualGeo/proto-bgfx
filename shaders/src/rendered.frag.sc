$input v_normal, v_texcoord0, v_wpos, v_view, v_weyepos

#include <bgfx_shader.sh>
#include "shaderlib.sh"
    //uniform vec4  u_viewRect;
    //uniform vec4  u_viewTexel;
    //uniform mat4  u_view;
    //uniform mat4  u_invView;
    //uniform mat4  u_proj;
    //uniform mat4  u_invProj;
    //uniform mat4  u_viewProj;
    //uniform mat4  u_invViewProj;
    //uniform mat4  u_model[BGFX_CONFIG_MAX_BONES];
    //uniform mat4  u_modelView;
    //uniform mat4  u_modelViewProj;
    //uniform vec4  u_alphaRef4;
    //#define u_alphaRef u_alphaRef4.x

    struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};

SAMPLER2D(s_diffuse, 0);
//uniform vec4 u_diffuse;
//#define u_diffuseColor u_diffuse.xyz
//#define u_hasDiffuseTexture u_diffuse.w
//uniform vec4 u_hasDiffuseTexture;

SAMPLER2D(s_opacity, 1);
//uniform vec4 u_texturesEnable;
//#define u_hasOpacityTexture u_texturesEnable.x

uniform vec4 u_params[8];
//#define u_diffuseColor 			u_params[0].xyz
//#define u_hasDiffuseTexture 	u_params[0].w
//#define u_hasOpacityTexture		u_params[1].x
#define u_mat_diffuse u_params[0].xyz
#define u_mat_hasDiffuseTexture u_params[0].w
#define u_mat_specular u_params[1].xyz
#define u_mat_ambient u_params[2].xyz
#define u_mat_shininess u_params[2].w
#define u_mat_hasOpacityTexture u_params[3].x

#define u_dirLight_0_dir u_params[4].xyz
#define u_dirLight_0_diffuse u_params[5].xyz
#define u_dirLight_0_ambient u_params[6].xyz
#define u_dirLight_0_specular u_params[7].xyz

//#define u_dir_light_1_dir   	u_params[5].xyz
//#define u_dir_light_1_color 	u_params[6].xyz

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
//    vec3 light_dir = normalize(dir_light.m_direction);

//    vec3 diffuse = dir_light.m_ambient * calculateLambertDiffuse(normal, light_dir, material.m_diffuse);
//    vec3 specular = dir_light.m_ambient * calculateBlinnSpecular(normal, view_dir, light_dir, material.m_specular, material.m_shininess);

//    return diffuse + specular;
//}

Material material = Material(
    u_mat_diffuse,
    u_mat_specular,
    u_mat_ambient,
    u_mat_shininess);

//    lightColor = contribDirLight(dirLight, normal, view, v_wpos);
vec3 contribDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 wpos)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
//    vec3 reflectDir = -lightDir - 2.0 * dot(normal, -lightDir) * normal;
//    vec3 reflectDir = vec3(1.0, 1.0, 0.0);

    // combine results
    vec3 ambient;
    vec3 diffuse;
    float diff = max(dot(normal, lightDir), 0.0);
    if (u_mat_hasDiffuseTexture > 0.5) {
        vec4 texColor = texture2D(s_diffuse, v_texcoord0);

        //                if (texColor.a < 0.1)
        //                        discard;

        //                if (length(vec3(texColor)) < 0.01) // uv texture failed
        //                        discard;

        ambient = light.ambient * vec3(texColor);
        //            diffuse = light.diffuse * diff * vec3(texColor);
        diffuse = light.diffuse * diff * vec3(texColor);
        //            diffuse = light.diffuse * diff * material.diffuse;
        //            ambient = vec3(diff);
        //                specular = light.specular * vec3(texColor);
    } else {
        ambient = light.ambient * material.ambient;

        diffuse = light.diffuse * diff * material.diffuse;
    }
//    return diffuse;
//    return ambient;

    //    diffuse = light.diffuse * diff * material.diffuse;
    //    vec3 specular = light.specular * spec * vec3(texture2D(texture_specular1, fs_in.TexCoords));
    //        vec3 specular = light.specular * spec * max(vec3(0.1), material.specular);
    vec3 specular;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //        if (has_texture_specular) {
    //                specular = light.specular * spec * vec3(texture2D(s_specular, texCoords)).r;
    //        }
    //        else {
//    specular = light.specular * spec * material.specular;
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    specular = light.specular * spec * material.specular;
//    specular = light.specular * spec;
    return specular;
    //        }

    //            vec3 specular = light.specular * spec * vec3(0.2, 0.2, 0.1);
    //            vec3 specular = light.specular * spec * vec3(1.0f);
    //    vec3 specular = light.specular * spec * vec3(0.1, 0.1, 0.1);
    //    return ambient;
    //    return diffuse;
    //    return specular;
    //    if (shadow) {
    //            float shadow = ShadowCalculation(fs_in.FragPosLightSpace[light.id], normal, light);
    //            return (ambient + (1.0 - shadow) * (diffuse + specular));
    //    } else {

    return ambient + diffuse + specular;
    //    }
}

void main()
{
    if (u_mat_hasOpacityTexture > 0.5) {
        if (texture2D(s_opacity, v_texcoord0).x < 0.5) {
            discard;
        }
    }

    DirLight dirLight = DirLight(
        u_dirLight_0_dir,
        u_dirLight_0_diffuse,
        u_dirLight_0_ambient,
        u_dirLight_0_specular);

    //    vec3 linear_color = vec3(0.0, 0.0, 0.0);
    vec3 result = vec3(0.0);

    // When the triangle is back-facing, the normal direction will be flipped
    vec3 normal;
    normal = normalize(v_normal);
    //    vec3 view_dir = normalize(-v_view);
//    vec3 view = normalize(v_view);
    vec3 viewDir = normalize(v_weyepos - v_wpos);
    //    vec3 view_dir = normalize(u_viewPos - v_fragPos);
    //    vec3 normal = dot(v_normal, view_dir) > 0.0 ? normalize(v_normal) : normalize(- v_normal);

    //    vec3 lightColor;
    //    result += contribDirLight(dirLight, normal, view_dir, v_texcoord0);
    result += contribDirLight(dirLight, normal, viewDir, v_wpos);
    //    lightColor += contribDirLight(dirLight, normal, view, v_wpos);

    //    linear_color += calculateSingleLightShading(DirLight(u_dirLight_0_dir, u_dirLight_0_color), material, normal, view_dir);
    //    linear_color += calculateSingleLightShading(DirLight(u_dir_light_1_dir, u_dir_light_1_color), material, normal, view_dir);

    //    linear_color += material.m_ambient;

    //        vec3 corrected_color = pow(linear_color, vec3_splat(1.0 / gamma));
    //       result = pow(result, vec3_splat(1.0 / gamma));

    //    const float gamma = 0.5;
    //        const float exposure = 2.0;
    //    result = vec3(1.0) - exp(result * exposure);
    //    result = pow(result, vec3(1.0 / gamma));

    //    vec4 color = toLinear(texture2D(s_diffuse, v_texcoord0) );
    //    vec4 color = texture2D(s_diffuse, v_texcoord0) ;

    //    gl_FragColor.xyz = max(vec3_splat(0.05), lightColor.xyz)*color.xyz;
    //    gl_FragColor.w = 1.0;
    //}
    //    gl_FragColor.xyz = corrected_color;
    //    gl_FragColor.xyz = linear_color;
    //    gl_FragColor.w = 1.0;
    gl_FragColor = vec4(result, 1.0);
    //    gl_FragColor = toGamma(gl_FragColor);
}

//void main()
//{

//	//gl_FragColor = v_color0;

//	vec3 normal = normalize(v_normal);
////    gl_FragColor.xyz = (normal + 1.0) * 0.5;
////    gl_FragColor.w = 1.0;
////    return;

////     vec4 color;
////    = vec4(v_texcoord0.x, v_texcoord0.y, 1.0, 1.0);
////        color = texture2D(s_diffuse, v_texcoord0);
////    gl_FragColor = color;
////    return;

//    vec4 color;
//    if (u_mat_hasDiffuseTexture > 0.5) {
//        color = texture2D(s_diffuse, v_texcoord0);

////        if (color.w < 0.1) {
////            discard;
////        }
//        if (u_mat_hasOpacityTexture > 0.5) {
//            if (texture2D(s_opacity, v_texcoord0).x < 0.5) {
//                discard;
//            }
//        }

//    }
//    else {
//        color = vec4(u_mat_diffuse, 1.0);
//    }

////    color = vec4(u_diffuseColor.xyz, 1.0);
//    gl_FragColor = color;
////    vec3 lightDir = vec3(0.0, 0.0, -1.0);
////    float ndotl = dot(normalize(v_normal), lightDir);
////    float spec = pow(ndotl, 30.0);
////    gl_FragColor = vec4(pow(pow(color.xyz, vec3_splat(2.2) ) * ndotl + spec, vec3_splat(1.0/2.2) ), 1.0);

//}
