#include "Texture.h"
#include <iostream>

void Texture::load() {
	int format;
	if(channel == 1) format = GL_RED;
	if(channel == 3) format = GL_RGB;
	if(channel == 4) format = GL_RGBA;

	glGenTextures(1, &TTO);
	glBindTexture(GL_TEXTURE_2D, TTO);//和VBO同理，绑定后通过GL_TEXTURE_2D赋值
	glTexImage2D(GL_TEXTURE_2D,     //目标
		0,                 //多级渐进纹理级别
		format,            //保存的格式
		width, height,     //长宽
		0,                 //历史遗留
		format,            //原图格式
		GL_UNSIGNED_BYTE,  //存储为Byte数组
		data.data());             //数据源
	//设置环绕方式，放大缩小时的过滤模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //第二个参数s,t,r相当于x,y,z，对单一轴设置环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //此处为简单复制，也是默认方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //设置放大缩小时的插值方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_NEAREST 为邻近
	glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
	is_load = true;
}

void Texture::unload() {
	glDeleteTextures(1, &TTO);
	is_load = false;
}

Texture::Texture(const string &imagePath) {
    //openGL的图像坐标系是→↑，而一般图像为→↓，加载时默认翻转y轴
    stbi_set_flip_vertically_on_load(true);
    //保存图像内存
    try {
        unsigned char* tmp = stbi_load(imagePath.c_str(), &width, &height, &channel, 0);

		data.resize(width * height * channel);
		memcpy(data.data(), tmp, data.size());

		stbi_image_free(tmp);
    } catch(...) {
        std::cerr << imagePath << ": Image load failed." << std::endl;
    }
}

Texture::Texture(int w, int h, int c, vector<uint8_t> &d):
	width(w), height(h), channel(c){
	data = move(d);
}


void Texture::activeTarget(unsigned int id) {
	if(!is_load) load();
    //允许同时使用至多16张纹理，在绑定前激活0号纹理单元，就能绑定到0号单元，片段着色器中采样器的值即为其采用的目标单元
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, TTO);
}

void Texture::setParameter(int pname, int params) {
	if(!is_load) load();
    glBindTexture(GL_TEXTURE_2D, TTO);
    glTexParameteri(GL_TEXTURE_2D, pname, params);
}