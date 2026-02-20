#include <doctest.h>
#include <filesystem>
#include <fstream>
#include <expected>
#include <iostream>

#include "File.h"

void set_temp_dir()
{
  //set temp dir
  std::filesystem::current_path(std::filesystem::temp_directory_path());
}

TEST_CASE("File::retrieve() works")
{
  set_temp_dir();

  //create file
  std::ofstream file("example.txt");

  //retrieve existing file
  auto file_contents{ Nuke::File::retrieve("example.txt") };

  CHECK(file_contents.has_value());
  CHECK(file_contents.value() == std::string{ "" });

  //delete file
  std::filesystem::remove("example.txt");
}

TEST_CASE("File::retrieve() rejecting nonexisting file")
{
  set_temp_dir();

  //attempt retrieving non-existing file
  auto file_contents{ Nuke::File::retrieve("example.txt") };

  CHECK(!file_contents.has_value());
}

TEST_CASE("Filesystem::retrieval() works")
{
  set_temp_dir();

  Nuke::File::Filesystem fsystem{ std::filesystem::current_path() };

  if (!std::filesystem::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  //mount temp
  fsystem.mount("nuke");

  //create file
  std::ofstream file{ "nuke/example.txt" };

  if (file.is_open())
  {
    file << "Hello, world!\n";
    file.close();
  }
  else
    std::cout << "failure in opening file\n";

  auto contents{ fsystem.retrieve("example.txt") };

  CHECK(contents.has_value());
  CHECK(contents.value() == std::string{ "Hello, world!\n" });

  //remove temporary nuke directory
  std::filesystem::remove_all("nuke");
}

TEST_CASE("Filesystem::retrieve() rejecting nonexistent file")
{
  set_temp_dir();

  Nuke::File::Filesystem fsystem{ std::filesystem::current_path() };

  if (!std::filesystem::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  //mount temp
  fsystem.mount("nuke");

  //don't create file

  auto contents{ fsystem.retrieve("example.txt") };

  CHECK(!contents.has_value());

  //remove temporary nuke directory
  std::filesystem::remove_all("nuke");
}

TEST_CASE("Filesystem::retrieve() rejecting directory argument")
{
  set_temp_dir();

  Nuke::File::Filesystem fsystem{ std::filesystem::current_path() };

  if (!std::filesystem::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  if (!std::filesystem::create_directory("nuke/test"))
    std::cout << "failure in creating directory\n";

  //mount temp
  fsystem.mount("nuke");

  auto contents{ fsystem.retrieve("test") };

  CHECK(!contents.has_value());

  //remove temporary nuke directory
  std::filesystem::remove_all("nuke");
}

TEST_CASE("Filesystem::mount() rejecting non-existing directory")
{
  set_temp_dir();

  Nuke::File::Filesystem fsystem{ std::filesystem::current_path() };

  //mount existing 
  fsystem.mount("nuke");

  CHECK(fsystem.get_mounts().size() == 0);
}

TEST_CASE("Filesystem::mount() works")
{
  set_temp_dir();

  Nuke::File::Filesystem fsystem{ std::filesystem::current_path() };

  if (!std::filesystem::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  //mount existing 
  fsystem.mount("nuke");

  CHECK(fsystem.get_mounts().at(0) == "nuke");

  //remove temporary nuke directory
  std::filesystem::remove_all("nuke");
}
