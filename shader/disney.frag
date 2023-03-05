
out vec4 FragColor;

//在世界坐标下运算
in vec2 uv;
in vec3 normal;
in vec3 frag_pos;
in vec3 view_pos;

const float PI = 3.1415926;
const float IVPI = 0.3183098;

uniform sampler2D diffuse_map;

uniform vec3 base_color;
uniform float metallic;
uniform float roughness;
uniform float specular;
uniform float specular_tint;
// to be used.
uniform float sheen;
uniform float sheenTint;
uniform float subsurface;
uniform float clearcoat;
uniform float clearcoat_gloss;
uniform float anisotropic;

vec3 get_base_color() {
    #ifdef USE_DIFFUSE_MAP
        return vec3(texture(diffuse_map, uv));
    #endif
    return base_color ;
}

struct Light{
    //三种类型的光源 0-Directional 1-Point 2-Spot 3-Surround
    int type;
    vec3 direction;
    vec3 position;
    float inCut, outCut;
    vec3 emission;
};
uniform Light light[16];
uniform int light_N;

float pow5(float x) {
    float y = x * x;
    return y * y * x;
}
float pow2(float x) {
    return x * x;
}
float SchlickFresnel(float fd90, float t) {
    return 1 + (fd90 - 1) * pow5(1 - t);
}

vec3 BRDF(vec3 wi, vec3 wo, vec3 n){

    if(dot(n, wo) < 0) n = -n;
    if(dot(n, wi) < 0) return vec3(0); // 反向

    vec3 h = normalize(wi + wo);
    float idh = dot(wi, h);
    float idn = dot(wi, n);
    float odn = dot(wo, n);
    float ndh = dot(n, h);

    // diffuse
    float fd90 = roughness * 2 * idh * idh + 0.5;
    float fdi = SchlickFresnel(fd90, idn);
    float fdo = SchlickFresnel(fd90, odn);
    vec3 diffuse = get_base_color() * IVPI * fdi * fdo;

    // specular D
    float a2 = max(0.01f, roughness * roughness);
    float D = a2 / pow2((a2 - 1) * pow2(ndh) + 1) * IVPI;

    // specular F
    vec3 f0 = mix(vec3(0.04), get_base_color(), metallic);
    vec3 F = f0 + (1 - f0) * pow5(1 - idh);

    // specular G
    float k = pow2(1 + roughness) / 8;
    float G = 1 / ((1 - k + k / idn) *  (1 - k + k / odn));

    return mix(diffuse, D * G * F / (4 * idn * odn), metallic);
}


void main() {

    vec3 result = vec3(0, 0, 0);
    for(int i = 0; i < light_N; i++){
        Light l = light[i];
        if(l.type == 3) result += l.emission * get_base_color();
        else {
            vec3 wi = normalize(l.type == 0 ? -l.direction : l.position - frag_pos);
            vec3 wo = normalize(view_pos - frag_pos);
            vec3 col;
            if(l.type == 0) {
                col = l.emission;
            } else {
                float d = length(l.position - frag_pos);
                col = l.emission / (d * d);
            }
            result += BRDF(wi, wo, normal) * col;
        }
    }

    FragColor = vec4(result, 1.0);
}
