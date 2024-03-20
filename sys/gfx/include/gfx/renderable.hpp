//
// Created by chk on 3/20/2024.
//

#ifndef CHK_PIX_GFX_RENDERABLE_HPP
#define CHK_PIX_GFX_RENDERABLE_HPP

#include <gfx/framebuffer.hpp>

struct IRenderable {
  virtual void render(Framebuffer &target) = 0;
};

#endif // CHK_PIX_GFX_RENDERABLE_HPP
