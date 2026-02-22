#include "File.h"

#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <iostream>
#include <expected>
#include <filesystem>
#include <vector>

#ifdef __WIN32__
#include <windows.h>

namespace Nuke
{
  namespace File
  {
    std::filesystem::path get_executable_path()
    {
      wchar_t buffer[MAX_PATH];
      GetModuleFileName(nullptr, buffer, MAX_PATH);
      return std::filesystem::path{ buffer };
    }

    std::filesystem::path get_executable_dir()
    {
      wchar_t buffer[MAX_PATH];
      GetModuleFileName(nullptr, buffer, MAX_PATH);
      return std::filesystem::path{ buffer }.parent_path();
    }
  }
}

#elif __APPLE__
#include <unistd.h>

namespace Nuke
{
  namespace File
  {
    std::filesystem::path get_executable_path()
    {
      static std::filesystem::path path{};

      //valid cache
      if (path.empty() || !std::filesystem::exists(path))
        path = std::filesystem::canonical("/proc/self/exe");
      
      return path;
    }

    std::filesystem::path get_executable_dir()
    {
      return get_executable_path().parent_path();
    }
  }
}

#elif __linux__
#include <unistd.h>

namespace Nuke
{
  namespace File
  {
    std::filesystem::path get_executable_path()
    {
      static std::filesystem::path path{};

      //valid cache
      if (path.empty() || !std::filesystem::exists(path))
        path = std::filesystem::canonical("/proc/self/exe");
      
      return path;
    }

    std::filesystem::path get_executable_dir()
    {
      return get_executable_path().parent_path();
    }
  }
}

#endif


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

      //check if its a directory
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

    std::expected<std::string, std::string> retrieve(const std::filesystem::path& relative_file_path)
    {
      //combine root with filename
      std::filesystem::path abs_file_path{ get_executable_dir() / relative_file_path };

      //check if valid file
      if (!is_valid_file(abs_file_path))
        return std::unexpected{ "FILE_DOESN'T_EXIST_OR_IS_DIRECTORY" };

      return get_file_contents(abs_file_path);
    }

    std::expected<std::ofstream, std::string> create_file(const std::filesystem::path& file_name)
    {
      std::filesystem::path abs_file_path{ get_executable_dir() / file_name };
      std::ofstream file{ abs_file_path };

      if (!file.is_open())
        return std::unexpected{ "FAILED_TO_CREATE_FILE" };

      return std::move(file);
    }

    bool create_directory(const std::filesystem::path& dir_name)
    {
      std::filesystem::path dir_path{ get_executable_dir() / dir_name };
      return std::filesystem::create_directory(dir_path);
    }

    bool remove_file(const std::filesystem::path& file_name)
    {
      std::filesystem::path file_path{ get_executable_dir() / file_name };
      return std::filesystem::remove(file_path);
    }

    bool remove_directory(const std::filesystem::path& dir_name)
    {
      std::filesystem::path file_path{ get_executable_dir() / dir_name };
      return std::filesystem::remove_all(file_path);
    }

    Seek::Seek()
    {
      mounts_.insert(get_executable_dir());
    }

    void Seek::clear()
    {
      mounts_.clear();

      mounts_.insert(get_executable_dir());
    }

    void Seek::remove(const std::filesystem::path& mount)
    {
      mounts_.erase(mount);
    }

    void Seek::mount(const std::filesystem::path& new_mount)
    {
      if (new_mount == std::filesystem::path{})
        return;

      std::filesystem::path abs_mount_path{ get_executable_dir() / new_mount };

      if (!is_valid_directory(abs_mount_path))
        return;

      mounts_.insert(abs_mount_path);
    }

    void Seek::list_mounts() const
    {
      for (const auto& mount : mounts_)
        std::cout << mount << '\n';
    }

    bool Seek::mounted(const std::filesystem::path& mount) const
    {
      if (mounts_.contains(std::filesystem::path{ get_executable_dir() / mount }))
        return true;

      return false;
    }

    std::expected<std::string, std::string> Seek::retrieve(const std::filesystem::path& relative_file_path)
    {
      std::vector<std::filesystem::path> file_paths{};
      file_paths.reserve(3);

      //check mounted directories for file
      for (const auto& mount : mounts_)
      {
        std::filesystem::path path{ mount / relative_file_path };

        if (!is_valid_file(path))
          continue;

        file_paths.push_back(path);
      }

      //found unique file
      if (file_paths.size() == 1)
      {
        auto contents{ get_file_contents(file_paths.at(0)) };
        return contents;
      }

      //didn't find file with matching name
      if (file_paths.size() < 1)
        return std::unexpected{ "FAILURE_IN_FINDING_FILE" };
      
      //found more than one file with same name
      std::cout << "ERROR: Files with same name in different mounts:\n";
      for (const auto& p : file_paths)
        std::cout << p << '\n';
      
      return std::unexpected{ "AMBIGUOUS_FILE_RETRIEVAL" };
    }
  }
}