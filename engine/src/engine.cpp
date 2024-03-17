//
// Created by chk on 3/17/2024.
//

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

    auto mp = win.mouse_pos();
    auto ws = win.fb().size() - V2i{1, 1};

    soft.draw_line(win.fb(), {0, 0}, mp, {1, 0, 0, 1});
    soft.draw_line(win.fb(), {ws.x, 0}, mp, {0, 1, 0, 1});
    soft.draw_line(win.fb(), {0, ws.y}, mp, {0, 0, 1, 1});
    soft.draw_line(win.fb(), {ws.x, ws.y}, mp, {0, 1, 1, 1});

    // soft.draw_pixel(win.fb(), mp, {0.4f, 0.5f, 0.6f, 1.0f});

    // printf("Running frame %llu...\n", win.frame_index());
  });
}
