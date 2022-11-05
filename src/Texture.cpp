#include "Texture.h"
#include <iostream>

Texture::Texture(const char* imagePath) {
    //openGL的图像坐标系是→↑，而一般图像为→↓，故翻转y轴
    stbi_set_flip_vertically_on_load(true);
    //保存图像内存
    try {
        int width, height, channel;
        unsigned char* data = stbi_load(imagePath, &width, &height, &channel, 0);

        int format;
        if(channel == 1) format = GL_RED;
        if(channel == 3) format = GL_RGB;
        if(channel == 4) format = GL_RGBA;

        glGenTextures(1, &textureObject);
        glBindTexture(GL_TEXTURE_2D, textureObject);//和VBO同理，绑定后通过GL_TEXTURE_2D赋值
        //设置环绕方式，放大缩小时的过滤模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //第二个参数s,t,r相当于x,y,z，对单一轴设置环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //此处为简单复制，也是默认方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //设置放大缩小时的插值方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_NEAREST 为邻近

        glTexImage2D(GL_TEXTURE_2D,     //目标
                     0,                 //多级渐进纹理级别
                     format,            //保存的格式
                     width, height,     //长宽
                     0,                 //历史遗留
                     format,              //原图格式
                     GL_UNSIGNED_BYTE,  //存储为Byte数组
                     data);             //数据源

        glGenerateMipmap(GL_TEXTURE_2D);//生成多级渐远纹理（具体先略过）
        
        stbi_image_free(data);//释放图像内存
    } catch(...) {
        std::cerr << imagePath << ": Image load failed." << std::endl;
    }
}

/// 将这张图片应用到纹理单元
/// \param id 0~15
void Texture::activeTarget(unsigned int id)const {
    //允许同时使用至多16张纹理，在绑定前激活0号纹理单元，就能绑定到0号单元，片段着色器中采样器的值即为其采用的目标单元
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, textureObject);
}

/// 设置一些参数
/// \param pname
/// \param params
void Texture::setParameter(int pname, int params)const {
    glBindTexture(GL_TEXTURE_2D, textureObject);
    glTexParameteri(GL_TEXTURE_2D, pname, params);
}
