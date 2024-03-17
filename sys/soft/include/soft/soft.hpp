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

  void draw_clear(Bitmap &target, const V4 &c);
  void draw_clear(Bitmap &target, u32 c);

  void draw_pixel(Bitmap &target, const V2 &p, const V4 &c);
  void draw_pixel(Bitmap &target, const V2 &p, u32 c);

  void draw_line(Bitmap &target, const V2 &p0, const V2 &p1, const V4 &c);
  void draw_line(Bitmap &target, const V2 &p0, const V2 &p1, u32 c);

  void draw_triangle(Bitmap &target, const V2 &p0, const V2 &p1, const V2 &p2,
                     const V4 &c);
  void draw_triangle(Bitmap &target, const V2 &p0, const V2 &p1, const V2 &p2,
                     u32 c);
  
private:
};

#endif // CHK_PIX_SOFT_HPP
