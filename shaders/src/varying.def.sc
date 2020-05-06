// vec4 v_color0    : COLOR0    = vec4(1.0, 1.0, 0.0, 1.0);
vec3 v_normal    : NORMAL    = vec3(0.0, 0.0, 1.0);
vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);
vec3 v_fragPos       : TEXCOORD1 = vec3(0.0, 0.0, 0.0);
vec3 v_view      : TEXCOORD2 = vec3(0.0, 0.0, 0.0);
//vec3 v_fragPos      : TEXCOORD3 = vec3(0.0, 0.0, 0.0);
//vec3 v_weyepos : TEXCOORD3 = vec3(0.0, 0.0, 0.0);
vec4 v_position : TEXCOORD4;
//vec4 v_fragPosLightSpace : TEXCOORD5;
vec4 v_fragPosLightSpace_0 : TEXCOORD5;
vec4 v_fragPosLightSpace_1 : TEXCOORD6;
vec4 v_fragPosLightSpace_2 : TEXCOORD7;
//vec4 v_shadowCoord : TEXCOORD6;

vec3 a_position  : POSITION;
// vec4 a_color0    : COLOR0;
vec3 a_normal    : NORMAL;
//vec3 a_normal    : TEXCOORD1;
vec2 a_texcoord0 : TEXCOORD0;
