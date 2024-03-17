//
// Created by chk on 3/17/2024.
//

#include <core/core.hpp>
#include <engine/engine.hpp>
#include <win/win.hpp>

int main() {
  Win win{{800, 600}, "chk_engine_test"};
  win.lock_internal_resolution({160, 90}, true);

  return win.run([]() {
    // printf("Running frame %llu...\n", win.frame_index());
  });
}
