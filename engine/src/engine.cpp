//
// Created by chk on 3/17/2024.
//

#include "glm/ext/scalar_constants.hpp"
#include <core/core.hpp>
#include <engine/engine.hpp>
#include <soft/soft.hpp>
#include <win/win.hpp>

int main() {
  Win win{{800, 600}, "chk_engine_test"};
  win.lock_internal_resolution({160, 90}, false);

  Soft soft{};

  Bitmap peko = Bitmap::from_file("data/img/peko.png");
  Bitmap nousagi = Bitmap::from_file("data/img/nousagi.png");

  return win.run([&win, &soft, &peko, &nousagi]() {
    soft.draw_clear(win.fb(), {0.1f, 0.2f, 0.3f, 1.0f});

    V2 mp = win.mouse_pos();
    V2 ws = win.fb().size() - V2i{1, 1};

    { // Draw peko
      r32 aspect = peko.aspect_ratio_inv();
      V2 p0 = {4, 4};
      V2 p1 = {4, mp.x * aspect};
      V2 p2 = {mp.x, mp.x * aspect};
      V2 p3 = {mp.x, 4};

      V2 uv0 = {0, 1};
      V2 uv1 = {0, 0};
      V2 uv2 = {1, 0};
      V2 uv3 = {1, 1};

      soft.draw_triangle(win.fb(), p0, p1, p2, peko, uv0, uv1, uv2);
      soft.draw_triangle(win.fb(), p2, p3, p0, peko, uv2, uv3, uv0);
    }

    { // Draw nousagi
      r32 aspect = nousagi.aspect_ratio_inv();
      V2 p2 = ws - V2{4, 4};
      V2 p3 = ws - V2{4, mp.x * aspect};
      V2 p0 = ws - V2{mp.x, mp.x * aspect};
      V2 p1 = ws - V2{mp.x, 4};

      V2 uv0 = {0, 1};
      V2 uv1 = {0, 0};
      V2 uv2 = {1, 0};
      V2 uv3 = {1, 1};

      soft.draw_triangle(win.fb(), p0, p1, p2, nousagi, uv0, uv1, uv2);
      soft.draw_triangle(win.fb(), p2, p3, p0, nousagi, uv2, uv3, uv0);
    }
  });
}
