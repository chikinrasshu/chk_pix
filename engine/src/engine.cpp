//
// Created by chk on 3/17/2024.
//

#include <engine/engine.hpp>
#include <win/win.hpp>

int main() {
  Win win{{1280, 720}, "chk_engine_test"};
  win.lock_internal_resolution({160, 90}, true);

  return win.run([&win]() {
    // win.fb().debug();
  });
}
