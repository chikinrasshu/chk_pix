//
// Created by chk on 3/17/2024.
//

#include <core/log.hpp>
#include <core/mapping.hpp>
#include <win/win.hpp>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

b32 g_win_push();
b32 g_win_pop();

#ifndef NDEBUG
void g_gl_debug_output(GLenum source, GLenum type, GLuint id, GLenum severity,
                       GLsizei length, const GLchar *message,
                       const void *user_ptr) {
  (void)source;
  (void)id;
  (void)length;
  (void)user_ptr;
  (void)severity;

  // Print, log, whatever based on the enums and message
  switch (type) {
  default:
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    Log::log("OpenGL: {}", message);
    break;
  case GL_DEBUG_SEVERITY_LOW:
  case GL_DEBUG_SEVERITY_MEDIUM:
    Log::info("OpenGL: {}", message);
    break;
  case GL_DEBUG_SEVERITY_HIGH:
    Log::warn("OpenGL: {}", message);
    break;
  }
}
#endif

Win::Win(V2i size, StringView caption) {
  if (!g_win_push()) {
    _exit_code = 1;
    return;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_FORWARD_COMPAT);

#ifndef NDEBUG
  glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif

  auto handle =
      glfwCreateWindow(size.x, size.y, caption.data(), nullptr, nullptr);
  if (!handle) {
    _exit_code = 1;
    return;
  }
  _ptr = handle;

  { // Set callbacks
    glfwSetWindowUserPointer(handle, this);

    glfwSetWindowCloseCallback(handle, [](GLFWwindow *handle) {
      auto *win = static_cast<Win *>(glfwGetWindowUserPointer(handle));
      if (win) {
        win->_running = false;
      }
    });

    glfwSetWindowRefreshCallback(handle, [](GLFWwindow *handle) {
      auto *win = static_cast<Win *>(glfwGetWindowUserPointer(handle));
      if (win && win->running()) {
        win->tick(false);
      }
    });

    glfwSetWindowPosCallback(handle, [](GLFWwindow *handle, s32 x, s32 y) {
      auto *win = static_cast<Win *>(glfwGetWindowUserPointer(handle));
      if (win) {
        win->_pos.x = x, win->_pos.y = y;
        win->_moved = true;
      }
    });

    glfwSetWindowSizeCallback(handle, [](GLFWwindow *handle, s32 w, s32 h) {
      auto *win = static_cast<Win *>(glfwGetWindowUserPointer(handle));
      if (win) {
        win->_size.x = w, win->_size.y = h;
        win->_resized = true;
      }
    });

    glfwSetFramebufferSizeCallback(
        handle, [](GLFWwindow *handle, s32 w, s32 h) {
          auto *win = static_cast<Win *>(glfwGetWindowUserPointer(handle));
          if (win) {
            win->_fb_size.x = w, win->_fb_size.y = h;
            win->_fb_resized = true;

            if (!win->_locked) {
              win->_ires.x = w, win->_ires.y = h;
              win->_ires_changed = true;
            }
          }
        });

    glfwSetWindowContentScaleCallback(
        handle, [](GLFWwindow *handle, r32 sx, r32 sy) {
          auto *win = static_cast<Win *>(glfwGetWindowUserPointer(handle));
          if (win) {
            win->_scale.x = sx, win->_scale.y = sy;
          }
        });

    glfwSetCursorPosCallback(handle, [](GLFWwindow *handle, r64 mx, r64 my) {
      auto *win = static_cast<Win *>(glfwGetWindowUserPointer(handle));
      if (win) {
        r32 nx = static_cast<r32>(mx), ny = static_cast<r32>(my);
        r32 dx = nx - win->_raw_mouse_pos.x, dy = ny - win->_raw_mouse_pos.y;

        win->_raw_mouse_delta.x = dx, win->_raw_mouse_delta.y = dy;
        win->_raw_mouse_pos.x = nx, win->_raw_mouse_pos.y = ny;
      }
    });
  }

  { // Initialize OpenGL
    glfwMakeContextCurrent(handle);
    s32 ver = gladLoadGL(glfwGetProcAddress);
    if (!ver) {
      _exit_code = 1;
      return;
    }

    s32 ver_major = GLAD_VERSION_MAJOR(ver);
    s32 ver_minor = GLAD_VERSION_MINOR(ver);
    Log::info("Loaded OpenGL {}.{}", ver_major, ver_minor);

#ifndef NDEBUG
    if (GLAD_GL_ARB_debug_output) {
      Log::info("Enabling OpenGL debug_output");
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(g_gl_debug_output, nullptr);

      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                            nullptr, GL_TRUE);
    }
#endif

    glfwSwapInterval(1);
  }

  { // Get default values
    r64 tmp_a_r64, tmp_b_r64;

    glfwGetWindowPos(handle, &_pos.x, &_pos.y);
    glfwGetWindowSize(handle, &_size.x, &_size.y);
    glfwGetFramebufferSize(handle, &_fb_size.x, &_fb_size.y);
    glfwGetWindowContentScale(handle, &_scale.x, &_scale.y);
    glfwGetCursorPos(handle, &tmp_a_r64, &tmp_b_r64);

    _ires.x = _fb_size.x, _ires.y = _fb_size.y;
    _raw_mouse_pos.x = static_cast<r32>(tmp_a_r64),
    _raw_mouse_pos.y = static_cast<r32>(tmp_b_r64);

    // Set minimum dimensions
    glfwSetWindowSizeLimits(handle, 64, 64, GLFW_DONT_CARE, GLFW_DONT_CARE);
  }

  _running = true;
}

