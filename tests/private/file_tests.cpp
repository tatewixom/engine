#include <doctest.h>
#include <filesystem>
#include <fstream>
#include <expected>
#include <iostream>

#include "File.h"

TEST_CASE("File::create_file() works")
{
  auto val{ Nuke::File::create_file("example.txt") };
  CHECK(val.has_value());

  Nuke::File::remove_file("example.txt");
}

TEST_CASE("File::remove_file() works")
{
  auto val{ Nuke::File::create_file("example.txt") };
  CHECK(val.has_value());

  Nuke::File::remove_file("example.txt");
  CHECK(!std::filesystem::exists(Nuke::File::get_executable_dir() / "example.txt"));
}

TEST_CASE("File::create_directory() works")
{
  Nuke::File::create_directory("nuke");
  CHECK(std::filesystem::exists(Nuke::File::get_executable_dir() / "nuke"));

  Nuke::File::remove_directory("nuke");
}

TEST_CASE("File::remove_directory() works")
{
  Nuke::File::create_directory("nuke");
  CHECK(std::filesystem::exists(Nuke::File::get_executable_dir() / "nuke"));

  Nuke::File::remove_directory("nuke");
  CHECK(!std::filesystem::exists(Nuke::File::get_executable_dir() / "nuke"));
}

TEST_CASE("File::retrieve() works")
{
  //create file
  std::ofstream file{ Nuke::File::get_executable_dir() / "example.txt" };

  //make sure file opened
  CHECK(file.is_open());
  file.close();

  //retrieve existing file
  auto file_contents{ Nuke::File::retrieve("example.txt") };
  CHECK(file_contents.has_value());

  //check if string is empty (as it should)
  if (file_contents.has_value())
    CHECK(file_contents.value() == std::string{ "" });

  //delete file
  Nuke::File::remove_file("example.txt");
}

TEST_CASE("File::retrieve() rejecting nonexisting file")
{
  //attempt retrieving non-existing file
  auto file_contents{ Nuke::File::retrieve("example.txt") };
  CHECK(!file_contents.has_value());
}

TEST_CASE("File::Seek::retrieval() works")
{
  Nuke::File::Seek fsystem{};

  if (!Nuke::File::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  //mount temp
  fsystem.mount("nuke");

  //create file
  std::ofstream file{ Nuke::File::create_file("nuke/example.txt").value() };

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
  Nuke::File::remove_directory("nuke");
}

TEST_CASE("File::Seek::retrieve() rejecting nonexistent file")
{
  Nuke::File::Seek fsystem{};

  if (!Nuke::File::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  //mount temp
  fsystem.mount("nuke");

  //don't create file

  //check if it returns an error
  auto contents{ fsystem.retrieve("example.txt") };
  CHECK(!contents.has_value());

  //remove temporary nuke directory
  Nuke::File::remove_directory("nuke");
}

TEST_CASE("File::Seek::retrieve() rejecting directory argument")
{
  Nuke::File::Seek fsystem{};

  if (!Nuke::File::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  if (!Nuke::File::create_directory("nuke/test"))
    std::cout << "failure in creating directory\n";

  //mount temp
  fsystem.mount("nuke");
  auto contents{ fsystem.retrieve("test") };

  CHECK(!contents.has_value());

  //remove temporary nuke directory
  Nuke::File::remove_directory("nuke");
}

TEST_CASE("File::Seek::retrieve() Ambiguous file name error works")
{
  Nuke::File::Seek fsystem{};

  if (!Nuke::File::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  if (!Nuke::File::create_directory("nuke/test"))
    std::cout << "failure in creating directory\n";

  Nuke::File::create_file("nuke/test/example.txt");
  Nuke::File::create_file("nuke/example.txt");

  fsystem.mount("nuke");
  fsystem.mount("nuke/test");

  auto contents{ fsystem.retrieve("example.txt") };
  CHECK(!contents.has_value());

  //remove temporary nuke directory
  Nuke::File::remove_directory("nuke");
}

TEST_CASE("File::Seek::mount() rejecting non-existing directory")
{
  Nuke::File::Seek fsystem{};

  //mount non-existing
  fsystem.mount("nuke");

  CHECK(!fsystem.mounted("nuke"));
}

TEST_CASE("File::Seek::mount() works")
{
  Nuke::File::Seek fsystem{};

  if (!Nuke::File::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  //mount existing 
  fsystem.mount("nuke");

  CHECK(fsystem.mounted("nuke"));

  //remove temporary nuke directory
  Nuke::File::remove_directory("nuke");
}