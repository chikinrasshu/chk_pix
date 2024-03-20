//
// Created by chk on 3/20/2024.
//

#include <core/log.hpp>
#include <gfx/framebuffer.hpp>

#include <glad/gl.h>

Framebuffer::Framebuffer(const V2i &size) {
  glGenFramebuffers(1, &_fbo);
  if (_fbo) {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_tex);
    glGenTextures(1, &_ds_tex);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, _ds_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    resize(size);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           _tex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D, _ds_tex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      Log::warn("OpenGL: Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &_fbo); }

b32 Framebuffer::resize(const V2i &new_size) {
  _size = new_size;
  glBindTexture(GL_TEXTURE_2D, _tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, new_size.x, new_size.y, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, nullptr);

  glBindTexture(GL_TEXTURE_2D, _ds_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, new_size.x, new_size.y, 0,
               GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
  return true;
}

Framebuffer Framebuffer::from_bitmap(const Bitmap &bmp) {
  u32 gl_format = GL_RGB;
  switch (bmp.bpp()) {
  case 1:
    gl_format = GL_RED;
    break;
  case 2:
    gl_format = GL_RG;
    break;
  case 3:
    gl_format = GL_RGB;
    break;
  case 4:
    gl_format = GL_RGBA;
    break;
  default:
    Log::warn("Invalid bpp: {}", bmp.bpp());
  }

  Framebuffer result{bmp.size()};
  glBindTexture(GL_TEXTURE_2D, result._tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, result._size.x, result._size.y, 0,
               gl_format, GL_UNSIGNED_BYTE, bmp.memory());

  return result;
}
