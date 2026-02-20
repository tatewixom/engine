#pragma once

#include <string>
#include <string_view>
#include <expected>
#include <vector>
#include <filesystem>

namespace Nuke
{
  namespace File
  {
    class Filesystem
    {
    public:
      Filesystem(const std::filesystem::path& working);

      std::filesystem::path working() const noexcept;
      void working(const std::filesystem::path& new_working) noexcept;

      void mount(const std::filesystem::path& new_mount);
      void list_mounts() const;
      const std::vector<std::filesystem::path>& get_mounts() const noexcept;

      std::expected<std::string, std::string> retrieve(const std::filesystem::path& relative_file_path);

    private:
      std::filesystem::path working_{};
      std::vector<std::filesystem::path> mounts_{};

    private:
      static constexpr const char* sysmessage{ "Nuke::Filesystem : " };
    };

    bool is_valid_file(const std::filesystem::path& path);
    bool is_valid_directory(const std::filesystem::path& path);
    std::expected<std::string, std::string> retrieve(const std::filesystem::path& relative_file_path);
  }
}