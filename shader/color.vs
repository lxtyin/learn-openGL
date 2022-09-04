#version 330 core
layout (location = 0) in vec3 aPos;   // 位置

// LOCAL GLOBAL VIEW
uniform mat4 model;  //模型矩阵 l2g
uniform mat4 view;   //观察矩阵 g2v
uniform mat4 projection;   //投影矩阵，v2裁剪空间

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}