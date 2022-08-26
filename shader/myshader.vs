#version 330 core
layout (location = 0) in vec3 aPos;   // 位置
layout (location = 1) in vec3 aNor;   // 法线方向
layout (location = 2) in vec2 aTexpos;// 纹理坐标

out vec2 texPos;   //输出纹理坐标
out vec3 normal;   //输出法线方向
out vec3 fragPos;  //输出为观察坐标下位置
out vec3 lightPos; 

uniform vec3 lightWorldPos = vec3(2, 9, 3); //光源世界位置 暂定
// LOCAL GLOBAL VIEW
uniform mat4 model;  //模型矩阵 l2g
uniform mat4 view;   //观察矩阵 g2v
uniform mat4 projection;   //投影矩阵，v2裁剪空间

void main() {
    mat4 vm = view * model;
    gl_Position = projection * vm * vec4(aPos, 1.0); //顶点着色器必须给出的一项
    normal = normalize(mat3(transpose(inverse(vm))) * aNor); //法线变换
    fragPos = vec3(vm * vec4(aPos, 1.0));
    lightPos = vec3(view * vec4(lightWorldPos, 1.0));
    texPos = aTexpos;
}