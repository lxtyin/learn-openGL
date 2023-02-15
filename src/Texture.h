#ifndef MAIN_CPP_TEXTURE_H
#define MAIN_CPP_TEXTURE_H

#include "glad/glad.h"
#include "tool/stb_image.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

//纹理对象，一张图片
class Texture{

	int width;
	int height;
	int channel;
	vector<uint8_t> data;

	unsigned int TTO;	/** < texture object. */
	bool is_load = false;
public:

	void load();	/** < load to gpu */
	void unload();	/** < unload from gpu */

	/**
	 * Load from local path.
	 * \param imagePath
	 */
    explicit Texture(const string &imagePath);

	Texture(int width, int height, int channel, vector<uint8_t> &data);

	/**
	 * 将这张图片应用到纹理单元
	 * \param id
	 */
    void activeTarget(unsigned int id);

	/**
	 * 设置一些参数
	 * \param pname
	 * \param params
	 */
    void setParameter(int pname, int params);
};

#endif //MAIN_CPP_TEXTURE_H
