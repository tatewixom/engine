#pragma once

#include <string>
#include <string_view>
#include <expected>
#include <set>
#include <filesystem>

namespace Nuke
{
  namespace File
  {
    std::filesystem::path get_executable_path();
    std::filesystem::path get_executable_dir();

    bool is_valid_file(const std::filesystem::path& path);
    bool is_valid_directory(const std::filesystem::path& path);

    std::expected<std::string, std::string> get_file_contents(const std::filesystem::path& path);
    std::expected<std::string, std::string> retrieve(const std::filesystem::path& relative_file_path);

    std::expected<std::ofstream, std::string> create_file(const std::filesystem::path& file_name);
    bool create_directory(const std::filesystem::path& dir_name);

    bool remove_file(const std::filesystem::path& file_name);
    bool remove_directory(const std::filesystem::path& dir_name);

    class Seek
    {
    public:
      Seek();
      
      void clear();
      void remove(const std::filesystem::path& mount);
      void mount(const std::filesystem::path& new_mount);
      void list_mounts() const;
      bool mounted(const std::filesystem::path& mount) const;

      std::expected<std::string, std::string> retrieve(const std::filesystem::path& relative_file_path);

    private:
      std::set<std::filesystem::path> mounts_{};
    };
  }
}