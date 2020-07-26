
// source: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/6.multiple_lights.fs
// ------------------------------------ DIR_LIGHT
#ifndef N_DIR_LIGHT
#define N_DIR_LIGHT 1
#endif
#if N_DIR_LIGHT > 0
struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
    mat4 lightSpaceMatrix;
};
#define N_DIR_LIGHT_VEC4 8
//#if N_DIR_LIGHT > 0
uniform vec4 u_dirLights[N_DIR_LIGHT_VEC4 * N_DIR_LIGHT];
//#define dirLights(i) DirLight(vec3(u_dirLights[i * N_DIR_LIGHT_VEC4]), \
//                vec3(u_dirLights[i * N_DIR_LIGHT_VEC4 + 1]), \
//                vec3(u_dirLights[i * N_DIR_LIGHT_VEC4 + 2]), \
//                vec3(u_dirLights[i * N_DIR_LIGHT_VEC4 + 3]))
#define dirLights(varName, i) DirLight varName; \
varName.ambient = u_dirLights[i * N_DIR_LIGHT_VEC4].xyz; \
varName.diffuse = u_dirLights[i * N_DIR_LIGHT_VEC4 + 1].xyz; \
varName.specular = u_dirLights[i * N_DIR_LIGHT_VEC4 + 2].xyz; \
varName.direction = u_dirLights[i * N_DIR_LIGHT_VEC4 + 3].xyz; \
varName.lightSpaceMatrix = mat4(u_dirLights[i * N_DIR_LIGHT_VEC4 + 4], \
                                u_dirLights[i * N_DIR_LIGHT_VEC4 + 5], \
                                u_dirLights[i * N_DIR_LIGHT_VEC4 + 6], \
                                u_dirLights[i * N_DIR_LIGHT_VEC4 + 7])

//#endif
//#define nbDirLights u_dirLights[0].w
#endif


// ------------------------------------ SPOT_LIGHT
#ifndef N_SPOT_LIGHT
#define N_SPOT_LIGHT 0
#endif
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
//#define spotLights_position(i) u_spotLights[i * N_SPOT_LIGHT_VEC4 + 4].xyz
#define spotLights(varName, i) SpotLight varName; \
varName.ambient = 		u_spotLights[i * N_SPOT_LIGHT_VEC4].xyz; \
varName.diffuse = 		u_spotLights[i * N_SPOT_LIGHT_VEC4 + 1].xyz; \
varName.specular = 		u_spotLights[i * N_SPOT_LIGHT_VEC4 + 2].xyz; \
varName.direction = 	u_spotLights[i * N_SPOT_LIGHT_VEC4 + 3].xyz; \
varName.position = 		u_spotLights[i * N_SPOT_LIGHT_VEC4 + 4].xyz; \
varName.cutOff = 		u_spotLights[i * N_SPOT_LIGHT_VEC4 + 0].w; \
varName.outerCutOff = 	u_spotLights[i * N_SPOT_LIGHT_VEC4 + 1].w; \
varName.constant = 		u_spotLights[i * N_SPOT_LIGHT_VEC4 + 2].w; \
varName._linear = 		u_spotLights[i * N_SPOT_LIGHT_VEC4 + 3].w; \
varName.quadratic = 	u_spotLights[i * N_SPOT_LIGHT_VEC4 + 4].w; \
varName.lightSpaceMatrix = mat4(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 5], \
                                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 6], \
                                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 7], \
                                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 8])

//#define spotLights(i) SpotLight(vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4]), \
//                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 1]), \
//                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 2]), \
//                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 3]), \
//                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 4]), \
//                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 0].w, \
//                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 1].w, \
//                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 2].w, \
//                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 3].w, \
//                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 4].w, \
//                mat4(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 5], \
//                     u_spotLights[i * N_SPOT_LIGHT_VEC4 + 6], \
//                     u_spotLights[i * N_SPOT_LIGHT_VEC4 + 7], \
//                     u_spotLights[i * N_SPOT_LIGHT_VEC4 + 8]))

//#define nbSpotLights u_spotLights[0].x
#endif


// ------------------------------------ POINT_LIGHT
#ifndef N_POINT_LIGHT
#define N_POINT_LIGHT 0
#endif
#if N_POINT_LIGHT > 0
struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    float constant;
    float _linear;
    float quadratic;
};
#define N_POINT_LIGHT_VEC4 4
//#if N_POINT_LIGHT > 0
uniform vec4 u_pointLights[N_POINT_LIGHT_VEC4 * N_POINT_LIGHT];
#define pointLights(i) PointLight(vec3(u_pointLights[i * N_POINT_LIGHT_VEC4]), \
                vec3(u_pointLights[i * N_POINT_LIGHT_VEC4 + 1]), \
                vec3(u_pointLights[i * N_POINT_LIGHT_VEC4 + 2]), \
                vec3(u_pointLights[i * N_POINT_LIGHT_VEC4 + 3]), \
                u_pointLights[i * N_POINT_LIGHT_VEC4 + 1].w, \
                u_pointLights[i * N_POINT_LIGHT_VEC4 + 2].w, \
                u_pointLights[i * N_POINT_LIGHT_VEC4 + 3].w)
#endif
//#define nbPointLights u_pointLights[0].w

#define N_LIGHT (N_SPOT_LIGHT + N_POINT_LIGHT + N_DIR_LIGHT)


//// ------------------------------------ MATERIAL
//struct Material {
//    vec3 diffuse;
//    vec3 specular;
//    vec3 ambient;
//    float shininess;
//};
////Material material;
//SAMPLER2D(s_diffuse, 0);
//SAMPLER2D(s_opacity, 1);
//uniform vec4 u_material[4];
//#define material_diffuse u_material[0].xyz
//#define material_specular u_material[1].xyz
//#define material_ambient u_material[2].xyz
//#define material_shininess u_material[3].x
//#define material_hasDiffuseTexture u_material[3].y
//#define material_hasOpacityTexture u_material[3].z

// ------------------------------------ SHADOW MAPS
//#ifdef HAS_SHADOW
//SAMPLER2D(s_shadowMap_light_0, 4);
//SAMPLER2D(s_shadowMap_light_1, 5);
SAMPLER2D(s_shadowMap_dirLight_0, 4);
SAMPLER2D(s_shadowMap_dirLight_1, 5);
//#endif
//SAMPLER2D(s_shadowMap_light_2, 6);

uniform mat4 u_lightSpaceMatrix;


// ------------------------------------ OTHERS
uniform vec4 u_viewPos;
#define viewPos u_viewPos.xyz

uniform mat4 u_invModel;
//SAMPLER2D(s_shadowMap, 3);


