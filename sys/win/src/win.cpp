//
// Created by chk on 3/17/2024.
//

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
  (void)type;
  (void)id;
  (void)severity;
  (void)length;
  (void)user_ptr;

  // Print, log, whatever based on the enums and message
  fprintf(stderr, "OpenGL: %s\n", message);
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
      }
    });
    glfwSetWindowSizeCallback(handle, [](GLFWwindow *handle, s32 w, s32 h) {
      auto *win = static_cast<Win *>(glfwGetWindowUserPointer(handle));
      if (win) {
        win->_size.x = w, win->_size.y = h;
      }
    });
    glfwSetFramebufferSizeCallback(
        handle, [](GLFWwindow *handle, s32 w, s32 h) {
          auto *win = static_cast<Win *>(glfwGetWindowUserPointer(handle));
          if (win) {
            if (!win->_locked) {
              win->_fb.resize({w, h});
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
        win->_raw_mouse_pos.x = static_cast<r32>(mx),
        win->_raw_mouse_pos.y = static_cast<r32>(my);
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
    printf("Loaded OpenGL %d.%d\n", ver_major, ver_minor);

#ifndef NDEBUG
    if (GLAD_GL_ARB_debug_output) {
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
    s32 tmp_a_s32, tmp_b_s32;

    glfwGetWindowPos(handle, &_pos.x, &_pos.y);
    glfwGetWindowSize(handle, &_size.x, &_size.y);
    glfwGetFramebufferSize(handle, &tmp_a_s32, &tmp_b_s32);
    glfwGetWindowContentScale(handle, &_scale.x, &_scale.y);
    glfwGetCursorPos(handle, &tmp_a_r64, &tmp_b_r64);

    if (!_locked) {
      _fb.resize({tmp_a_s32, tmp_b_s32});
    }
    _fb.init();
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

  s32 win_px_w, win_px_h;
  { // Clear the window
    glfwGetFramebufferSize(handle, &win_px_w, &win_px_h);

    glViewport(0, 0, win_px_w, win_px_h);
    glClearColor(_bg.x, _bg.y, _bg.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  if (_on_tick) {
    _on_tick();
  }

  // Calculate the viewport dimensions based on scaling config
  s32 vx = 0, vy = 0, vw = 0, vh = 0;
  r32 scale = 1.0f;
  if (_locked) {
    r32 sw = (r32)win_px_w, sh = (r32)win_px_h;
    r32 bw = (r32)_fb.size().x, bh = (r32)_fb.size().y;

    scale = std::min(sw / bw, sh / bh);
    if (scale >= 1.0 && _integer) {
      scale = floorf(scale);
    }

    vw = (s32)(bw * scale), vh = (s32)(bh * scale);
    vx = ((s32)sw - vw) / 2, vy = ((s32)sh - vh) / 2;
  }
  glViewport(vx, vy, vw, vh);

  // Calculate scaled mouse coordinates
  V2 mp = {_raw_mouse_pos.x * _scale.x, _raw_mouse_pos.y * _scale.y};
  V2 tl = {vx, vy}, br = V2{vw, vh} + tl;
  _mouse_pos = glm::round(map_2d(mp, tl, br, {0, 0}, _fb.size()));

  _fb.render();
  glfwSwapBuffers(handle);
  ++_frame_index;
  return _exit_code;
}

b32 Win::lock_internal_resolution(V2i res, b32 integer_scaling) {
  _locked = true;
  _integer = integer_scaling;
  _fb.resize(res);
  return true;
}

[[maybe_unused]] b32 Win::unlock_internal_resolution() {
  _locked = false;
  s32 w, h;
  glfwGetFramebufferSize(static_cast<GLFWwindow *>(_ptr), &w, &h);
  _fb.resize({w, h});
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
