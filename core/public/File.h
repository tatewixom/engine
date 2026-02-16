#pragma once

#include <string>
#include <string_view>
#include <expected>

namespace Nuke
{
  namespace File
  {
    std::expected<std::string, std::string> retrieve(std::string_view fileName);
  }
}