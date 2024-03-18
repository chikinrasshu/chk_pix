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
  win.lock_internal_resolution({160, 90}, true);

  Soft soft{};

  Bitmap sample = Bitmap::from_file("data/img/peko_megaman.png");

  return win.run([&win, &soft, &sample]() {
    soft.draw_clear(win.fb(), {0.1f, 0.2f, 0.3f, 1.0f});

    V2 mp = win.mouse_pos();
    V2 ws = win.fb().size() - V2i{1, 1};

    V2 p0 = {4,4};
    V2 p1 = {4, mp.y};
    V2 p2 = {mp.x, mp.y};
    V2 p3 = {mp.x, 4};

    V2 uv0 = {0,1};
    V2 uv1 = {0,0};
    V2 uv2 = {1,0};
    V2 uv3 = {1,1};

    soft.draw_triangle(win.fb(), p0, p1, p2, sample, uv0, uv1, uv2);
    soft.draw_triangle(win.fb(), p2, p3, p0, sample, uv2, uv3, uv0);

    soft.draw_pixel(win.fb(), p0, {1, 0, 0, 1});
    soft.draw_pixel(win.fb(), p1, {0, 1, 0, 1});
    soft.draw_pixel(win.fb(), p2, {0, 0, 1, 1});
    soft.draw_pixel(win.fb(), p3, {1, 1, 1, 1});

    // printf("Running frame %llu...\n", win.frame_index());
  });
}
