# Nuke Engine

_Nuke Engine_ is a light-weight game engine designed for the creation of a future game called _Stairs to Hell_. The engine is meant to be pretty bare-bones, but offer powerful control over what's going on in your game via C++.

It uses a renderer that only allows GLB (or GLTF) formatted 3D models. 

Anyone is free to use any code provided by this open-source engine as it is under an MIT license.

## Building 

This project uses CMake as its build system. Simply configure and build the project from the root **CMakeLists.txt** file (steps shown below). This is targeted to work on macOS, Linux (Debian), or Windows. That covers (very) roughly 91% of operating systems people use on desktops. If you aren't on those, don't lose hope

You _could_ try and compile this on different distributions of linux or other operating systems that work with:  

* A modern **C++ compiler** (C++23)
* **OpenGL**

_But of course_, keep in mind that there's currently no future of support outside of the aforementioned scope.

>Basically, if your system supports OpenGL and C++ (which is _a lot_), you're golden.

### Steps for building:

#### Step 1:

Create a new **build** folder inside the **engine** directory and set your current directory in your terminal to it:

>cd build

 Then, configure the project using:
 
>cmake ..

#### Step 2:

Now that you have configured the project, build the project by running:

>cmake --build .

#### Done

Now you have successfully configured and built the engine!

## Other

if you are interested in the internals or are wanting to learn more about how to use the engine, head over to the **docs** directory! Start with the **introduction** file to get an idea of where to navigate. Thanks for using _Nuke Engine_!