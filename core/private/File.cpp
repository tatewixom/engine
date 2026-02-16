#include "File.h"

#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <iostream>
#include <expected>

namespace Nuke
{
  namespace File
  {
    std::expected<std::string, std::string> retrieve(std::string_view fileName)
    {
      std::ifstream in_file(fileName.data());

      if (!in_file.is_open())
        return std::unexpected{"ERROR::FILE.CPP::RETRIEVE()::FAILURE_IN_OPENING_FILE"};

      std::ostringstream buffer;
      buffer << in_file.rdbuf();
      return buffer.str();
    }
  }
}