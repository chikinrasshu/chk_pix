//
// Created by chk on 3/17/2024.
//

#ifndef CHK_PIX_WIN_WIN_H
#define CHK_PIX_WIN_WIN_H

#include <core/core.hpp>
#include <core/fn.hpp>
#include <core/vec.hpp>

#include "framebuffer.hpp"

class Win {
public:
  Win(V2i size, StringView caption);
  virtual ~Win();

  s32 run(const Callback &on_tick = nullptr);
  s32 tick(b32 process_events = true);

  b32 lock_internal_resolution(V2i res, b32 integer_scaling = true);
  b32 unlock_internal_resolution();

  // Getters
  [[nodiscard]] s32 exit_code() const { return _exit_code; }
  [[nodiscard]] b32 running() const { return _running; }
  [[nodiscard]] b32 moved() const { return _moved; }
  [[nodiscard]] b32 resized() const { return _resized; }
  [[nodiscard]] b32 fb_resized() const {
    return _locked ? _ires_changed : _fb_resized;
  }
  [[nodiscard]] u64 frame_index() const { return _frame_index; }

  [[nodiscard]] V2i pos() const { return _pos; }
  [[nodiscard]] V2i size() const { return _size; }
  [[nodiscard]] V2i fb_size() const { return _fb_size; }
  [[nodiscard]] V2i internal_resolution() const { return _ires; }
  [[nodiscard]] V2 scale() const { return _scale; }
  [[nodiscard]] V2 raw_mouse_pos() const { return _mouse_pos; }
  [[nodiscard]] V2 mouse_pos() const { return _mouse_pos; }
  [[nodiscard]] b32 mouse_moved() const { return _mouse_moved; }

  void set_bg(const V3 bg) { _bg = bg; }
  [[nodiscard]] V3 &bg() { return _bg; }
  [[nodiscard]] const V3 &bg() const { return _bg; }

private:
  s32 _exit_code{0};
  b32 _running{false};
  b32 _moved{false};
  b32 _resized{false};
  b32 _fb_resized{false};
  b32 _ires_changed{false};
  b32 _locked{false};
  b32 _integer{true};
  u64 _frame_index{0};

  V2i _pos{0, 0};
  V2i _size{0, 0};
  V2i _fb_size{0, 0};
  V2i _ires{0, 0};
  V4 _vp{0, 0, 0, 0};
  V2 _scale{1, 1};

  b32 _mouse_moved{false};
  V2 _raw_mouse_delta{0, 0};
  V2 _raw_mouse_pos{0, 0};
  V2 _mouse_pos{0, 0};

  V3 _bg{0.3f, 0.4f, 0.5f};

  Callback _on_tick{nullptr};

  void *_ptr{nullptr};
};

#endif // CHK_PIX_WIN_WIN_H
