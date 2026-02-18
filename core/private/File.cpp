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
    #ifdef __WIN32__

    std::expected<std::string, std::string> retrieve(std::string_view fileName)
    {
      std::ifstream in_file(fileName.data());

      if (!in_file.is_open())
        return std::unexpected{"ERROR::FILE.CPP::RETRIEVE()::FAILURE_IN_OPENING_FILE"};

      std::ostringstream buffer;
      buffer << in_file.rdbuf();
      return buffer.str();
    }
    
    #elif __APPLE__

    std::expected<std::string, std::string> retrieve(std::string_view fileName)
    {
      std::ifstream in_file(fileName.data());

      if (!in_file.is_open())
        return std::unexpected{"ERROR::FILE.CPP::RETRIEVE()::FAILURE_IN_OPENING_FILE"};

      std::ostringstream buffer;
      buffer << in_file.rdbuf();
      return buffer.str();
    }
    
    #elif __linux__

    std::expected<std::string, std::string> retrieve(std::string_view fileName)
    {
      std::ifstream in_file(fileName.data());

      if (!in_file.is_open())
        return std::unexpected{"ERROR::FILE.CPP::RETRIEVE()::FAILURE_IN_OPENING_FILE"};

      std::ostringstream buffer;
      buffer << in_file.rdbuf();
      return buffer.str();
    }

    std::expected<std::string, std::string> retrieve_t(std::string_view fileName)
    {
      //getting root path of working directory
      std::filesystem::path root{ std::filesystem::current_path() };

      //check if it failed
      if (root == std::filesystem::path())
        return std::unexpected{"ERROR::FILE.CPP::RETRIEVE()::FAILURE_IN_FINDING_CURRENT_DIRECTORY" };

      //check if desired file exists and is not a directory
      if (std::filesystem::path file{ root.string() + std::string{ fileName }}; 
          std::filesystem::exists(file) && 
          !std::filesystem::is_directory(file))
      {
        std::ifstream in_file(file.string().data());

        if (!in_file.is_open())
          return std::unexpected{"ERROR::FILE.CPP::RETRIEVE()::FAILURE_IN_OPENING_FILE"};

        //create string buffer
        std::ostringstream buffer;

        //pour contents of file to string buffer
        buffer << in_file.rdbuf();
        return buffer.str();
      }
    }

    #endif

  }
}