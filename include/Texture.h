#ifndef MAIN_CPP_TEXTURE_H
#define MAIN_CPP_TEXTURE_H

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "stb_image.h"

//纹理对象，一张图片
class Texture{
    unsigned int textureObject;
    int width, height, channel;
public:
    Texture(const char *imagePath);
    void activeTarget(unsigned int id)const;
    void setParameter(int pname, int params)const;
};

#endif //MAIN_CPP_TEXTURE_H
