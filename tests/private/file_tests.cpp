#include <doctest.h>
#include <filesystem>
#include <fstream>
#include <expected>
#include <iostream>

#include "File.h"

//get_executable_path()

TEST_CASE("File::get_executable_path() works")
{
  std::filesystem::path exe_path{ Nuke::File::get_executable_path() };
  CHECK(std::filesystem::exists(exe_path));
}

//get_executable_dir()

TEST_CASE("File::get_executable_dir() works")
{
  std::filesystem::path exe_path{ Nuke::File::get_executable_dir() };
  CHECK(std::filesystem::exists(exe_path));
}

//is_valid_file()

TEST_CASE("File::is_valid_file() returns true for existing file")
{
  Nuke::File::create_file("example.txt");
  CHECK(Nuke::File::is_valid_file("example.txt"));
  Nuke::File::remove_file("example.txt");
}

TEST_CASE("File::is_valid_file() returns false for non-existing file")
{
  CHECK(!Nuke::File::is_valid_file("example.txt"));
}

TEST_CASE("File::is_valid_file() returns false for directory")
{
  Nuke::File::create_directory("nuke");
  CHECK(!Nuke::File::is_valid_file("nuke"));
}

//retrieve()

TEST_CASE("File::retrieve() works")
{
  auto file{ Nuke::File::create_file("example.txt") };
  CHECK(file.has_value());

  if (file.has_value())
  {
    file.value() << "Hello, World!\n";
    file.value().close();
  }

  auto contents{ Nuke::File::retrieve("example.txt") };
  CHECK(contents.has_value());

  if (contents.has_value())
    CHECK(contents == "Hello, World!\n");

  Nuke::File::remove_file("example.txt");
}

TEST_CASE("File::retrieve() rejecting non-existing file")
{
  //attempt retrieving non-existing file
  auto file_contents{ Nuke::File::retrieve("example.txt") };
  CHECK(!file_contents.has_value());
}

TEST_CASE("File::retrieve() rejecting directory")
{
  Nuke::File::create_directory("nuke");

  //attempt retrieving directory
  auto file_contents{ Nuke::File::retrieve("nuke") };
  CHECK(!file_contents.has_value());
}

//create_file()

TEST_CASE("File::create_file() works")
{
  auto file{ Nuke::File::create_file("example.txt") };
  CHECK(file.has_value());

  Nuke::File::remove_file("example.txt");
}

TEST_CASE("File::create_file() rejects already existing file")
{
  Nuke::File::create_file("example.txt");

  //try to create again
  auto file{ Nuke::File::create_file("example.txt") };
  CHECK(!file.has_value());

  Nuke::File::remove_file("example.txt");
}

//remove_file()

TEST_CASE("File::remove_file() works")
{
  auto file{ Nuke::File::create_file("example.txt") };
  CHECK(file.has_value());

  Nuke::File::remove_file("example.txt");
  CHECK(!std::filesystem::exists(Nuke::File::get_executable_dir() / "example.txt"));
}

TEST_CASE("File::remove_file() rejects removing directory")
{
  Nuke::File::create_directory("nuke");
  CHECK(!Nuke::File::remove_file("nuke"));
}

//create_directory()

TEST_CASE("File::create_directory() works")
{
  Nuke::File::create_directory("nuke");
  CHECK(std::filesystem::exists(Nuke::File::get_executable_dir() / "nuke"));

  Nuke::File::remove_directory("nuke");
}

TEST_CASE("File::create_directory() rejects making already existing dir")
{
  Nuke::File::create_directory("nuke");

  //try to create again
  CHECK(!Nuke::File::create_directory("nuke"));

  Nuke::File::remove_directory("nuke");
}

//remove_directory()

TEST_CASE("File::remove_directory() works")
{
  Nuke::File::create_directory("nuke");

  Nuke::File::remove_directory("nuke");
  CHECK(!std::filesystem::exists(Nuke::File::get_executable_dir() / "nuke"));
}

TEST_CASE("File::remove_directory() rejects non existing dir")
{
  CHECK(!Nuke::File::remove_directory("nuke"));
}

//Seek

TEST_CASE("File::Seek::clear() works")
{
  Nuke::File::Seek fsystem{};

  if (!Nuke::File::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  fsystem.mount("nuke");
  CHECK(fsystem.mounted("nuke"));

  fsystem.clear();
  CHECK(!fsystem.mounted("nuke"));

  //remove temporary nuke directory
  Nuke::File::remove_directory("nuke");
}

TEST_CASE("File::Seek::remove() works")
{
  Nuke::File::Seek fsystem{};

  if (!Nuke::File::create_directory("nuke"))
    std::cout << "failure in creating directory\n";

  fsystem.mount("nuke");
  CHECK(fsystem.mounted("nuke"));

  fsystem.remove("nuke");
  CHECK(!fsystem.mounted("nuke"));

  //remove temporary nuke directory
  Nuke::File::remove_directory("nuke");
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

TEST_CASE("File::Seek::mount() rejecting non-existing directory")
{
  Nuke::File::Seek fsystem{};

  //mount non-existing
  fsystem.mount("nuke");

  CHECK(!fsystem.mounted("nuke"));
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
  Nuke::File::create_file("example.txt");

  fsystem.mount("nuke");
  fsystem.mount("nuke/test");

  auto contents{ fsystem.retrieve("example.txt") };
  CHECK(!contents.has_value());

  //remove temporary nuke directory
  Nuke::File::remove_directory("nuke");
  Nuke::File::remove_file("nuke");
}