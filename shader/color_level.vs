
layout (location = 0) in vec3 aPos;   // 位置
layout (location = 1) in vec3 aNor;   // 法线方向
layout (location = 2) in vec2 aTexpos;// 纹理坐标

out vec3 frag_pos;

// LOCAL GLOBAL VIEW
uniform mat4 model;  //模型矩阵 l2g
uniform mat4 view;   //观察矩阵 v2g
uniform mat4 projection;   //投影矩阵，v2裁剪空间

void main() {
    frag_pos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * inverse(view) * model * vec4(aPos, 1.0);
}