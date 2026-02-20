#include "File.h"

#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <iostream>
#include <expected>
#include <filesystem>

namespace Nuke
{
  namespace File
  {
    bool is_valid_file(const std::filesystem::path& path)
    {
      //check if file exists
      if (!std::filesystem::exists(path))
        return false;

      //check if a regular file
      if (!std::filesystem::is_regular_file(path))
        return false;

      return true;
    }

    bool is_valid_directory(const std::filesystem::path& path)
    {
      //check if file exists
      if (!std::filesystem::exists(path))
        return false;

      //check if a regular file
      if (!std::filesystem::is_directory(path))
        return false;

      return true;
    }

    std::expected<std::string, std::string> get_file_contents(const std::filesystem::path& path)
    {
      //attempt opening file
      std::ifstream in_file(path);
      if (!in_file.is_open())
        return std::unexpected{ "FAILURE_IN_OPENING_FILE" };

      //create string buffer
       std::ostringstream buffer;

      //pour contents of file to string buffer
      buffer << in_file.rdbuf();
      return buffer.str();
    }

    Filesystem::Filesystem(const std::filesystem::path &working)
      : working_{ working }
    { }

    std::filesystem::path Filesystem::working() const noexcept
    {
      return working_;
    }

    void Filesystem::working(const std::filesystem::path& new_working) noexcept
    {
      working_ = new_working;
    }

    void Filesystem::mount(const std::filesystem::path& new_mount)
    {
      if (new_mount == std::filesystem::path{})
      {
        std::cout << sysmessage << "passed an empty mount path\n";
        return;
      }

      if (!is_valid_directory(new_mount))
      {
        std::cout << sysmessage << "mount " << new_mount << " doesn't exist.\n";
        return;
      }

      mounts_.push_back(new_mount);
    }

    void Filesystem::list_mounts() const
    {
      std::cout << sysmessage << "Filesystem mounts:\n";
      for (const auto& mount : mounts_)
        std::cout << mount << '\n';
    }

    const std::vector<std::filesystem::path>& Filesystem::get_mounts() const noexcept
    {
      return mounts_;
    }

    std::expected<std::string, std::string> Filesystem::retrieve(const std::filesystem::path& relative_file_path)
    {
      for (const auto& point : mounts_)
      {
        std::filesystem::path mount_path{ point / std::string{ relative_file_path } };

        if (!is_valid_file(mount_path))
          continue;

        auto contents{ get_file_contents(mount_path) };
        return contents;
      }

      std::cout << sysmessage << relative_file_path << " not found in mounted paths.\n";
      std::cout << sysmessage << "Searching for " << relative_file_path << " in working directory...\n";

      //check if there's a working directory
      if (working_ == std::filesystem::path())
        return std::unexpected{ "NO_CURRENT_WORKING_DIRECTORY" };

      //combine root with filename
      std::filesystem::path working_dir_file_path{ working_ / relative_file_path };

      if (!is_valid_file(working_dir_file_path))
      {
        std::cout << sysmessage << "Couldn't find " << relative_file_path << '\n';
          return std::unexpected{ "COULDN'T_FIND_FILE" };
      }

      auto contents{ get_file_contents(working_dir_file_path) };
      return contents;
    }

    std::expected<std::string, std::string> retrieve(const std::filesystem::path& relative_file_path)
    {
      //getting root path of working directory
      std::filesystem::path root{ std::filesystem::current_path() };

      //check if it failed
      if (root == std::filesystem::path())
        return std::unexpected{ "FAILURE_IN_FINDING_CURRENT_DIRECTORY" };

      //combine root with filename
      std::filesystem::path abs_file_path{ root / relative_file_path };

      //check if valid file
      if (!is_valid_file(abs_file_path))
        return std::unexpected{ "FILE_DOESN'T_EXIST_OR_IS_DIRECTORY" };

      return get_file_contents(abs_file_path);
    }
  }
}