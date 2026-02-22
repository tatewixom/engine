# Nuke::File Documentation

## Outline

This doc is about the design and interworkings of the main File namespace in _Nuke Engine_. We'll cover motivating problems, guarantees, and philosophies.

## Overall motivating problem

These file functions and classes were made for easier access to files, and the need for file functions that guarantee that all file paths are relative to the executable. Without that guarantee, it's essentially impossible to retrieve files unless you somehow force the user to invoke the program from the same directory, which won't happen. 

So, these functions offer a strong guarantee that all file paths provided to them are relative to the executable parent directory. 

## Classes

### Seek

#### Motivating problem

The **Seek** class was made for the sole-intention of accessing files easier by mounting a directory location to be searched, and then when you want to access a file, you simply call the member function **retrieve()** as shown below: 

> Seek seek{};
> seek.mount("example");
> auto contents{ seek.retrieve("stop_sign.png") };

To explain what just happened here, we created a **Seek** object, mounted a directory _relative to the executable_, and stored the contents of the file in the **contents** variable (I'll eleborate more what it returns in later).

This makes it much easier to access a file's contents by just specifying the name. You could easily get the contents of a file within three lines of code and have a good amount of safety guaranteed when doing it.

This will become useful when wanting to access models. You simply mount your models folder, specify the name, and the engine can easily find it. 

#### Guarantees

The standard file system is very odd in the way that it does things if you're a person who is wanting to get files relative to your executable easily. Why C++ hasn't introduced a **get_executable_dir()** function or something along those lines yet is beyond me, but it is provided here with that exact name in the **File** namespace. 

This class (and every other function in the **File** namespace) provides a guarantee that it will look relative only to the executable. it will not search from the point at which the program was invoked. If you want that type of relativity, use **std::filesystem** functions.

#### Member functions

##### clear()

This clears the underlying **std::set** of all values, essentially erasing all mounts. The only mount that doesn't get erased is the defualt mount to the path of the executable. When clearing, it performs a call to the **get_executable_dir()** function, which reinitializes the executable path cache if it's not in a valid state.

##### remove()

This function removes a specified path given by the user. If the path exists in the set, it gets erased. If it doesn't exist, nothing happens. Note that this function won't allow the removal of the path to the executable mount.

##### mount()

The mount function takes a path relative to the executable, finds the absolute path of it, ensures it exists and is a directory, and inserts it in the underlying **std::set**.

##### list_mounts()

Lists all current mounts using **std::cout**.

##### mounted()

Returns a true or false value indicating whether the mount exists in the underlying **std::set** or not. If it's true, it exists. If it's false, it doesn't exist.

##### retrieve()

Searches through all mounted directory paths and returns the contents the specified file if successful. If two files are found in two mounts with the same name, the retrieve function returns an **std::unexpected**. It is then up to the caller, or engine-user, to decide what to do from there. 

The reason **std::expected** was chosen as a return value instead of using, say, exceptions is because file system errors usually aren't fatal to the program, but do need some type of way to communicate to the caller that it failed. So, exceptions are overkill for this reason and, personally, I believe exceptions are a bad solution to things that aren't **truly** exceptional (an error that the program _cannot_ continue with afterwards);

## Free functions

### get_executable_path()

This integral function is the backbone that makes everything file-related possible. 

This function works for the targeted platforms and it essentially asks the operating system for the path to the executable. This is implemented by using OS specific macros and creating a path for the correct function depending on what OS is used for compilation.

So this function isn't too wasteful, it stores a path variable in cache of the executable path. The first caller that uses this function initializes it, and every subsequent call first makes sure that the path isn't empty and is in a valid state.

### get_executable_dir()

All this function does is call **get_executable_path** and then use the member function **std::filesystem::path::parent_path()** to return the directory in which the executable is located.

### is_valid_file()

Returns whether a given path exists and if it is a regular file. If either of those conditions are not true, it returns false.

### is_valid_dir()

Returns whether a given path exists and if it is indeed a directory. if either of those conditions are not true, it returns false.

### get_file_contents()

**This function isn't recommended to use**, but is meant to be a helper function for retrieve. The reason this isn't hidden is because it could be useful to have a function that quickly returns the contents of a file **not relative to the executable**. This is the only function in the **File** namespace that violates this guarantee and therefore should not be used unless you have a reason why.

### retrieve()

Returns the contents of a file relative to the executable.

### create_file()

Creates a file relative to the executable, opens the file, and returns an **std::ofstream** responsible for accessing that file.

### create_directory()

Creates a directory relative to the executable.

### remove_file()

Removes a file relative to the executable. 

### remove_directory()

Removes a directory relative to the executable.