#include <core/file.hpp>
#include <core/log.hpp>

#include <fstream>

File::File(const String &path) {
  std::ifstream ifs(path, std::ios::binary);
  if (ifs) {
    ifs.unsetf(std::ios::skipws);

    std::streampos file_size;
    ifs.seekg(0, std::ios::end);
    file_size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    _memory_size = file_size;
    _memory = new u8[_memory_size];

    ifs.read(reinterpret_cast<char *>(_memory),
             static_cast<std::streamsize>(_memory_size));
  } else {
    Log::warn("Failed to open file '{}'", path.c_str());
  }
}

File::~File() {
  delete[] _memory;
  _memory = nullptr;
  _memory_size = 0;
}
