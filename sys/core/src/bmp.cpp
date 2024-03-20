//
// Created by chk on 3/17/2024.
//

#include <core/bmp.hpp>
#include <core/file.hpp>
#include <core/log.hpp>
#include <stb/stb_image.h>

Bitmap::Bitmap(V2i size, s32 bpp) : _bpp{bpp} { resize(size); }

Bitmap::~Bitmap() {
  if (_memory) {
    if (_from_file) {
      stbi_image_free(_memory);
      _from_file = false;
    } else {
      delete[] _memory;
    }

    _memory = nullptr;
  }
  _memory_size = 0;
}

b32 Bitmap::debug() {
  if (!_memory) {
    Log::warn("Debugging uninitialized bitmap\n");
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
    if (_from_file) {
      stbi_image_free(_memory);
      _from_file = false;
    } else {
      delete[] _memory;
    }
    _memory = nullptr;
  }

  _size = new_size;
  _pitch = static_cast<s32>((sizeof(u8) * _bpp) * _size.x);
  _memory_size = _pitch * _size.y;
  _memory = new u8[_memory_size];

  return true;
}

Bitmap Bitmap::from_file(const String &path, s32 bpp) {
  File file{path};

  s32 file_w, file_h, file_comp;
  stbi_info_from_memory(file.memory(), (s32)file.memory_size(), &file_w,
                        &file_h, &file_comp);

  u8 *memory = stbi_load_from_memory(file.memory(), (s32)file.memory_size(),
                                     &file_w, &file_h, &file_comp, bpp);

  Bitmap result{};
  if (memory) {
    result._from_file = true;
    result._size.x = file_w;
    result._size.y = file_h;
    result._bpp = bpp;
    result._pitch = static_cast<s32>(file_w * sizeof(u8) * result._bpp);
    result._memory_size = result._pitch * result._size.y;
    result._memory = memory;
  }

  return result;
}
