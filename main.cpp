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

void update(){
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
    
    //设置清空屏幕用的颜色，即背景颜色
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    //启用深度测试
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    scene = new Scene();
    camera = Camera::create_perspective_camera(glm::radians(45.0f), 1.0 * SCREEN_W / SCREEN_H, 0.1f, 1000);

	renderer = new Renderer(scene, camera);

	camera_box = new Instance;
	scene->add_child(camera_box);
	camera_box->add_child(camera);

	scene->add_light(new SurroundLight(vec3(0.2, 0.2, 0.2)));
	PointLight* light = new PointLight(vec3(1.5, 1.5, 1.5));
    light->decay = vec3(1.0, 0.01, 0.0037);
    scene->add_light(light);
	DirectionalLight* light2 = new DirectionalLight(vec3(0.5, 0.5, 0.5));
	scene->add_light(light2);

    // terrain
    Instance* terrain = new Instance;
    float low_height, max_height;
    terrain->mesh = Loader::load_dem("terrain/terrain2.dem", low_height, max_height);
    scene->add_child(terrain);

	// house
	Instance *house = Loader::load_model("models/Lowpoly_tree_sample.obj");
	scene->add_child(house);

    // 原色
    terrain->material = new Material;
	terrain->material->diffuse_map = new Texture("terrain/terrain2.bmp");

    // 指示光源的箱子
    Instance *lightbox = new Instance;
    lightbox->mesh = new Mesh(BOX_MESH_SOURCE);
    lightbox->material = new Material;
    lightbox->material->use_light = false;
    lightbox->material->diffuse_color = vec3(0.8, 0.8, 1);
    lightbox->transform.position = vec3(0, 5, 0);

	light->transform = lightbox->transform;

	scene->add_child(lightbox);

	camera_box->transform.position = vec3(0, max_height + 10, 0);

	float last_time = glfwGetTime();
    while(!glfwWindowShouldClose(window)) {
        //渲染部分
        //将颜色缓冲clear，即变成背景颜色
        //将深度缓冲clear，删掉前一帧的深度信息
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer->render();

        detaTime = glfwGetTime() - last_time;//更新detaTime
		last_time += detaTime;
        update();

        glfwSwapBuffers(window); //交换两层颜色缓冲
        glfwPollEvents();//检查有没有发生事件，调用相应回调函数
    }

    //释放所有资源
    glfwTerminate();
    return 0;
}
