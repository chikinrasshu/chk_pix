//
// Created by chk on 3/17/2024.
//

#ifndef CHK_PIX_CORE_VEC_HPP
#define CHK_PIX_CORE_VEC_HPP

#include <core/core.hpp>

#include <glm/glm.hpp>

template <typename T> using V2T = glm::vec<2, T, glm::highp>;
template <typename T> using V3T = glm::vec<3, T, glm::highp>;
template <typename T> using V4T = glm::vec<4, T, glm::highp>;

using V2 = V2T<r32>;
using V2i = V2T<s32>;
using V2u = V2T<u32>;

using V3 = V3T<r32>;
using V3i = V3T<s32>;
using V3u = V3T<u32>;

using V4 = V4T<r32>;
using V4i = V4T<s32>;
using V4u = V4T<u32>;

#endif // CHK_PIX_CORE_VEC_HPP
