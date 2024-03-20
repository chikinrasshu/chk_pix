//
// Created by chk on 3/20/2024.
//

#ifndef CHK_PIX_GFX_FRAMEBUFFER_HPP
#define CHK_PIX_GFX_FRAMEBUFFER_HPP

#include <core/bmp.hpp>
#include <core/core.hpp>
#include <core/vec.hpp>

class Framebuffer {
public:
  explicit Framebuffer(const V2i &size);
  virtual ~Framebuffer();

  b32 resize(const V2i &new_size);

  [[nodiscard]] V2i size() const { return _size; }
  [[nodiscard]] u32 tex() const { return _tex; }
  [[nodiscard]] u32 id() const { return _fbo; }

  static Framebuffer from_bitmap(const Bitmap &bmp);

private:
  V2i _size{0, 0};
  u32 _tex{0}, _ds_tex{0};
  u32 _fbo{0};
};

#endif // CHK_PIX_GFX_FRAMEBUFFER_HPP
