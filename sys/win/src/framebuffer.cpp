//
// Created by chk on 3/17/2024.
//

#include <win/framebuffer.hpp>

#include <glad/gl.h>

WinFramebuffer::WinFramebuffer() : Bitmap() {}

WinFramebuffer::~WinFramebuffer() {
  if (_initialized) {
    // Free OpenGL resources
    glDeleteBuffers(1, &_ebo);
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);

    // Free Shader
    if (_sh) {
      glDeleteProgram(_sh);
    }

    // Free Texture
    if (_tex) {
      glDeleteTextures(1, &_tex);
    }
  }

  _initialized = false;
  _vao = 0, _vbo = 0, _ebo = 0;
  _sh = 0;
  _tex = 0, _tex_loc = 0;
}

u32 compile_shader(const String &src, u32 kind) {
  u32 result = glCreateShader(kind);

  const auto raw_code = src.data();
  const auto code = static_cast<const GLchar *const *>(&raw_code);
  const auto raw_len = static_cast<GLint>(src.length());
  const auto lens = &raw_len;

  glShaderSource(result, 1, code, lens);
  glCompileShader(result);

  GLint success = 0;
  glGetShaderiv(result, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    GLint info_len;
    glGetShaderiv(result, GL_INFO_LOG_LENGTH, &info_len);
    auto info_buffer = new GLchar[info_len];
    glGetShaderInfoLog(result, info_len, &info_len, info_buffer);

    fprintf(stderr, "Compilation error: %s\n", info_buffer);

    delete[] info_buffer;

    glDeleteShader(result);
    result = 0;
  }

  return result;
}

b32 WinFramebuffer::init() {
  {
    printf("Initializing framebuffer...\n");

    { // Load shader
      String vert_src = R"(
        #version 410
        layout(location = 0) in vec3 in_pos;
        layout(location = 1) in vec2 in_uv;
        out vec2 vert_uv;

        void main() {
          gl_Position = vec4(in_pos, 1);
          vert_uv = in_uv;
        }
      )";

      String frag_src = R"(
        #version 410
        in vec2 vert_uv;
        out vec4 out_col;
        uniform sampler2D tex;

        void main() {
          out_col = texture(tex, vert_uv);
        }
      )";

      u32 vert_id = compile_shader(vert_src, GL_VERTEX_SHADER);
      u32 frag_id = compile_shader(frag_src, GL_FRAGMENT_SHADER);

      _sh = glCreateProgram();
      if (!_sh) {
        fprintf(stderr, "Failed to create the shader program\n");
        return false;
      }

      glAttachShader(_sh, vert_id);
      glAttachShader(_sh, frag_id);

      glLinkProgram(_sh);

      GLint success = 0;
      glGetProgramiv(_sh, GL_LINK_STATUS, &success);
      if (success == GL_FALSE) {
        GLint info_len;
        glGetProgramiv(_sh, GL_INFO_LOG_LENGTH, &info_len);
        auto info_buffer = new GLchar[info_len];
        glGetProgramInfoLog(_sh, info_len, &info_len, info_buffer);

        fprintf(stderr, "Link error: %s\n", info_buffer);

        delete[] info_buffer;

        glDeleteProgram(_sh);
        _sh = 0;
      }

      glDetachShader(_sh, vert_id);
      glDetachShader(_sh, frag_id);

      glDeleteShader(vert_id);
      glDeleteShader(frag_id);
    }

    { // Texture related
      glGenTextures(1, &_tex);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, _tex);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      if (_sh) {
        // Cache the sampler uniform location
        glUseProgram(_sh);
        _tex_loc = glGetUniformLocation(_sh, "tex");
        glUseProgram(0);
      }
    }

    { // Buffer related
      glGenVertexArrays(1, &_vao);
      glGenBuffers(1, &_vbo);
      glGenBuffers(1, &_ebo);

      glBindVertexArray(_vao);

      // Upload vertices
      glBindBuffer(GL_ARRAY_BUFFER, _vbo);
      glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(sizeof(_vertices)),
                   _vertices, GL_STATIC_DRAW);

      // Upload indices
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   static_cast<GLint>(sizeof(_indices)), _indices,
                   GL_STATIC_DRAW);

      // 0: Pos
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WinVertex),
                            reinterpret_cast<void *>(offsetof(WinVertex, pos)));
      glEnableVertexAttribArray(0);

      // 1: UV
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(WinVertex),
                            reinterpret_cast<void *>(offsetof(WinVertex, uv)));
      glEnableVertexAttribArray(1);

      glBindVertexArray(0);
    }

    _initialized = true;
  }

  return _initialized;
}

b32 WinFramebuffer::render() {
  if (!_initialized) {
    return false;
  }

  glUseProgram(_sh);

  // Upload bitmap memory
  u32 format = GL_RGBA;
  switch (bpp()) {
  case 1: {
    format = GL_RED;
  } break;
  case 2: {
    format = GL_RG;
  } break;
  case 3: {
    format = GL_RGB;
  } break;
  case 4: {
    format = GL_RGBA;
  } break;
  default: {
    fprintf(stderr, "Invalid bpp = %d\n", bpp());
    return false;
  }
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size().x, size().y, 0, format,
               GL_UNSIGNED_BYTE, memory());
  glGenerateMipmap(GL_TEXTURE_2D);

  // Bind texture to shader
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, _tex);
  glUniform1i(_tex_loc, 0);

  // Render bitmap to screen
  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(_indices)),
                 GL_UNSIGNED_INT, nullptr);

  return true;
}
