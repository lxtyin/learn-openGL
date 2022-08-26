#version 330 core
out vec4 FragColor;

//在观察空间下运算
uniform sampler2D texture1;
uniform vec3 lightCol = vec3(0.9882, 1.0, 0.9333);
uniform float ambientStrength = 0.1;
uniform float specularStrength = 0.5;

in vec3 normal; //已标准化法线
in vec3 fragPos;
in vec2 texPos;
in vec3 lightPos;

void main() {
    vec3 ambient = ambientStrength * lightCol;

    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 diffuse = max(dot(normal, lightDir), 0) * lightCol;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, normal)); //方向为入射，法线，出射
    vec3 specular = pow(max(dot(reflectDir, viewDir), 0), 16) * specularStrength * lightCol; //越大光亮越集中

    vec3 selfCol =  texture(texture1, texPos).xyz;
    FragColor = vec4(selfCol * (ambient + diffuse + specular), 1.0);
}