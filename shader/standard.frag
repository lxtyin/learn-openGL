
out vec4 FragColor;

//在世界坐标下运算
in vec2 uv;
in vec3 normal; //已标准化法线
in vec3 frag_pos;
in vec3 view_pos; //观察者位置

#ifdef USE_DIFFUSE_MAP
    uniform sampler2D diffuse_map;
#else
    uniform vec3 diffuse_color;
#endif

#ifdef USE_SPECULAR_MAP
    uniform sampler2D specular_map;
    uniform float spininess = 128;
#endif

#ifdef USE_LIGHT

    // 默认配比
    uniform float DIFFUSE_RATE = 0.7;
    uniform float SPECULAR_RATE = 0.5;

    // 光源类
    struct Light{
        //三种类型的光源 0-Directional 1-Point 2-Spot 3-Surround
        int type;
        //携带信息
        vec3 direction;
        vec3 position;
        float inCut, outCut; //两个圆锥边界的余弦
        vec3 emission;
    };
    uniform Light light[16];
    uniform int light_N;

    vec3 calcuColor(Light l){
        vec3 light_dir;     //指向光源的向量
        float decay;        //衰减比例

        #ifdef USE_DIFFUSE_MAP
            vec3 diffuse_color = vec3(texture(diffuse_map, uv));
        #endif

        if(l.type == 0){
            light_dir = normalize(-l.direction);
            decay = 1.0f;
        }else if(l.type == 1){
            light_dir = normalize(l.position - frag_pos);
            float d = length(l.position - frag_pos);
            decay = 1.0 / (d * d);
        }else if(l.type == 2){
            light_dir = normalize(l.position - frag_pos);
            float cs = dot(light_dir, normalize(-l.direction));
            float ad = clamp((cs - l.outCut) / (l.inCut - l.outCut), 0.0, 1.0);
            float d = length(l.position - frag_pos);
            decay = 1.0 / (d * d);
        }else if(l.type == 3){
            return l.emission * diffuse_color;
        }

        vec3 result = l.emission * diffuse_color * max(dot(light_dir, normal), 0.) * DIFFUSE_RATE;
        #ifdef USE_SPECULAR_MAP
            vec3 specular_color = vec3(texture(specular_map, uv));
            vec3 view_dir = normalize(view_pos - frag_pos);
            vec3 half_dir = normalize(light_dir + view_dir);
            vec3 specular = l.emission * specular_color * pow(max(dot(half_dir, normal), 0.), spininess) * SPECULAR_RATE;
            result = result + specular;
        #endif
        return result * decay;
    }
#endif


void main() {
#ifdef USE_LIGHT
    vec3 result = vec3(0, 0, 0);
    for(int i = 0; i < light_N; i++) result += calcuColor(light[i]);
    FragColor = vec4(result, 1.0);
#else
    #ifdef USE_DIFFUSE_MAP
        FragColor = texture(diffuse_map, uv);
    #else
        FragColor = vec4(diffuse_color, 1.0);
    #endif
#endif
}
