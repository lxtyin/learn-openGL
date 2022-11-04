#include <bits/stdc++.h>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Light.h"
#include "tool.h"
#include "Mesh.h"
#include "model_loader.h"

using namespace std;

float lastTime, detaTime = 0;
int screenWidth = 800, screenHeight = 800;

Transform viewTrans, playerTrans; //视点transform，玩家transform
ParallelLight parallelLight(glm::vec3(0.8, 0.8, 0.8), glm::vec3(3, -1, 0)); //全局平行光


//窗口大小更改时的回调函数
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    screenWidth = width;
    screenHeight = height;
    cout << "view changed! " << width << ' ' << height << '\n';
    glViewport(0, 0, width, height);
}

//鼠标移动回调函数
double mouse_lastX = -1e9, mouse_lastY = -1e9;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if(mouse_lastX < -1e8) { //first move
        mouse_lastX = xpos;
        mouse_lastY = ypos;
    }

//    cout << "move " << xpos - mouse_lastX << ' ' << ypos - mouse_lastY << '\n';

    //这里应该是 以自身为中心，按世界的y轴旋转，故在自身坐标系下表示世界y轴
    glm::vec3 hvec = glm::inverse(viewTrans.transmat) * glm::vec4(0, -1, 0, 0);
    viewTrans.rotate((float)((xpos - mouse_lastX) * detaTime) * 0.7, hvec);
    playerTrans.rotate((float)((xpos - mouse_lastX) * detaTime) * 0.7, 0, -1, 0);

    viewTrans.rotate((float)((ypos - mouse_lastY) * detaTime), -1, 0, 0);
    mouse_lastX = xpos;
    mouse_lastY = ypos;
}

//处理按键
void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_W)) playerTrans.translate(0, 0, -4 * detaTime);
    if(glfwGetKey(window, GLFW_KEY_S)) playerTrans.translate(0, 0, 4 * detaTime);
    if(glfwGetKey(window, GLFW_KEY_D)) playerTrans.translate(4 * detaTime, 0, 0);
    if(glfwGetKey(window, GLFW_KEY_A)) playerTrans.translate(-4 * detaTime, 0, 0);
    if(glfwGetKey(window, GLFW_KEY_SPACE)) playerTrans.translate(0, 4 * detaTime, 0);
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) playerTrans.translate(0, -4 * detaTime, 0);
    if(glfwGetKey(window, GLFW_KEY_T)){
        parallelLight.direction = glm::point_rotate(parallelLight.direction, 2 * detaTime, vec3(0, -1, 0));
    }
    if(glfwGetKey(window, GLFW_KEY_Y)){
        parallelLight.direction = glm::point_rotate(parallelLight.direction, -2 * detaTime, vec3(0, -1, 0));
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);

    viewTrans.setPosition(playerTrans.position());
}

//初始化窗口（采用默认设置）
GLFWwindow* initWindow() {
    glfwInit();
    //设置各种选项值
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心渲染模式

    //创建窗口，放入上下文中
    GLFWwindow* window = glfwCreateWindow(800, 800, "My Window", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    //openGL本质是一个巨大的状态机，很多内容都是通过设置来完成的
    //注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //设置鼠标模式
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

int main(int argc, const char* argv[]) {

    auto* window = initWindow();
    //初始化glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    
    //设置清空屏幕用的颜色，即背景颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    //启用深度测试
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // 地形
    Transform terrainTrans;
    Dem_Mesh terrainMesh = dem_loader("../terrain/terrain2.dem");
    Texture terrainTexture("../terrain/terrain2.bmp", GL_RGB);
    terrainTexture.activeTarget(0);
    Shader terrainShader("../shader/simple_light.vs", "../shader/simple_light.fs");

    // 设置初始视点：dem正上方，向下看
    playerTrans.setPosition(0, terrainMesh.mx_height + 4, 0);
    viewTrans.setPosition(0, terrainMesh.mx_height + 4, 0);
    viewTrans.setLookAt(glm::vec3(0, 1e9, 0), glm::vec3(0, 0, 1));

    // 一个箱子
    Mesh boxMesh(BOX_MESH_SOURCE);
    Transform boxTrans;
    Shader boxShader("../shader/color.vs", "../shader/color.fs");
    PointLight boxLight(vec3(1, 1, 1));
    boxTrans.setPosition(9, terrainMesh.mx_height + 1, 8);
    boxTrans.scale(0.3, 0.3, 0.3);
    boxLight.transform = boxTrans;

    while(!glfwWindowShouldClose(window)) {
        
        //渲染部分
        //将颜色缓冲clear，即变成背景颜色
        //将深度缓冲clear，删掉前一帧的深度信息
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //shader作为状态机
        terrainShader.use();
        //设置物体本身材质
        terrainShader.setInt("material.diffuse", 0);//贴图id
        terrainShader.setInt("material.specular", 1);
        terrainShader.setFloat("material.spininess", 16);
        //传入必要的变换矩阵
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
        terrainShader.setMat4("view", glm::inverse(viewTrans.transmat));
        terrainShader.setMat4("projection", projection);
        terrainShader.setVec3("viewPos", viewTrans.position());

        //应用光照
        Light::applyAllLightTo(terrainShader);

        terrainShader.setMat4("model", terrainTrans.transmat);
        terrainMesh.Draw();

        // ---
        boxShader.use();

        boxShader.setMat4("view", glm::inverse(viewTrans.transmat));
        boxShader.setMat4("projection", projection);
        boxShader.setVec3("viewPos", viewTrans.position());

        boxShader.setMat4("model", boxTrans.transmat);
        boxMesh.Draw();

        // terrainTrans.rotate(detaTime * M_PI / 8, 0, 1, 0, LOCAL_SPACE);
        boxTrans.rotate(detaTime* M_PI / 8, 0, 1, 0, WORLD_SPACE);
        boxLight.transform = boxTrans;

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
