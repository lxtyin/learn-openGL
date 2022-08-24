#version 330 core
layout (location = 0) in vec3 aPos;   // 位置
layout (location = 1) in vec3 aNor;   // 法线方向
layout (location = 2) in vec2 aTexpos;// 纹理坐标

out vec2 texPos;   //输出纹理坐标
out vec3 normal;   //输出法线方向
out vec3 worldPos; //输出世界位置 让片元知道它在世界的什么地方

uniform mat4 transform;  //物体从局部坐标变到世界坐标的矩阵，相当于物体的位置方向大小等属性
uniform mat4 view;       //从世界坐标变换到观察坐标的位置，相当于摄像机的位置大小方向属性
uniform mat4 projection; //透视投影矩阵，它能忽略掉平头截体之外的顶点，然后给顶点附上w分量，最后所有顶点位置会除去w，得到透视效果

void main() {
    gl_Position = projection * view * transform * vec4(aPos, 1.0); //顶点着色器必须给出的一项
    normal = normalize(aNor);
    worldPos = vec3(transform * vec4(aPos, 1.0));
    texPos = aTexpos;
}