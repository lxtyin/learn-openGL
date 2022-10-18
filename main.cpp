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
double mouse_lastX = -1e9, mouse_lastY = -1e9;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if(mouse_lastX < -1e8) { //first move
        mouse_lastX = xpos;
        mouse_lastY = ypos;
    }

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
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);

    // if(glfwGetKey(window, GLFW_KEY_J)) light.angleIn += 0.02, light.angleOut += 0.02;
    // if(glfwGetKey(window, GLFW_KEY_K)) light.angleIn -= 0.02, light.angleOut -= 0.02;

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
    

    vector<Vertex> boxdata{
            //pos                //nor                //tex
            {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 0},
            {0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1, 0},
            {0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1, 1},
            {0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1, 1},
            {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 1},
            {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 0},

            {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 0},
            {0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1, 0},
            {0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1, 1},
            {0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1, 1},
            {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 1},
            {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 0},

            {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 0},
            {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 0},
            {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 1},
            {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 1},
            {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 1},
            {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 0},

            {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0, 0},
            {0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1, 0},
            {0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1, 1},
            {0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1, 1},
            {0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0, 1},
            {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0, 0},

            {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0, 0},
            {0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,    1, 0},
            {0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1, 1},
            {0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1, 1},
            {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0, 1},
            {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0, 0},

            {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0, 0},
            {0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,    1, 0},
            {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    1, 1},
            {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    1, 1},
            {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0, 1},
            {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0, 0}
    };
    Mesh objBox(boxdata);
    Transform boxTrans;
    Shader lightshader("../shader/color.vs", "../shader/color.fs");
    
    Transform terrainTrans;
    Dem_Mesh terrain = dem_loader("../terrain/terrain2.dem");
    Texture terrainTexture("../terrain/terrain2.bmp", GL_RGB);
    Texture terrainTexture2("../terrain/terrain2.bmp", GL_RGB);
    terrainTexture.activeTarget(0);
    terrainTexture2.activeTarget(1);
    Shader myshader("../shader/simple_light.vs", "../shader/simple_light.fs");

    //设置初始视点：dem正上方，向下看
    playerTrans.setPosition(0, terrain.mx_height + 10, 0);
    viewTrans.setPosition(0, terrain.mx_height + 10, 0);
    viewTrans.setLookAt(glm::vec3(0, 1e9, 0), glm::vec3(0, 0, 1));

    PointLight light(vec3(2, 2, 1));
    boxTrans.setPosition(3, 1, 3);
    boxTrans.scale(0.3, 0.3, 0.3);
    light.transform = boxTrans;

    ParallelLight light2(glm::vec3(0.1, 0.1, 0.1), glm::vec3(1, -1, 0));

    while(!glfwWindowShouldClose(window)) {
        
        //渲染部分
        //将颜色缓冲clear，即变成背景颜色
        //将深度缓冲clear，删掉前一帧的深度信息
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //shader作为状态机
        myshader.use();
        //设置物体本身材质
        myshader.setInt("material.diffuse", 0);//贴图id
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
        
        myshader.setMat4("model", terrainTrans.transmat);
        terrain.Draw();

        // ---
        lightshader.use();

        lightshader.setMat4("view", glm::inverse(viewTrans.transmat));
        lightshader.setMat4("projection", projection);
        lightshader.setVec3("viewPos", viewTrans.position());

        lightshader.setMat4("model", boxTrans.transmat);
        objBox.Draw();

        terrainTrans.rotate(detaTime * M_PI / 8, 0, 1, 0, LOCAL_SPACE);

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
