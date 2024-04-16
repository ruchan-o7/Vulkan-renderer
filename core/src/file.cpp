
#include "file.h"

#include <fstream>

#include "log.h"

namespace vr {

List<char> ReadFile(const std::string& filename) {
  std::ifstream f(filename, std::ios::ate | std::ios::binary);
  if (!f.is_open()) {
    f.close();
    CORE_ERROR("FAILED TO OPEN FILE!");
  }
  size_t fileSize = (size_t)f.tellg();
  List<char> buffer(fileSize);
  f.seekg(0);
  f.read(buffer.data(), fileSize);
  f.close();
  return buffer;
}
}  // namespace vr
