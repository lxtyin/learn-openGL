#version 330 core
out vec4 FragColor;

//在观察空间下运算

//光照颜色、配比，此处先写死单一光源
struct Light{
    vec3 position;
    vec3 ambient; //这边的颜色均已包含配比系数
    vec3 diffuse;
    vec3 specular;

    vec3 decay; //衰减的三个参数
};
uniform Light light;

//物体材质，反映物体本身性质
struct Material{
    sampler2D diffuse; // 漫反射颜色 = 环境光颜色 = 物体本身颜色，强度差别取决于光照设定
    sampler2D specular;   // 高光贴图
    float spininess; // 反光度
};
uniform Material material;

in vec3 normal; //已标准化法线
in vec3 fragPos;
in vec2 texPos;

float decayValue(){
    float distance = length(light.position - fragPos);
    vec3 tmp = vec3(1.0, distance, distance * distance) * light.decay;
    return 1.0 / (tmp.x + tmp.y + tmp.z);
}

void main() {
    vec3 curDiffuseCol = vec3(texture(material.diffuse, texPos));
    vec3 curSpecularCol = vec3(texture(material.specular, texPos));

    vec3 ambient = light.ambient * curDiffuseCol;
    
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 diffuse = light.diffuse * curDiffuseCol * max(dot(lightDir, normal), 0);

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, normal)); //方向为入射，法线，出射
    vec3 specular = light.specular * curSpecularCol * pow(max(dot(reflectDir, viewDir), 0), material.spininess);

    FragColor = vec4((ambient + diffuse + specular) * decayValue(), 1.0);
}