#version 330 core
out vec4 FragColor;

//在世界坐标下运算
in vec3 normal; //已标准化法线
in vec3 fragPos;
in vec2 texPos;
uniform vec3 viewPos; //观察者位置

//物体材质，反映物体本身性质
struct Material{
    sampler2D diffuse; // 漫反射颜色 = 环境光颜色 = 物体本身颜色，强度差别取决于光照设定
    sampler2D specular;   // 镜面反射颜色，通常为一定比例的白色
    float spininess; // 反光度
};
uniform Material material;

// 光源类
struct Light{
    //三种类型的光源 0-Parallel 1-Point 2-Spot
    int type;
    //携带信息
    vec3 direction;
    vec3 position;
    vec3 decay;
    float inCut, outCut; //两个圆锥边界的余弦
    //共同信息 光品质
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light[16];
uniform int light_N;

float decayValue(Light light){
    float dist = length(light.position - fragPos);
    vec3 tmp = vec3(1.0, dist, dist * dist) * light.decay;
    return 1.0 / (tmp.x + tmp.y + tmp.z);
}

vec3 calcuColor(Light l){
    vec3 lightDir;      //指向光源的向量
    float distDecay;    //距离衰减（除平行光均有）
    float angleDecay;   //角度衰减（仅聚光）
    if(l.type == 0){
        lightDir = normalize(-l.direction);
        distDecay = 1;
        angleDecay = 1;
    }else if(l.type == 1){
        lightDir = normalize(l.position - fragPos);
        distDecay = decayValue(l);
        angleDecay = 1;
    }else{
        lightDir = normalize(l.position - fragPos);
        distDecay = decayValue(l);
        float cs = dot(lightDir, normalize(-l.direction));
        angleDecay = clamp((cs - l.outCut) / (l.inCut - l.outCut), 0.0, 1.0);
    }
    vec3 curDiffuseCol = vec3(texture(material.diffuse, texPos));
    vec3 curSpecularCol = vec3(texture(material.specular, texPos));
    vec3 ambient = l.ambient * curDiffuseCol;
    vec3 diffuse = l.diffuse * curDiffuseCol * max(dot(lightDir, normal), 0);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, normal)); //方向为入射，法线，出射   
    vec3 specular = l.specular * curSpecularCol * pow(max(dot(reflectDir, viewDir), 0), material.spininess);
    return (ambient + diffuse + specular) * distDecay * angleDecay;
}


void main() {
    vec3 result = vec3(0, 0, 0);
    for(int i = 0; i < light_N; i++) result += calcuColor(light[i]);
    FragColor = vec4(result, 1.0);
}