Win::~Win() {
  if (_running) {
    glfwDestroyWindow(static_cast<GLFWwindow *>(_ptr));
  }
  _running = false;

  g_win_pop();
}

s32 Win::run(const Callback &on_tick) {
  if (on_tick)
    _on_tick = on_tick;

  s32 result = _exit_code;
  while (_running) {
    result = tick();
  }
  return result;
}

s32 Win::tick(b32 process_events) {
  auto *handle = static_cast<GLFWwindow *>(_ptr);

  if (process_events) {
    glfwPollEvents();
  }

  if (!_running) {
    return _exit_code;
  }

  { // Calculate the viewport dimensions based on scaling config
    if (resized()) {
      _vp = {0, 0, _fb_size};
      if (_locked) {
        r32 sw = (r32)_fb_size.x, sh = (r32)_fb_size.y;
        r32 bw = (r32)_ires.x, bh = (r32)_ires.y;

        r32 scale = std::min(sw / bw, sh / bh);
        if (scale >= 1.0 && _integer) {
          scale = floorf(scale);
        }

        _vp.z = bw * scale, _vp.w = bh * scale;
        _vp.x = (sw - _vp.z) / 2.0f, _vp.y = (sh - _vp.w) / 2.0f;
      }
    }
  }

  { // Calculate scaled mouse coordinates
    if (mouse_moved()) {
      V2 mp = {_raw_mouse_pos.x * _scale.x, _raw_mouse_pos.y * _scale.y};
      V2 tl = {_vp.x, _vp.y}, br = V2{_vp.x + _vp.z, _vp.y + _vp.w};
      _mouse_pos = glm::floor(map_2d(mp, tl, br, {0, 0}, _ires));
    }
  }

  { // Clear the window
    glViewport(0, 0, _fb_size.x, _fb_size.y);
    glClearColor(_bg.x, _bg.y, _bg.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  { // Generate frame
    if (_on_tick) {
      _on_tick();
    }
  }

  { // Present frame
    glfwSwapBuffers(handle);
  }

  { // Reset boolean state
    _moved = _resized = _fb_resized = _ires_changed = false;
    _mouse_moved = false;
  }

  { // Calculate delta time and finish frame.
    ++_frame_index;
  }

  return _exit_code;
}

b32 Win::lock_internal_resolution(V2i res, b32 integer_scaling) {
  _locked = true;
  _integer = integer_scaling;
  _ires = res;
  _ires_changed = true;
  return true;
}

[[maybe_unused]] b32 Win::unlock_internal_resolution() {
  _locked = false;
  glfwGetFramebufferSize(static_cast<GLFWwindow *>(_ptr), &_ires.x, &_ires.y);
  _ires_changed = true;
  return true;
}

// Implement GLFW initialization
static s32 g_win_count = 0;

b32 g_win_push() {
  if (g_win_count == 0) {
    if (!glfwInit())
      return false;
  }
  return ++g_win_count > 0;
}

b32 g_win_pop() {
  if (g_win_count == 1) {
    glfwTerminate();
  }
  return --g_win_count >= 0;
}
