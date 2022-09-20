#include <bits/stdc++.h>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Light.h"
#include "tool.h"

using namespace std;

double lastTime, detaTime = 0;
int screenWidth = 800, screenHeight = 800;

Transform viewTrans, playerTrans;
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
    glm::vec3 hvec = glm::inverse(viewTrans.transmat) * glm::vec4(0, -1, 0, 0);
    viewTrans.rotate((float)((xpos - lastX) * detaTime) * 0.7, hvec);
    playerTrans.rotate((float)((xpos - lastX) * detaTime) * 0.7, 0, -1, 0);

    viewTrans.rotate((float)((ypos - lastY) * detaTime), -1, 0, 0);
    lastX = xpos;
    lastY = ypos;
}

//处理按键
SpotLight light(glm::vec3(1, 1, 1), 13, 15);
void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_W)) playerTrans.translate(0, 0, -4 * detaTime);
    if(glfwGetKey(window, GLFW_KEY_S)) playerTrans.translate(0, 0, 4 * detaTime);
    if(glfwGetKey(window, GLFW_KEY_D)) playerTrans.translate(4 * detaTime, 0, 0);
    if(glfwGetKey(window, GLFW_KEY_A)) playerTrans.translate(-4 * detaTime, 0, 0);
    if(glfwGetKey(window, GLFW_KEY_SPACE)) playerTrans.translate(0, 4 * detaTime, 0);
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) playerTrans.translate(0, -4 * detaTime, 0);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);
    if(glfwGetKey(window, GLFW_KEY_J)) light.angleIn += 0.02, light.angleOut += 0.02;
    if(glfwGetKey(window, GLFW_KEY_K)) light.angleIn -= 0.02, light.angleOut -= 0.02;

    viewTrans.setPosition(playerTrans.position());
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

    float dataSource2[] = {
        //pos              
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f,  0.5f, 
        0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO); //建立对象
    glGenBuffers(1, &VBO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dataSource2), dataSource2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);//unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Shader lightshader("../shader/color.vs", "../shader/color.fs");

    Texture img1("../pic/box.png", GL_RGBA);
    Texture img2("../pic/box_edge.png", GL_RGBA);
    img1.activeTarget(0);
    img2.activeTarget(1);
    Shader myshader("../shader/simple_light.vs", "../shader/simple_light.fs");

    light.direction = vec3(0, 0, -1);
    light.decay = vec3(1.0, 0.014, 0.0007);
    ParallelLight light2(glm::vec3(1, 1, 1));

    std::vector<Transform> objs;
    auto rd = []() {return rand() % 6 - 3;};
    Transform tmp;
    for(int i = 1;i <= 30;i++) {
        tmp.translate(rd(), rd(), rd());
        tmp.rotate(rand() % 60 - 30, rd(), rd(), rd());
        objs.push_back(tmp);
    }

    while(!glfwWindowShouldClose(window)) {
        
        //渲染部分
        //将颜色缓冲clear，即变成背景颜色
        //将深度缓冲clear，删掉前一帧的深度信息
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myshader.use();
        //设置物体本身材质
        myshader.setInt("material.diffuse", 0);
        myshader.setInt("material.specular", 1);
        myshader.setFloat("material.spininess", 16);

        //传入必要的变换矩阵
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
        myshader.setMat4("view", glm::inverse(viewTrans.transmat));
        myshader.setMat4("projection", projection);
        myshader.setVec3("viewPos", viewTrans.position());

        //应用光照
        Light::applyAllLightTo(myshader);

        for(int i = 0;i < 30;i++) {
            myshader.setMat4("model", objs[i].transmat);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        light.transform = viewTrans;
        // lightshader.use();
        // lightshader.setVec3("color", light.specular);
        // lightshader.setMat4("model", light.transform.transmat);
        // lightshader.setMat4("view", glm::inverse(viewTrans.transmat));
        // lightshader.setMat4("projection", projection);
        // glBindVertexArray(lightVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

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
