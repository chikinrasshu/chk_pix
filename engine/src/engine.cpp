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

  return win.run([&win, &soft]() {
    soft.draw_clear(win.fb(), {0.1f, 0.2f, 0.3f, 1.0f});

    V2 mp = win.mouse_pos();
    V2 ws = win.fb().size() - V2i{1, 1};

    // soft.draw_line(win.fb(), {0, 0}, mp, {1, 0, 0, 1});
    // soft.draw_line(win.fb(), {ws.x, 0}, mp, {0, 1, 0, 1});
    // soft.draw_line(win.fb(), {0, ws.y}, mp, {0, 0, 1, 1});
    // soft.draw_line(win.fb(), {ws.x, ws.y}, mp, {0, 1, 1, 1});

    V2 cp = ws / 2.0f;
    V2 dp = mp - cp;

    r32 a = -(2.0f * glm::pi<r32>()) * (1.0f / 3.0f);
    r32 b = a + (glm::pi<r32>() / 2.0f);
    r32 c = a + a;
    r32 d = b + a;

    V2 t0 = cp + dp;
    V2 t1 = cp + V2{cos(a), sin(a)} * dp.x + V2{cos(b), sin(b)} * dp.y;
    V2 t2 = cp + V2{cos(c), sin(c)} * dp.x + V2{cos(d), sin(d)} * dp.y;

    // V4 col = {0.4f, 0.6f, 0.8f, 1.0f};
    // soft.draw_line(win.fb(), t0, t1, {1,0,0, 1});
    // soft.draw_line(win.fb(), t1, t2, {0,1,0, 1});
    // soft.draw_line(win.fb(), t2, t0, {0,0,1, 1});

    V4 cr = {1, 0, 0, 1};
    V4 cg = {0, 1, 0, 1};
    V4 cb = {0, 0, 1, 1};

    soft.draw_triangle(win.fb(), t0, t1, t2, cr, cg, cb);

    soft.draw_pixel(win.fb(), t0, {1, 0, 0, 1});
    soft.draw_pixel(win.fb(), t1, {0, 1, 0, 1});
    soft.draw_pixel(win.fb(), t2, {0, 0, 1, 1});

    // printf("Running frame %llu...\n", win.frame_index());
  });
}
