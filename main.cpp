#include <bits/stdc++.h>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "Renderer.h"
#include "Camera.h"
#include "tool/model_loader.h"
using namespace std;

const int SCREEN_W = 1680;
const int SCREEN_H = 960;

Transform playerTrans; // 玩家transform(辅助相机)
Camera *camera;
Scene *scene;
Renderer *renderer;

PointLight *light;
Instance *terrain;
Instance *lightbox, *light_center;
Material *terrain_mat[4]; //四种材质
int terrain_mat_id = 0;

//鼠标移动回调函数
float lastTime, detaTime = 0;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static double mouse_lastX = xpos, mouse_lastY = ypos;

    //这里应该是 以自身为中心，按世界的y轴旋转，故在自身坐标系下表示世界y轴
    vec3 hvec = glm::inverse(camera->view.transmat) * glm::vec4(0, -1, 0, 0);
    camera->view.rotate(((xpos - mouse_lastX) * detaTime) * 0.7, hvec);
    playerTrans.rotate(((xpos - mouse_lastX) * detaTime) * 0.7, vec3(0, -1, 0));

    camera->view.rotate((float)((ypos - mouse_lastY) * detaTime), vec3(-1, 0, 0));
    mouse_lastX = xpos;
    mouse_lastY = ypos;
}

/*
 * 处理按键
 * WASD Left-Shift Space 控制视点运动
 * 0123 切换显示方式（原色，单色层，双色层，三色层）
 * 上下左右键调整光源位置（黄色亮方块指示）
 * O 键旋转DEM
 * ESC 键退出
 */
void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_W)) playerTrans.translate(vec3(0, 0, -4 * detaTime));
    if(glfwGetKey(window, GLFW_KEY_S)) playerTrans.translate(vec3(0, 0, 4 * detaTime));
    if(glfwGetKey(window, GLFW_KEY_D)) playerTrans.translate(vec3(4 * detaTime, 0, 0));
    if(glfwGetKey(window, GLFW_KEY_A)) playerTrans.translate(vec3(-4 * detaTime, 0, 0));
    if(glfwGetKey(window, GLFW_KEY_SPACE)) playerTrans.translate(vec3(0, 4 * detaTime, 0));
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) playerTrans.translate(vec3(0, -4 * detaTime, 0));

    if(glfwGetKey(window, GLFW_KEY_UP)) lightbox->transform.translate(vec3(0, 0, 4 * detaTime));
    if(glfwGetKey(window, GLFW_KEY_DOWN)) lightbox->transform.translate(vec3(0, 0, -4 * detaTime));
    if(glfwGetKey(window, GLFW_KEY_LEFT)) lightbox->transform.rotate(detaTime, vec3(3, 0, 0), PARENT_SPACE);
    if(glfwGetKey(window, GLFW_KEY_RIGHT)) lightbox->transform.rotate(detaTime, vec3(-3, 0, 0), PARENT_SPACE);

    if(glfwGetKey(window, GLFW_KEY_O)) terrain->transform.rotate(detaTime, vec3(0, 1, 0));

    if(glfwGetKey(window, GLFW_KEY_0)) terrain_mat_id = 0;
    if(glfwGetKey(window, GLFW_KEY_1)) terrain_mat_id = 1;
    if(glfwGetKey(window, GLFW_KEY_2)) terrain_mat_id = 2;
    if(glfwGetKey(window, GLFW_KEY_3)) terrain_mat_id = 3;

    terrain->material = terrain_mat[terrain_mat_id];

    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);

    camera->view.setPosition(playerTrans.position());
    light->transform = lightbox->transform_global();
}

//初始化窗口（采用默认设置）
GLFWwindow* initWindow() {
    glfwInit();
    //设置各种选项值
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心渲染模式

    //创建窗口，放入上下文中
    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "My Window", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    //openGL本质是一个巨大的状态机，很多内容都是通过设置来完成的
    //注册回调函数
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

    renderer = new Renderer;
    scene = new Scene();
    scene->use();
    camera = Camera::create_perspective_camera(glm::radians(45.0f), 1.0 * SCREEN_W / SCREEN_H, 0.1f, 1000);
    camera->use();

    light = new PointLight(vec3(1.5, 1.5, 1.5));
    light->decay = vec3(1.0, 0.01, 0.0037);
    scene->add_light(light);
    scene->add_light(new SurroundLight(vec3(0.2, 0.2, 0.2)));

    // terrain
    terrain = new Instance;
    float low_height, max_height;
    terrain->mesh = new Mesh(load_dem("../terrain/terrain1.dem", low_height, max_height));
    scene->add_child(terrain);

    // 原色
    terrain_mat[0] = new Material;
    terrain_mat[0]->shader_file = "../shader/standard";
    terrain_mat[0]->diffuse_map = new Texture("../terrain/terrain1.bmp");

    // 三种色层
    for(int i = 1;i <= 3;i++){
        terrain_mat[i] = new Material;
        terrain_mat[i]->shader_file = "../shader/color_level";
        terrain_mat[i]->extend["color_level[0]"] = vec3(1, 1, 0.3);
        terrain_mat[i]->extend["color_level[1]"] = vec3(0.4, 1, 0.3);
        terrain_mat[i]->extend["color_level[2]"] = vec3(0.3, 0.7, 1);
        terrain_mat[i]->extend["low_height"] = low_height;
        terrain_mat[i]->extend["max_height"] = max_height;
        terrain_mat[i]->extend["height_level"] = i;
    }

    terrain->material = terrain_mat[0];

    // 指示光源的箱子
    lightbox = new Instance;
    lightbox->mesh = new Mesh(BOX_MESH_SOURCE);
    lightbox->material = new Material;
    lightbox->material->use_light = false;
    lightbox->material->diffuse_color = vec3(1, 1, 1);
    lightbox->transform.setPosition(vec3(0, 0, 10));
    lightbox->transform.scale(vec3(1, 1, 1));

    light_center = new Instance;
    light_center->transform.setPosition(vec3(0, max_height, 0));
    light_center->add_child(lightbox);
    scene->add_child(light_center);

    // 设置初始视点：dem正上方，向下看
    playerTrans.setPosition(vec3(0, max_height + 4, 0));
    camera->view.setLookAt(vec3(0, 1e9, 0), vec3(0, 0, 1));

    while(!glfwWindowShouldClose(window)) {
        //渲染部分
        //将颜色缓冲clear，即变成背景颜色
        //将深度缓冲clear，删掉前一帧的深度信息
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer->render();

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
