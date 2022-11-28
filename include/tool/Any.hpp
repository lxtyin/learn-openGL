//
// Created by lx_tyin on 2022/11/25.
//

#ifndef OPENGL_ANY_HPP
#define OPENGL_ANY_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <string>
using std::string;
using glm::vec2;
using glm::vec3;
using glm::mat4;

/**
 * a class can store Any type.
 * contains int, float, vec2, vec3 and mat4 now.
 */
class Any {
public:
    string type_name;
    void *value_ptr = nullptr;

    Any() {
        type_name = "int";
        value_ptr = new int(0);
    }
    Any(int v){
        type_name = "int";
        value_ptr = new int((int) v);
    }
    Any(float v){
        type_name = "float";
        value_ptr = new float(v);
    }
    Any(vec2 v){
        type_name = "vec2";
        value_ptr = new vec2(v);
    }
    Any(vec3 v){
        type_name = "vec3";
        value_ptr = new vec3(v);
    }
    Any(mat4 v){
        type_name = "mat4";
        value_ptr = new mat4(v);
    }

    Any(const Any &t) {
        type_name = t.type_name;
        if(type_name == "int") value_ptr = new int(*(int*)t.value_ptr);
        if(type_name == "float") value_ptr = new float(*(float*)t.value_ptr);
        if(type_name == "vec2") value_ptr = new vec2(*(vec2*)t.value_ptr);
        if(type_name == "vec3") value_ptr = new vec3(*(vec3*)t.value_ptr);
        if(type_name == "mat4") value_ptr = new mat4(*(mat4*)t.value_ptr);
    }

    ~Any() {
        if(type_name == "int") delete (int*)value_ptr;
        if(type_name == "float") delete (float*)value_ptr;
        if(type_name == "vec2") delete (vec2*)value_ptr;
        if(type_name == "vec3") delete (vec3*)value_ptr;
        if(type_name == "mat4") delete (mat4*)value_ptr;
    }

    Any& operator =(const Any &t) {
        if(this == &t) return *this;        /** < self-assignment properly */
        this->~Any();
        type_name = t.type_name;
        if(type_name == "int") value_ptr = new int(*(int*)t.value_ptr);
        if(type_name == "float") value_ptr = new float(*(float*)t.value_ptr);
        if(type_name == "vec2") value_ptr = new vec2(*(vec2*)t.value_ptr);
        if(type_name == "vec3") value_ptr = new vec3(*(vec3*)t.value_ptr);
        if(type_name == "mat4") value_ptr = new mat4(*(mat4*)t.value_ptr);
        return *this;
    };
};

#endif //OPENGL_ANY_HPP
