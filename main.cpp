#include <bits/stdc++.h>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
using namespace std;

double lastTime, detaTime = 0;
int screenWidth = 800, screenHeight = 800;

Transform viewTrans, objTrans;
//窗口大小更改时的回调函数
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    screenWidth = width;
    screenHeight = height;
    cout << "view changed! " << width << ' ' << height << '\n';
    glViewport(0, 0, width, height);
}

//鼠标移动回调函数
double lastX = 400, lastY = 400;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    //这里应该是 以自身为中心，按世界的y轴旋转，故在自身坐标系下表示世界y轴
    glm::vec3 hvec = glm::inverse(viewTrans.transform) * glm::vec4(0, -1, 0, 0);
    viewTrans.rotate((float)((xpos - lastX) * detaTime) * 0.7, hvec);
    
    viewTrans.rotate((float)((ypos - lastY) * detaTime), -1, 0, 0);
    lastX = xpos;
    lastY = ypos;
}

//处理按键
void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_W)) viewTrans.translate(0, 0, -4 * detaTime);
    if(glfwGetKey(window, GLFW_KEY_S)) viewTrans.translate(0, 0, 4 * detaTime);
    if(glfwGetKey(window, GLFW_KEY_D)) viewTrans.translate(4 * detaTime, 0, 0);
    if(glfwGetKey(window, GLFW_KEY_A)) viewTrans.translate(-4 * detaTime, 0, 0);
    if(glfwGetKey(window, GLFW_KEY_SPACE)) viewTrans.translate(0, 4 * detaTime, 0, WORLD_SPACE);
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) viewTrans.translate(0, -4 * detaTime, 0, WORLD_SPACE);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, const char * argv[]) {

    glfwInit();
    //设置各种选项值
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心渲染模式

    //创建窗口，放入上下文中
    GLFWwindow *window = glfwCreateWindow(800, 800, "My Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    //初始化glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    //openGL本质是一个巨大的状态机，很多内容都是通过设置来完成的
    //注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    //设置清空屏幕用的颜色，即背景颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    //启用深度测试
    glEnable(GL_DEPTH_TEST);

    //设置鼠标模式
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float dataSource1[] = {
            //pos                //nor                //tex
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 0,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1, 0,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1, 1,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1, 1,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 1,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 0,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 0,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1, 0,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1, 1,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1, 1,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 1,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 0,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 0,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 0,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 1,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 1,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 1,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 0,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0, 0,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1, 0,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1, 1,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1, 1,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0, 1,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0, 0,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0, 0,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,    1, 0,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1, 1,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1, 1,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0, 1,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0, 0,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0, 0,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,    1, 0,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    1, 1,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    1, 1,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0, 1,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0, 0
    };

    unsigned int VBO, VAO; //存储对象ID
    glGenVertexArrays(1, &VAO); //建立对象
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dataSource1), dataSource1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //这里最好先解除VAO的绑定，再解绑VBO和EBO，先解除VBO好像也对，但先解绑EBO就不对了（暂不理解VAO记录了些什么
    glBindVertexArray(0);//unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    

    Texture img1("../pic/wood.png");
    img1.activeTarget(0);

    //编译，连接写好的shader，得到shaderProgram
    Shader myshader("../shader/myshader.vs", "../shader/myshader.fs");

    objTrans.translate(0, -4, 0);
    while(!glfwWindowShouldClose(window)) {
        
        //渲染部分
        //将颜色缓冲clear，即变成背景颜色
        //将深度缓冲clear，删掉前一帧的深度信息
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myshader.use();
        myshader.setInt("texture1", 0);//uniform sampler2D的值不知道在那一个步骤中会被清掉，每次use都要更新

        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
        myshader.setMat4("model", objTrans.transform);
        myshader.setMat4("view", glm::inverse(viewTrans.transform));
        myshader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        detaTime = glfwGetTime() - lastTime;//更新detaTime
        lastTime += detaTime;
        processInput(window);//处理按键
        glfwSwapBuffers(window); //交换两层颜色缓冲
        glfwPollEvents();//检查有没有发生事件，调用相应回调函数
    }

    //释放所有资源
    glfwTerminate();
    return 0;
}
