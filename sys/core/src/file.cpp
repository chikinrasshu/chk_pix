#include <core/file.hpp>

#include <cstdio>
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

    ifs.read(reinterpret_cast<char *>(_memory), _memory_size);
  } else {
    printf("Failed to open file '%s'\n", path.c_str());
  }
}

File::~File() {
  delete[] _memory;
  _memory = nullptr;
  _memory_size = 0;
}
