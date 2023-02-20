#include <bits/stdc++.h>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "src/Renderer.h"
#include "src/Camera.h"
#include "src/tool/loader.h"
using namespace std;

const int SCREEN_W = 1680;
const int SCREEN_H = 960;

GLFWwindow *window;
Camera *camera;
Scene *scene;
Renderer *renderer;

Instance *camera_box;
float detaTime = 0;

vector<pair<Instance*, vec3>> boxes;
glm::mat4 input_transform;
void get_input() {
    ifstream f_m("matrix.txt");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            f_m >> input_transform[i][j];
        }
    }
    f_m.close();
}

void init() {

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    scene = new Scene();
    camera = Camera::create_perspective_camera(glm::radians(45.0f), 1.0 * SCREEN_W / SCREEN_H, 0.1f, 1000);

    renderer = new Renderer(scene, camera);

    camera_box = new Instance;
    scene->add_child(camera_box);
    camera_box->add_child(camera);

    scene->add_light(new SurroundLight(vec3(0.2, 0.2, 0.2)));
    scene->add_light(new DirectionalLight(vec3(0.7, 0.7, 0.7), vec3(0, -1, -2)));

    Mesh *box_mesh = new Mesh(BOX_MESH_SOURCE);

    get_input();
    for(int i = -4;i <= 4; i++){
        for(int j = -4;j <= 4; j++){
            for(int k = -4;k <= 4; k++){
                auto *box = new Instance;
                box->mesh = box_mesh;
                box->material = new Material;
                box->material->diffuse_color = vec3((i + 3.0) / 6, (j + 3.0) / 6, (k + 3.0) / 6);
                box->transform.position = vec3(i, j, k);
                box->transform.scale = vec3(0.1f, 0.1f, 0.1f);
                scene->add_child(box);
                auto tar = vec4(box->transform.position, 1) * input_transform;
                boxes.push_back({box, vec3(tar / tar[3])});
            }
        }
    }

    auto *x_line = new Instance;
    x_line->mesh = new Mesh(BOX_MESH_SOURCE);
    x_line->material = new Material;
    x_line->material->diffuse_color = vec3(1, 0, 0);
    x_line->transform.position = vec3(50, 0, 0);
    x_line->transform.scale = vec3(104, 0.1, 0.1);
    scene->add_child(x_line);

    auto *y_line = new Instance;
    y_line->mesh = new Mesh(BOX_MESH_SOURCE);
    y_line->material = new Material;
    y_line->material->diffuse_color = vec3(0, 1, 0);
    y_line->transform.position = vec3(0, 50, 0);
    y_line->transform.scale = vec3(0.1, 104, 0.1);
    scene->add_child(y_line);

    auto *z_line = new Instance;
    z_line->mesh = new Mesh(BOX_MESH_SOURCE);
    z_line->material = new Material;
    z_line->material->diffuse_color = vec3(0, 0, 1);
    z_line->transform.position = vec3(0, 0, 50);
    z_line->transform.scale = vec3(0.1, 0.1, 104);
    scene->add_child(z_line);
}

void update(float dt) {

    renderer->render();

    if(glfwGetKey(window, GLFW_KEY_W)){
        camera_box->transform.position += camera_box->transform.direction_z() * -4.0f * detaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_S)){
        camera_box->transform.position += camera_box->transform.direction_z() * 4.0f * detaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_D)){
        camera_box->transform.position +=  camera_box->transform.direction_x() * 4.0f * detaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_A)){
        camera_box->transform.position +=  camera_box->transform.direction_x() * -4.0f * detaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE)) camera_box->transform.position += vec3(0, 4 * detaTime, 0);
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))  camera_box->transform.position += vec3(0, -4 * detaTime, 0);

    if(glfwGetKey(window, GLFW_KEY_P)) {
        for(auto &[box, tar]: boxes){
            auto pos = box->transform.position + (tar - box->transform.position) * 0.03f;
            box->transform.position = pos;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_O)) {
        get_input();
        int n = 0;
        for(int i = -4;i <= 4; i++){
            for(int j = -4;j <= 4; j++){
                for(int k = -4;k <= 4; k++){
                    boxes[n].first->transform.position = vec3(i, j, k);
                    auto tar = vec4(boxes[n].first->transform.position, 1) * input_transform;
                    boxes[n++].second = vec3(tar / tar[3]);
                }
            }
        }
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static double mouse_lastX = xpos, mouse_lastY = ypos;
    double dx = xpos - mouse_lastX;
    double dy = ypos - mouse_lastY;
    camera_box->transform.rotation += vec3(0, dx, 0) / 400.0f;
    camera->transform.rotation += vec3(dy, 0, 0) / 400.0f;
    mouse_lastX = xpos;
    mouse_lastY = ypos;
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

    window = initWindow();
    //初始化glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    //启用深度测试
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    init();

    float last_time = glfwGetTime();
    while(!glfwWindowShouldClose(window)) {
        //渲染部分
        //将颜色缓冲clear，即变成背景颜色
        //将深度缓冲clear，删掉前一帧的深度信息
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        detaTime = glfwGetTime() - last_time;//更新detaTime
        last_time += detaTime;
        update(detaTime);

        glfwSwapBuffers(window); //交换两层颜色缓冲
        glfwPollEvents();//检查有没有发生事件，调用相应回调函数
    }

    //释放所有资源
    glfwTerminate();
    return 0;
}
