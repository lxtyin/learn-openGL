#version 330 core
out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 lightPos = vec3(1, 1, 0);
uniform vec3 lightCol = vec3(1, 1, 1);
uniform float ambientStrength = 0.2;

in vec3 normal;
in vec3 worldPos;
in vec2 texPos;

void main() {
    vec3 lightDir = normalize(lightPos - worldPos);
    float diffuseStrength = max(dot(normal, lightDir), 0);
    vec3 selfCol =  texture(texture1, texPos).xyz;
    FragColor = vec4((ambientStrength + diffuseStrength) * lightCol * selfCol, 1.0);
}