#version 330 core
layout (location = 0) in vec3 aPos;   // 位置
layout (location = 1) in vec3 aNor;   // 法线方向
layout (location = 2) in vec2 aTexpos;// 纹理坐标

out vec2 texPos;   //输出纹理坐标
out vec3 normal;   //输出法线方向
out vec3 fragPos;  //输出世界坐标下位置

// LOCAL GLOBAL VIEW
uniform mat4 model;  //模型矩阵 l2g
uniform mat4 view;   //观察矩阵 g2v
uniform mat4 projection;   //投影矩阵，v2裁剪空间

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0); //顶点着色器必须给出的一项
    normal = normalize(mat3(transpose(inverse(model))) * aNor); //法线变换
    fragPos = vec3(model * vec4(aPos, 1.0));
    texPos = aTexpos;
}