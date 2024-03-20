//
// Created by Jorge Botarro on 19-03-24.
//

#ifndef CHK_PIX_GFX_SHADER_HPP
#define CHK_PIX_GFX_SHADER_HPP

#include <core/bindable.hpp>
#include <core/core.hpp>
#include <core/map.hpp>
#include <core/mat.hpp>
#include <core/vec.hpp>

enum class ShaderKind { Vertex, Fragment };

class Shader : public IBindable {
public:
  Shader(const String &vert_path, const String &frag_path);
  ~Shader();

  b32 bind() override;
  b32 unbind() override;

  s32 get_uniform_loc(StringView name);

  void set_uniform(s32 loc, s32 v);
  void set_uniform(s32 loc, r32 v);
  void set_uniform(s32 loc, const V2 &v);
  void set_uniform(s32 loc, const V3 &v);
  void set_uniform(s32 loc, const V4 &v);
  void set_uniform(s32 loc, const V2i &v);
  void set_uniform(s32 loc, const V3i &v);
  void set_uniform(s32 loc, const V4i &v);
  void set_uniform(s32 loc, const V2u &v);
  void set_uniform(s32 loc, const V3u &v);
  void set_uniform(s32 loc, const V4u &v);
  void set_uniform(s32 loc, const Mat2 &v);
  void set_uniform(s32 loc, const Mat3 &v);
  void set_uniform(s32 loc, const Mat4 &v);

  void set_uniform(StringView name, s32 v);
  void set_uniform(StringView name, r32 v);
  void set_uniform(StringView name, const V2 &v);
  void set_uniform(StringView name, const V3 &v);
  void set_uniform(StringView name, const V4 &v);
  void set_uniform(StringView name, const V2i &v);
  void set_uniform(StringView name, const V3i &v);
  void set_uniform(StringView name, const V4i &v);
  void set_uniform(StringView name, const V2u &v);
  void set_uniform(StringView name, const V3u &v);
  void set_uniform(StringView name, const V4u &v);
  void set_uniform(StringView name, const Mat2 &v);
  void set_uniform(StringView name, const Mat3 &v);
  void set_uniform(StringView name, const Mat4 &v);

  static u32 compile_file(const String &path, ShaderKind kind);
  static u32 compile_src(StringView src, ShaderKind kind);

  [[nodiscard]] u32 id() const { return _id; }

private:
  u32 _id{0};
  StringMap<s32> _cache{};
};

#endif // CHK_PIX_GFX_SHADER_HPP
