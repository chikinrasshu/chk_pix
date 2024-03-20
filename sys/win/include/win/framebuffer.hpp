//
// Created by chk on 3/17/2024.
//

#ifndef CHK_PIX_WIN_FRAMEBUFFER_HPP
#define CHK_PIX_WIN_FRAMEBUFFER_HPP

#include <core/bmp.hpp>
#include <core/vec.hpp>

struct WinVertex {
  V3 pos;
  V2 uv;
};

class WinFramebuffer : public Bitmap {
public:
  WinFramebuffer();
  ~WinFramebuffer() override;

  b32 init();
  b32 render();

private:
  b32 _initialized{false};
  u32 _vao{0}, _vbo{0}, _ebo{0};
  u32 _sh{0};
  u32 _tex{0};
  s32 _tex_loc{0};

  WinVertex _vertices[4] = {{{+1, +1, 0}, {1, 0}},
                            {{+1, -1, 0}, {1, 1}},
                            {{-1, -1, 0}, {0, 1}},
                            {{-1, +1, 0}, {0, 0}}};
  u32 _indices[6] = {0, 1, 2, 2, 3, 0};
};

#endif // CHK_PIX_WIN_FRAMEBUFFER_HPP
