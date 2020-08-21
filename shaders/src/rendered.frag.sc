$input v_pos // vec3
$input v_normal // vec3
$input v_texcoord0 // vec2
//$input v_view // vec3
//$input v_posLightSpace

//#if N_SPOT_LIGHT > 0
$input v_posLightSpace_0
$input v_posLightSpace_1
//#endif

//$input v_posLightSpace_2
//$input v_shadowCoord

#include <bgfx_shader.sh>
#include "shaderlib.sh" // do not include shaderlib.sh before bgfx_shader.sh

#include "light.sc"
#include "material.sc"

// function prototypes
#if N_DIR_LIGHT > 0
vec3 CalcDirLight(int iLight, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace);
#endif
#if N_POINT_LIGHT > 0
vec3 CalcPointLight(int iLight, vec3 normal, vec3 fragPos, vec3 viewDir);
#endif
#if N_SPOT_LIGHT > 0
vec3 CalcSpotLight(int iLight, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace);
#endif

void main()
{
    //    if (nbSpotLights != 1)
    //    if (nbDirLights != 1)
    //    if (nbPointLights != 1) {
    //        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    //        return;
    //    }
    //    gl_FragColor =  vec4(v_posLightSpace.xyz, 1.0);
    //    return;

    if (material_hasOpacityTexture > -0.5) {
        float opacity = texture2D(s_opacity, v_texcoord0).r;
        if (opacity < 0.1) {
            discard;
        }
    }

    //    SpotLight spotLights[N_SPOT_LIGHT];
    //    for (int i =0; i <N_SPOT_LIGHT; ++i) {
    //        spotLights[i] = SpotLight(vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 1]),
    //                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 2]),
    //                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 3]),
    //                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 4]),
    //                vec3(u_spotLights[i * N_SPOT_LIGHT_VEC4 + 5]),
    //                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 1].w,
    //                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 2].w,
    //                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 3].w,
    //                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 4].w,
    //                u_spotLights[i * N_SPOT_LIGHT_VEC4 + 5].w);
    //    }

    //    DirLight dirLights[N_DIR_LIGHT];
    //    for (int i =0; i <N_DIR_LIGHT; ++i) {
    ////        dirLights[i] = DirLight(vec3(u_dirLights[i * 2]), vec3(u_dirLights[i * 2 + 1]));
    //        dirLights[i] = DirLight(vec3(u_dirLights[i * N_DIR_LIGHT_VEC4]),
    //                vec3(u_dirLights[i * N_DIR_LIGHT_VEC4 + 1]),
    //                vec3(u_dirLights[i * N_DIR_LIGHT_VEC4 + 2]),
    //                vec3(u_dirLights[i * N_DIR_LIGHT_VEC4 + 3]));
    //    }

    //    PointLight pointLights[N_POINT_LIGHT];
    //    for (int i =0; i <N_POINT_LIGHT; ++i) {
    //        pointLights[i] = PointLight(vec3(u_pointLights[i * N_POINT_LIGHT_VEC4]),
    //                vec3(u_pointLights[i * N_POINT_LIGHT_VEC4 + 1]),
    //                vec3(u_pointLights[i * N_POINT_LIGHT_VEC4 + 2]),
    //                vec3(u_pointLights[i * N_POINT_LIGHT_VEC4 + 3]),
    //                u_pointLights[i * N_POINT_LIGHT_VEC4 + 1].w,
    //                u_pointLights[i * N_POINT_LIGHT_VEC4 + 2].w,
    //                u_pointLights[i * N_POINT_LIGHT_VEC4 + 3].w);
    //    }

    vec3 color;
    if (material_hasDiffuseTexture > -0.5) {
        vec4 tex = texture2D(s_diffuse, v_texcoord0);
        if (tex.w < 0.5)
            discard;

        color = toLinear(tex).xyz;

        //                color = texture2D(s_diffuse, v_texcoord0).xyz;
//        color = toLinear(texture2D(s_diffuse, v_texcoord0)).xyz;
    } else {
        color = material_diffuse;
    }

    // properties
    vec3 norm = normalize(v_normal);
    vec3 viewDir = normalize(viewPos - v_pos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = vec3_splat(0.0);
    int iLight = 0;
    int i;
#if N_DIR_LIGHT > 0
//    for (int i = 0; i < N_DIR_LIGHT; i++)
    result = CalcDirLight(0, norm, viewDir, v_posLightSpace_0);
#endif
        // phase 2: point lights
#if N_POINT_LIGHT > 0
    for (int i = 0; i < N_POINT_LIGHT; i++)
        result += CalcPointLight(i, norm, v_pos, viewDir, v_posLightSpace_0);
#endif
        // phase 3: spot light
#if N_SPOT_LIGHT > 0
    result += CalcSpotLight(0, norm, v_pos, viewDir, v_posLightSpace_0);
    #if N_SPOT_LIGHT > 1
        result += CalcSpotLight(1, norm, v_pos, viewDir, v_posLightSpace_1);
    #endif
#endif

    result *= color;

    gl_FragColor = vec4(result, 1.0);
    gl_FragColor = toGamma(gl_FragColor);
}

vec3 phongSpecular(vec3 normal, vec3 viewDir, vec3 lightDir, vec3 lightSpecular, float shininess)
{
    vec3 reflectDir = reflect(-lightDir, normal);
    return lightSpecular * pow(max(dot(viewDir, reflectDir), 0.0), shininess);
}

vec3 blinnPhongSpecular(vec3 normal, vec3 viewDir, vec3 lightDir, vec3 lightSpecular, float shininess)
{
    vec3 halfwayDir = normalize(lightDir + viewDir);
    return lightSpecular * pow(max(dot(normal, halfwayDir), 0.0), shininess);
}
#define specularTerm phongSpecular
//#define specularTerm blinnPhongSpecular

#if N_DIR_LIGHT > 0
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, DirLight light, int iLight)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //    if (projCoords.x > 1.0 || projCoords.x < 0.0 || projCoords.y > 1.0 || projCoords.y < 0.0)
    //        return 0.0;

    //    vec3 projCoords = fragPosLightSpace.xyz;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

