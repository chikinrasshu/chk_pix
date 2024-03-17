//
// Created by chk on 3/17/2024.
//

#include <core/bmp.hpp>

Bitmap::Bitmap(V2i size, s32 bpp) : _bpp{bpp} { resize(size); }

Bitmap::~Bitmap() {
  if (_memory) {
    delete[] _memory;
    _memory = nullptr;
  }
  _memory_size = 0;
}

b32 Bitmap::debug() {
  if (!_memory) {
    fprintf(stderr, "Debugging uninitialized bitmap\n");
    return false;
  }

  u8 *ptr = memory();
  for (s32 y = 0; y < size().y; ++y) {
    u32 *pixel = reinterpret_cast<u32 *>(ptr);

    r32 g = ((r32)y / (r32)size().y) * 255.0f;
    for (s32 x = 0; x < size().x; ++x) {
      r32 r = ((r32)x / (r32)size().x) * 255.0f;

      u32 packed_color = ((u8)r << 16) | ((u8)g << 8) | (255 << 24);
      *pixel++ = packed_color;
    }
    ptr += pitch();
  }

  return true;
}

b32 Bitmap::resize(V2i new_size) {
  if (_memory) {
    delete[] _memory;
    _memory = nullptr;
  }

  _size = new_size;
  _pitch = static_cast<s32>((sizeof(u8) * _bpp) * _size.x);
  _memory_size = _pitch * _size.y;
  _memory = new u8[_memory_size];
  if (!_memory) {
    return false;
  }

  debug();
  return true;
}