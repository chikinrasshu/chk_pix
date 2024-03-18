//
// Created by chk on 3/18/2024.
//

#ifndef CHK_PIX_SOFT_HPP
#define CHK_PIX_SOFT_HPP

#include <core/bmp.hpp>
#include <core/core.hpp>
#include <core/vec.hpp>

class Soft {
public:
  Soft();
  ~Soft();

  u32 pack_rgb(const V3 &c);
  u32 pack_rgba(const V4 &c);

  V3 unpack_rgb(const u32 &c);
  V4 unpack_rgba(const u32 &c);

  void draw_clear(Bitmap &target, const V4 &c);
  void draw_clear(Bitmap &target, u32 c);

  void draw_pixel(Bitmap &target, const V2 &p, const V4 &c);
  void draw_pixel(Bitmap &target, const V2 &p, u32 c);

  void draw_line(Bitmap &target, const V2 &p0, const V2 &p1, const V4 &c);
  void draw_line(Bitmap &target, const V2 &p0, const V2 &p1, u32 c);

  void draw_triangle(Bitmap &target, const V2 &p0, const V2 &p1, const V2 &p2,
                     Bitmap &tex, const V2 &uv0, const V2 &uv1, const V2 &uv2);

private:
};

#endif // CHK_PIX_SOFT_HPP