//        if (projCoords.z > 1.0 || projCoords.z < 0.0)
//            return 0.0;
    //    projCoords.x = 1.0 - projCoords.x;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    //    float closestDepth = texture2D(s_shadowMap, projCoords.xy).r;
    //    float closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap, projCoords.xy));
    float closestDepth;

    if (iLight == 0) {

        closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap_dirLight_0, vec2(projCoords.x, projCoords.y)));
    } else if (iLight == 1) {
        closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap_dirLight_1, vec2(projCoords.x, projCoords.y)));
//    } else {
//        closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap_light_2, vec2(projCoords.x, projCoords.y)));
    }
    //    closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap_light_1, vec2(projCoords.x, projCoords.y)));
    //    float closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap_light_0, vec2(projCoords.x, projCoords.y)));
    //    float depth = unpackRgbaToFloat(texture2D(s_shadowMap, v_texcoord0));
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    //    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
//    float bias = 0.0001;
//        float bias = max(0.00001 * (1.0 - dot(normal, light.direction)), 0.00005);
        float bias = 0.00001 * (1.0 - dot(normal, light.direction));

//    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    // PCF
        float shadow = 0.0;
//        textureSize(s_shadowMap_dirLight_0, 3);
//        vec2 texelSize = vec2_splat(1.0 / 8192.0);
        vec2 texelSize = vec2_splat(1.0 / 16384.0);
        const int size = 1;
        for (int x = -size; x <= size; ++x) {
            for (int y = -size; y <= size; ++y) {
                float pcfDepth = unpackRgbaToFloat(texture2D(s_shadowMap_dirLight_0, projCoords.xy + vec2(x, y) * texelSize));
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        const int side = size * 2 + 1;
        shadow /= side * side;

    return shadow;
}

// calculates the color when using a directional light.
vec3 CalcDirLight(int iLight, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace)
{
//    DirLight light = dirLights(iLight);
//    spotLights(light, iLight);
    dirLights(light, iLight);
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
    //    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    // combine results
    //    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    //    vec3 specular = light.specular * spec;
    vec3 specular = specularTerm(normal, viewDir, lightDir, light.specular, material_shininess);
//    return (ambient + diffuse + specular);


    float shadow = 0.0;
    shadow = ShadowCalculation(fragPosLightSpace, normal, light, iLight);

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
    return lighting;
}
#endif

#if N_POINT_LIGHT > 0
// calculates the color when using a point light.
vec3 CalcPointLight(int iLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    PointLight light = pointLights(i);
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    //    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light._linear * distance + light.quadratic * (distance * distance));
    // combine results
    //    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    //    vec3 specular = light.specular * spec;
    vec3 specular = specularTerm(normal, viewDir, lightDir, light.specular, material_shininess);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
#endif

//#define N_SPOT_LIGHT 1
#if N_SPOT_LIGHT > 0
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, SpotLight light, int iLight)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //    if (projCoords.x > 1.0 || projCoords.x < 0.0 || projCoords.y > 1.0 || projCoords.y < 0.0)
    //        return 0.0;

    //    vec3 projCoords = fragPosLightSpace.xyz;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

//        if (projCoords.z > 1.0 || projCoords.z < 0.0)
//            return 0.0;
    //    projCoords.x = 1.0 - projCoords.x;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    //    float closestDepth = texture2D(s_shadowMap, projCoords.xy).r;
    //    float closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap, projCoords.xy));
    float closestDepth;

    if (iLight == 0) {
        closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap_spotLight_0, vec2(projCoords.x, projCoords.y)));
    }
