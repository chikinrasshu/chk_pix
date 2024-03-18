//
// Created by chk on 3/17/2024.
//

#ifndef CHK_PIX_BMP_HPP
#define CHK_PIX_BMP_HPP

#include "core.hpp"
#include "vec.hpp"

class Bitmap {
public:
  Bitmap() = default;
  explicit Bitmap(V2i size, s32 bpp = 4);
  virtual ~Bitmap();

  b32 debug();
  b32 resize(V2i new_size);

  static Bitmap from_file(const String &path, s32 bpp = 4);

  // Getters
  [[nodiscard]] V2i size() const { return _size; }
  [[nodiscard]] s32 bpp() const { return _bpp; }
  [[nodiscard]] s32 pitch() const { return _pitch; }
  [[nodiscard]] u8 *memory() const { return _memory; }
  [[nodiscard]] size_t memory_size() const { return _memory_size; }

private:
  b32 _from_file{false};
  V2i _size{0, 0};
  s32 _bpp{4};
  s32 _pitch{0};

  u8 *_memory{nullptr};
  size_t _memory_size{0};
};

#endif // CHK_PIX_BMP_HPP
