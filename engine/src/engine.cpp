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

  Bitmap sample = Bitmap::from_file("data/test.png");
  sample.debug();

  return win.run([&win, &soft, &sample]() {
    soft.draw_clear(win.fb(), {0.1f, 0.2f, 0.3f, 1.0f});

    V2 mp = win.mouse_pos();
    V2 ws = win.fb().size() - V2i{1, 1};

    V2 cp = ws / 2.0f;
    V2 dp = cp - mp;

    r32 da = glm::pi<r32>() / 2.0f;
    r32 aa = 0.0f;
    r32 ab = da;
    r32 ac = 2 * da;
    r32 ad = 3 * da;

    V2 t0 = cp + dp.x * V2{cos(aa), sin(aa)} +
            dp.y * V2{cos(aa + da), sin(aa + da)};

    V2 t1 = cp + dp.x * V2{cos(ab), sin(ab)} +
            dp.y * V2{cos(ab + da), sin(ab + da)};

    V2 t2 = cp + dp.x * V2{cos(ac), sin(ac)} +
            dp.y * V2{cos(ac + da), sin(ac + da)};

    V2 t3 = cp + dp.x * V2{cos(ad), sin(ad)} +
            dp.y * V2{cos(ad + da), sin(ad + da)};

    V2 uv0 = {0, 0};
    V2 uv1 = {0, 1};
    V2 uv2 = {1, 0};
    V2 uv3 = {1, 1};

    // soft.draw_triangle(win.fb(), t0, t1, t2, sample, uv0, uv1, uv2);

    soft.draw_triangle(win.fb(), t0, t1, t2, sample, uv0, uv1, uv2);
    soft.draw_triangle(win.fb(), t0, t2, t3, sample, uv0, uv2, uv3);

    soft.draw_pixel(win.fb(), t0, {1, 0, 0, 1});
    soft.draw_pixel(win.fb(), t1, {0, 1, 0, 1});
    soft.draw_pixel(win.fb(), t2, {0, 0, 1, 1});
    soft.draw_pixel(win.fb(), t3, {1, 1, 1, 1});

    // printf("Running frame %llu...\n", win.frame_index());
  });
}
