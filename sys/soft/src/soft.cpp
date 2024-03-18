//
// Created by chk on 3/18/2024.
//

#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include <cmath>
#include <soft/soft.hpp>

Soft::Soft() = default;

Soft::~Soft() = default;

u32 Soft::pack_rgb(const V3 &c) {
  return ((static_cast<u32>(c.x * 255.0f) << 16) |
          (static_cast<u32>(c.y * 255.0f) << 8) |
          (static_cast<u32>(c.z * 255.0f) << 0));
}

u32 Soft::pack_rgba(const V4 &c) {
  return ((static_cast<u32>(c.x * 255.0f) << 16) |
          (static_cast<u32>(c.y * 255.0f) << 8) |
          (static_cast<u32>(c.z * 255.0f) << 0) |
          (static_cast<u32>(c.w * 255.0f) << 24));
}

V3 Soft::unpack_rgb(const u32 &c) {
  return {(static_cast<r32>((c >> 16) & 0xFF) / 255.0f),
          (static_cast<r32>((c >> 8) & 0xFF) / 255.0f),
          (static_cast<r32>((c >> 0) & 0xFF) / 255.0f)};
}

V4 Soft::unpack_rgba(const u32 &c) {
  return {(static_cast<r32>((c >> 16) & 0xFF) / 255.0f),
          (static_cast<r32>((c >> 8) & 0xFF) / 255.0f),
          (static_cast<r32>((c >> 0) & 0xFF) / 255.0f),
          (static_cast<r32>((c >> 24) & 0xFF) / 255.0f)};
}

void Soft::draw_clear(Bitmap &target, const V4 &c) {
  return draw_clear(target, pack_rgba(c));
}

void Soft::draw_pixel(Bitmap &target, const V2 &p, const V4 &c) {
  return draw_pixel(target, p, pack_rgba(c));
}

void Soft::draw_line(Bitmap &target, const V2 &p0, const V2 &p1, const V4 &c) {

  return draw_line(target, p0, p1, pack_rgba(c));
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

r32 edge_fn(const V2 &a, const V2 &b, const V2 &p) {
  return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

void Soft::draw_triangle(Bitmap &target, const V2 &p0, const V2 &p1,
                         const V2 &p2, Bitmap &tex, const V2 &uv0,
                         const V2 &uv1, const V2 &uv2) {
  // Get winding order and skip backwards facing triangles
  V3 cp = glm::cross(V3(p1 - p0, 0), V3(p2 - p0, 0));
  if (cp.z >= 0) {
      // V2 tmp = ;
      // printf("Backwards triangle\n");
  }

  // Get triangle bounding box
  V2i minp = glm::max(glm::min(p0, glm::min(p1, p2)), {0, 0});
  V2i maxp =
      glm::min(glm::max(p0, glm::max(p1, p2)), V2(target.size()) - V2{1, 1});

  for (s32 y = minp.y; y <= maxp.y; ++y) {
    for (s32 x = minp.x; x <= maxp.x; ++x) {
      V2 p = V2{x, y} + V2{0.5f, 0.5f};

      r32 area = edge_fn(p0, p1, p2);
      r32 w0 = edge_fn(p1, p2, p);
      r32 w1 = edge_fn(p2, p0, p);
      r32 w2 = edge_fn(p0, p1, p);

      if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
        w0 /= area, w1 /= area, w2 /= area;

        V2 uv = {w0 * uv0.x + w1 * uv1.x + w2 * uv2.x,
                 w0 * uv0.y + w1 * uv1.y + w2 * uv2.y};

        // Sample tex
        uv = glm::fract(uv);
        s32 tex_x = (s32)(uv.x * (r32)tex.size().x);
        s32 tex_y = (s32)((1.0f - uv.y) * (r32)tex.size().y);
        u8 *ptr =
            tex.memory() + tex_y * tex.pitch() + sizeof(u8) * tex.bpp() * tex_x;
        u32 *pix = reinterpret_cast<u32 *>(ptr);

        /*
        V4 color = {w0 * c0.x + w1 * c1.x + w2 * c2.x,
                    w0 * c0.y + w1 * c1.y + w2 * c2.y,
                    w0 * c0.z + w1 * c1.z + w2 * c2.z,
                    w0 * c0.w + w1 * c1.w + w2 * c2.w};
        */

        draw_pixel(target, p, *pix);
      }
    }
  }
}