#if N_SPOT_LIGHT > 1
    else if (iLight == 1) {
        closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap_spotLight_1, vec2(projCoords.x, projCoords.y)));
//    } else {
//        closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap_light_2, vec2(projCoords.x, projCoords.y)));
    }
#endif
    //    closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap_light_1, vec2(projCoords.x, projCoords.y)));
    //    float closestDepth = unpackRgbaToFloat(texture2D(s_shadowMap_light_0, vec2(projCoords.x, projCoords.y)));
    //    float depth = unpackRgbaToFloat(texture2D(s_shadowMap, v_texcoord0));
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    //    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
//    float bias = 0.0001;
//        float bias = max(0.00001 * (1.0 - dot(normal, light.direction)), 0.00005);
        float bias = 0.00001 * (1.0 - dot(normal, light.direction));
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // PCF
    //    float shadow = 0.0;
    //    textureSize(s_shadowMap, 3);
    //    vec2 texelSize = vec2_splat(1.0 / 512.0);
    //    const int size = 1;
    //    for (int x = -size; x <= size; ++x) {
    //        for (int y = -size; y <= size; ++y) {
    //            float pcfDepth = unpackRgbaToFloat(texture2D(s_shadowMap, projCoords.xy + vec2(x, y) * texelSize));
    //            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    //        }
    //    }
    //    const int side = size * 2 + 1;
    //    shadow /= side * side;

    return shadow;
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(int iLight, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace)
{
//    SpotLight light;
    spotLights(light, iLight);
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    //    vec3 reflectDir = reflect(-lightDir, normal);
    //    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    //    float spec = specularTerm(normal, viewDir, lightDir, material_shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light._linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    //    epsilon *= 10;
    //    float temp = (theta - light.outerCutOff) / epsilon;
    //    float intensity = clamp(temp, 0.0, 1.0);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //    intensity = intensity * intensity;
    // combine results
    //    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    //    vec3 specular = light.specular * spec;
    vec3 specular = specularTerm(normal, viewDir, lightDir, light.specular, material_shininess);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    //    float shadow = ShadowCalculation(v_posLightSpace);
    float shadow = 0.0;
//    if (distance > 10.0)
        shadow = ShadowCalculation(fragPosLightSpace, normal, light, iLight);

    //    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w * 0.5 + 0.5;
    //    if (projCoords.x > 1.0 || projCoords.x < 0.0 || projCoords.y > 1.0 || projCoords.y < 0.0)
    //        return vec3(1.0, 0.0, 0.0);
    //        continue;
    //        0;
    //    else
    //        return vec3(1.0, 0.0, 0.0);

    //    if (fragPosLightSpace.x > 1.0 || fragPosLightSpace.x < -1.0 || fragPosLightSpace.y > 1.0 || fragPosLightSpace.x < -1.0) {
    //        return vec3(1.0, 0.0, 0.0);
    //    }
    //    return vec3_splat(shadow);
    //    vec3 lighting = ambient + diffuse + specular;
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
    //    return (ambient + diffuse + specular);
    return lighting;
}
#endif
