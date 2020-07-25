
// ------------------------------------ MATERIAL
struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};
//Material material;
SAMPLER2D(s_diffuse, 0);
SAMPLER2D(s_opacity, 1);
uniform vec4 u_material[4];
#define material_diffuse u_material[0].xyz
#define material_specular u_material[1].xyz
#define material_ambient u_material[2].xyz
#define material_shininess u_material[3].x
#define material_hasDiffuseTexture u_material[3].y
#define material_hasOpacityTexture u_material[3].z

