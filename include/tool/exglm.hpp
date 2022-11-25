#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace glm {
    //扩展矩阵左乘 仿照glm加速。
    template<typename T, qualifier Q>
    GLM_FUNC_QUALIFIER mat<4, 4, T, Q> translate(vec<3, T, Q> const& v, mat<4, 4, T, Q> const& m) {
        mat4 Result(m);
        Result[3] = m[3] + vec4(v, 0);
        return Result;
    }
    template<typename T, qualifier Q>
    GLM_FUNC_QUALIFIER mat<4, 4, T, Q> rotate(T angle, vec<3, T, Q> const& v, mat<4, 4, T, Q> const& m) {
        // 这边比较麻烦 先不管效率这么放这
        return rotate(mat4(1.0f), angle, v) * m;
    }

    //单点的旋转
    template<typename T, qualifier Q>
    GLM_FUNC_QUALIFIER vec<3, T, Q> point_rotate(vec<3, T, Q> const& ori, T angle, vec<3, T, Q> const& v) {
        return rotate(mat4(1.0f), angle, v) * vec4(ori, 1.0);
    }

    //参考父坐标缩放好像没有太大意义
}
