#ifndef MAIN_CPP_TEXTURE_H
#define MAIN_CPP_TEXTURE_H

#include "glad/glad.h"
#include "tool/stb_image.h"
#include <string>
using std::string;

//纹理对象，一张图片
class Texture{
    unsigned int textureObject;
public:
    Texture(const string &imagePath);
    void activeTarget(unsigned int id)const;
    void setParameter(int pname, int params)const;
};

#endif //MAIN_CPP_TEXTURE_H
