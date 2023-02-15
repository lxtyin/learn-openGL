#include "Transform.h"
#include "Instance.h"
#include <algorithm>
#include <iostream>

using namespace std;

Transform::Transform() :
	position(vec3(0, 0, 0)),
	rotation(vec3(0, 0, 0)),
	scale(vec3(1, 1, 1)){}

Transform::Transform(vec3 position, vec3 rotation, vec3 scale) :
	position(position),
	rotation(rotation),
	scale(scale){}

mat4 Transform::matrix() {
	mat4 s{ scale.x, 0, 0, 	0,
			0, scale.y, 0, 	0,
			0, 0, scale.z, 	0,
			0, 0, 0,		1 };
	float x = rotation.x;
	float y = rotation.y;
	float z = rotation.z;
	mat4 rot_x{ 1,		0,		0,		0,
				0,		cos(x),	-sin(x),0,
				0,		sin(x),	cos(x),	0,
				0, 		0,	 	0,		1};
	mat4 rot_y{ cos(y), 0,		sin(y), 0,
				0, 		1,		0,		0,
				-sin(y),0,		cos(y), 0,
				0, 		0,		0, 		1 };
	mat4 rot_z{ cos(z), -sin(z),0, 		0,
				sin(z), cos(z), 0, 		0,
				0, 	 	0,	   	1, 		0,
				0,   	0,  	0, 		1 };
	mat4 res(1);
	for(char c: order){
		if(c == 'X') res *= rot_x;
		if(c == 'Y') res *= rot_y;
		if(c == 'Z') res *= rot_z;

	}
	res *= s;
	res[3] = {position, 1};
	return res;
}

void Transform::set_order(string o)
{
	if (o.size() == 3)
	{
		std::sort(o.begin(), o.end());
		if (o == "XYZ")
		{
			order = o;
		}
		else
		{
			std::cerr << "Transform: order string error." << std::endl;
		}
	}
}

vec3 Transform::direction_x() {
	return matrix()[0];
}
vec3 Transform::direction_y() {
	return matrix()[1];
}
vec3 Transform::direction_z() {
	return matrix()[2];
}
