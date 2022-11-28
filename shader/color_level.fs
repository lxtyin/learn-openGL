
out vec4 FragColor;

//在世界坐标下运算
in vec2 uv;
in vec3 normal; //已标准化法线
in vec3 frag_pos;
in vec3 view_pos; //观察者位置

vec3 diffuse_color;

uniform float low_height;
uniform float max_height;
uniform int height_level;

uniform vec3 color_level[3];

#ifdef USE_LIGHT

    // 默认配比
    uniform float DIFFUSE_RATE = 0.7;
    uniform float SPECULAR_RATE = 0.5;

    // 光源类
    struct Light{
        //三种类型的光源 0-Parallel 1-Point 2-Spot 3-Surround
        int type;
        //携带信息
        vec3 direction;
        vec3 position;
        vec3 decay;
        float inCut, outCut; //两个圆锥边界的余弦
        //颜色 * 强度
        vec3 color;
    };
    uniform Light light[16];
    uniform int light_N;

    float decayValue(Light light){ //距离衰减
        float dist = length(light.position - frag_pos);
        vec3 tmp = vec3(1.0, dist, dist * dist) * light.decay;
        return 1.0 / (tmp.x + tmp.y + tmp.z);
    }

    vec3 calcuColor(Light l){
        vec3 light_dir;     //指向光源的向量
        float decay;        //衰减比例

        if(l.type == 0){
            light_dir = normalize(-l.direction);
            decay = 1;
        }else if(l.type == 1){
            light_dir = normalize(l.position - frag_pos);
            decay = decayValue(l);
        }else if(l.type == 2){
            light_dir = normalize(l.position - frag_pos);
            float cs = dot(light_dir, normalize(-l.direction));
            float ad = clamp((cs - l.outCut) / (l.inCut - l.outCut), 0.0, 1.0);
            decay = decayValue(l) * ad;
        }else if(l.type == 3){
            return l.color * diffuse_color;
        }

        vec3 result = l.color * diffuse_color * max(dot(light_dir, normal), 0) * DIFFUSE_RATE;
        return result * decay;
    }
#endif


void main() {
    int level = 0;
    float dlt = (max_height - low_height) / height_level;
    for(int i = 0;i < height_level;i++){
        if(frag_pos.y > low_height + dlt * i){
            level++;
        }
    }
    diffuse_color = color_level[level - 1];

#ifdef USE_LIGHT
    vec3 result = vec3(0, 0, 0);
    for(int i = 0; i < light_N; i++) result += calcuColor(light[i]);
    FragColor = vec4(result, 1.0);
#else
    FragColor = vec4(diffuse_color, 1.0);
#endif
}
