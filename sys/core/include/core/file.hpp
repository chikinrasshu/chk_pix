//
// Created by chk on 3/17/2024.
//

#ifndef CHK_PIX_CORE_FILE_HPP
#define CHK_PIX_CORE_FILE_HPP

#include <core/core.hpp>

class File {
public:
  explicit File(const String &path);
  ~File();

  [[nodiscard]] u8 *memory() const { return _memory; }
  [[nodiscard]] size_t memory_size() const { return _memory_size; }

private:
  u8 *_memory{nullptr};
  size_t _memory_size{0};
};

#endif // CHK_PIX_CORE_FILE_HPP
