//
// Created by chk on 3/18/2024.
//

#include <soft/soft.hpp>

Soft::Soft() = default;

Soft::~Soft() = default;

void Soft::draw_clear(Bitmap &target, const V4 &c) {
  u32 packed = ((static_cast<u32>(c.x * 255.0f) << 16) |
                (static_cast<u32>(c.y * 255.0f) << 8) |
                (static_cast<u32>(c.z * 255.0f) << 0) |
                (static_cast<u32>(c.w * 255.0f) << 24));
  return draw_clear(target, packed);
}

void Soft::draw_pixel(Bitmap &target, const V2 &p, const V4 &c) {
  u32 packed = ((static_cast<u32>(c.x * 255.0f) << 16) |
                (static_cast<u32>(c.y * 255.0f) << 8) |
                (static_cast<u32>(c.z * 255.0f) << 0) |
                (static_cast<u32>(c.w * 255.0f) << 24));
  return draw_pixel(target, p, packed);
}

void Soft::draw_line(Bitmap &target, const V2 &p0, const V2 &p1, const V4 &c) {
  u32 packed = ((static_cast<u32>(c.x * 255.0f) << 16) |
                (static_cast<u32>(c.y * 255.0f) << 8) |
                (static_cast<u32>(c.z * 255.0f) << 0) |
                (static_cast<u32>(c.w * 255.0f) << 24));
  return draw_line(target, p0, p1, packed);
}

void Soft::draw_triangle(Bitmap &target, const V2 &p0, const V2 &p1,
                         const V2 &p2, const V4 &c) {
  u32 packed = ((static_cast<u32>(c.x * 255.0f) << 16) |
                (static_cast<u32>(c.y * 255.0f) << 8) |
                (static_cast<u32>(c.z * 255.0f) << 0) |
                (static_cast<u32>(c.w * 255.0f) << 24));
  return draw_triangle(target, p0, p1, p2, packed);
}

void Soft::draw_clear(Bitmap &target, u32 c) {
  u8 *ptr = target.memory();
  for (s32 y = 0; y < target.size().y; ++y) {
    u32 *pixel = reinterpret_cast<u32 *>(ptr);
    for (s32 x = 0; x < target.size().x; ++x) {
      *pixel++ = c;
    }
    ptr += target.pitch();
  }
}

void Soft::draw_pixel(Bitmap &target, const V2 &p, u32 c) {
  s32 x = static_cast<s32>(p.x);
  s32 y = static_cast<s32>(p.y);

  // Only draw if inside the bounds
  if ((x >= 0 && x < target.size().x) && (y >= 0 && y < target.size().y)) {
    u8 *ptr = target.memory();
    ptr += target.bpp() * sizeof(u8) * x;
    ptr += target.pitch() * y;

    u32 *pixel = reinterpret_cast<u32 *>(ptr);
    *pixel = c;
  }
}

void Soft::draw_line(Bitmap &target, const V2 &p0, const V2 &p1, u32 c) {
  s32 x0 = static_cast<s32>(p0.x), y0 = static_cast<s32>(p0.y);
  s32 x1 = static_cast<s32>(p1.x), y1 = static_cast<s32>(p1.y);

  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = (dx > dy ? dx : -dy) / 2, e2;

  for (;;) {
    draw_pixel(target, {x0, y0}, c);
    if (x0 == x1 && y0 == y1) {
      break;
    }

    e2 = err;
    if (e2 > -dx) {
      err -= dy;
      x0 += sx;
    }
    
    if (e2 < dy) {
      err += dx;
      y0 += sy;
    }
  }
}

void Soft::draw_triangle(Bitmap &target, const V2 &p0, const V2 &p1,
                         const V2 &p2, u32 c) {}
