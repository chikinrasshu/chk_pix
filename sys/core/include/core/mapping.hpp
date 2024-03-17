//
// Created by chk on 3/17/2024.
//

#ifndef CHK_PIX_MAPPING_HPP
#define CHK_PIX_MAPPING_HPP

#include "core.hpp"
#include "vec.hpp"

r32 map_1d(r32 v, const V2 &src, const V2 &dst);
V2 map_2d(const V2 &p, const V2 &src_a, const V2 &src_b, const V2 &dst_a,
          const V2 &dst_b);

#endif // CHK_PIX_MAPPING_HPP
