//
// Created by Jorge Botarro on 19-03-24.
//

#include <core/file.hpp>
#include <core/log.hpp>
#include <gfx/shader.hpp>

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const String &vert_path, const String &frag_path) {
  u32 vert_id = compile_file(vert_path, ShaderKind::Vertex);
  u32 frag_id = compile_file(frag_path, ShaderKind::Fragment);

  _id = glCreateProgram();
  if (_id) {
    glAttachShader(_id, vert_id);
    glAttachShader(_id, frag_id);

    glLinkProgram(_id);

    GLint success = 0;
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
      GLint info_len;
      glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &info_len);
      auto info_buffer = new GLchar[info_len];
      glGetProgramInfoLog(_id, info_len, &info_len, info_buffer);

      Log::warn("Link error: {}", info_buffer);

      delete[] info_buffer;

      glDeleteProgram(_id);
      _id = 0;
    }

    glDetachShader(_id, vert_id);
    glDetachShader(_id, frag_id);

    glDeleteShader(vert_id);
    glDeleteShader(frag_id);
  }
}

Shader::~Shader() {
  glDeleteProgram(_id);
  _id = 0;
}

b32 Shader::bind() {
  b32 result = IBindable::bind();
  if (result) {
    glUseProgram(_id);
  }
  return result;
}

b32 Shader::unbind() {
  b32 result = IBindable::unbind();
  if (result) {
    glUseProgram(0);
  }
  return result;
}

u32 Shader::compile_file(const String &path, ShaderKind kind) {
  File f{path};
  if (f.memory()) {
    return compile_src({reinterpret_cast<char *>(f.memory()), f.memory_size()},
                       kind);
  }
  return 0;
}

u32 Shader::compile_src(StringView src, ShaderKind kind) {
  u32 gl_kind = 0;
  switch (kind) {
  case ShaderKind::Vertex:
    gl_kind = GL_VERTEX_SHADER;
    break;
  case ShaderKind::Fragment:
    gl_kind = GL_FRAGMENT_SHADER;
    break;
  }

  u32 result = glCreateShader(gl_kind);

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

    Log::warn("Compilation error: {}", info_buffer);

    delete[] info_buffer;

    glDeleteShader(result);
    result = 0;
  }

  return result;
}

// Uniform madness
s32 Shader::get_uniform_loc(StringView name) {
  auto it = _cache.find(name);
  if (it != _cache.end()) {
    return it->second;
  }

  s32 loc = glGetUniformLocation(_id, name.data());
  _cache[name] = loc;
  return loc;
}

void Shader::set_uniform(s32 loc, s32 v) {
  bind();
  glUniform1i(loc, v);
}

void Shader::set_uniform(s32 loc, r32 v) {
  bind();
  glUniform1f(loc, v);
}

void Shader::set_uniform(s32 loc, const V2 &v) {
  bind();
  glUniform2f(loc, v.x, v.y);
}

void Shader::set_uniform(s32 loc, const V3 &v) {
  bind();
  glUniform3f(loc, v.x, v.y, v.z);
}

void Shader::set_uniform(s32 loc, const V4 &v) {
  bind();
  glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void Shader::set_uniform(s32 loc, const V2i &v) {
  bind();
  glUniform2i(loc, v.x, v.y);
}

void Shader::set_uniform(s32 loc, const V3i &v) {
  bind();
  glUniform3i(loc, v.x, v.y, v.z);
}

void Shader::set_uniform(s32 loc, const V4i &v) {
  bind();
  glUniform4i(loc, v.x, v.y, v.z, v.w);
}

void Shader::set_uniform(s32 loc, const V2u &v) {
  bind();
  glUniform2ui(loc, v.x, v.y);
}

void Shader::set_uniform(s32 loc, const V3u &v) {
  bind();
  glUniform3ui(loc, v.x, v.y, v.z);
}

void Shader::set_uniform(s32 loc, const V4u &v) {
  bind();
  glUniform4ui(loc, v.x, v.y, v.z, v.w);
}

void Shader::set_uniform(s32 loc, const Mat2 &v) {
  bind();
  glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::set_uniform(s32 loc, const Mat3 &v) {
  bind();
  glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::set_uniform(s32 loc, const Mat4 &v) {
  bind();
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::set_uniform(StringView name, s32 v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, r32 v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const V2 &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const V3 &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const V4 &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const V2i &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const V3i &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const V4i &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const V2u &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const V3u &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const V4u &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const Mat2 &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const Mat3 &v) {
  set_uniform(get_uniform_loc(name), v);
}

void Shader::set_uniform(StringView name, const Mat4 &v) {
  set_uniform(get_uniform_loc(name), v);
}