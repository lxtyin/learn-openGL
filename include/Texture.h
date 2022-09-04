#ifndef MAIN_CPP_TEXTURE_H
#define MAIN_CPP_TEXTURE_H

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//纹理对象，一张图片
class Texture{
    unsigned int textureObject;
    int width, height, channel;
public:
    Texture();
    Texture(const char* imagePath, unsigned int type = GL_RGB);
    void activeTarget(unsigned int id)const;
    void setParameter(int pname, int params)const;
};

#endif //MAIN_CPP_TEXTURE_H
