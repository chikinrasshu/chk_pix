//
// Created by Jorge Botarro on 19-03-24.
//

#ifndef CHK_PIX_CORE_MAT_HPP
#define CHK_PIX_CORE_MAT_HPP

#include <core/core.hpp>

#include <glm/glm.hpp>

template <typename T> using Mat2T = glm::mat<2, 2, T, glm::highp>;
template <typename T> using Mat3T = glm::mat<3, 3, T, glm::highp>;
template <typename T> using Mat4T = glm::mat<4, 4, T, glm::highp>;

using Mat2 = Mat2T<r32>;
using Mat2i = Mat2T<s32>;
using Mat2u = Mat2T<u32>;

using Mat3 = Mat3T<r32>;
using Mat3i = Mat3T<s32>;
using Mat3u = Mat3T<u32>;

using Mat4 = Mat4T<r32>;
using Mat4i = Mat4T<s32>;
using Mat4u = Mat4T<u32>;

#endif // CHK_PIX_CORE_MAT_HPP
