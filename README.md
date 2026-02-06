# Nuke Engine

_Nuke Engine_ is a light-weight game engine designed for the creation of a future game called _Stairs to Hell_. The engine is meant to be pretty bare-bones, but offer powerful control over what's going on in your game via C++.

It uses a renderer that only allows GLB (or GLTF) formatted 3D models. 

Anyone is free to use any code provided by this open-source engine. As much documentation will be provided as I see necessary.

## Building 

This project uses CMake as its build system. Simply configure and build the project from the root **CMakeLists.txt** file (steps shown below). This is targeted to work on macOS, Linux (Debian), or Windows. That covers (very) roughly 91% of operating systems people use on desktops. If you aren't on those, don't lose hope

You _could_ try and compile this on different distributions of linux or other operating systems that work with:  

* A modern **C++ compiler** (C++23)
* **OpenGL**

_But of course_, keep in mind that there's currently no future of support outside of the aforementioned scope.

>Basically, if your system supports OpenGL and C++ (which is _a lot_), you're golden.

#### Steps for building:

### Step 1:

Create a new **build** folder inside the **engine** directory and set your current directory in your terminal to it:

>cd build

 Then, configure the project using:
 
>cmake ..

### Step 2:

Now that you have configured the project, build the project by running:

>cmake --build .

### Done

Now you have successfully configured and built the game!

## Philosophy and Motivating Problem

As an aside, many people have a different idea of what a game engine is. I.e., some people think have a graphical interface for an engine is critical, or perhaps having scripting functionality is non-negotiable. 

My definition, of what this engine is going to be, is simply a C++ library that acts as the structure to make your game work by providing important functionality that a game would entail. For example, some utilities would include an event system, window management system, state manager, etc.

I do want to point out that I do not plan on making this engine remotely similar to what Unity, Unreal, Godot, and other engines offer in their suite of functionality. This is simply meant to be _bare bones_ in terms of 3D graphics. 

Another thing I want to draw attention to is that this isn't meant to be general purpose. I am designing this with the sole-purpose of making _my_ game. I want this to be open-source so that other people who are facing the same problem as me can use it. My philosophy around general-purpose engines follows with the statements:

>"If you design for everyone, you design for no one."

or

>"Jack of all trades, master of none."

These adages highlight that for engines like the aforementioned, they aren't particularly good at everything for the average user. Don't get me wrong, you can make amazing games with those engines, but you're completely locked within their environment, which is made for everyone, so it becomes very hard to fit your specific needs without bloat or restrictions.

So, this is the reason why I am making this light-weight engine. It combines the minimal functionality that I need for 3D graphics and model manipulation, but it keeps the control and power of C++.

## Scope

The scope of the engine is the hardest part, because making the functionality can mostly be trivial once the guidelines are set. 

#### Platform

To reiterate, the systems I am targeting are:

1. Linux (Debian)
2. Windows
3. macOS

I ordered them in descending order of importance. Linux, being the target platform, at the top, and macOS at the bottom. 

I want to target the big three, but with macOS usually switching to their own proprietary hardware and software, it becomes increasingly hard to accommodate for it. Luckily, macOS provides support for OpenGL up until version 4.1 (though it comes with it's own bugs), which is far above what version I am targeting. Unluckily, it seems that macOS may completely lose support for OpenGL in favor of their **Metal** graphics API in the future. If that may happen, then support for macOS for this engine will cease.

#### Functionality

The systems that I consider "bare bones" to implement are as follows:

* Window management
  * Create valid context
  * Set window icon
  * Set title
* Event system
  * Input detection
  * Event bus
* Time system
  * Timing
  * Clock
* Rendering system
* File management
* Model system
* Animation system
* GLB/GLTF model loader
* State manager
* Camera system
* 3D audio system
  * Directional audio
* UI element system
  * Displaying statistics
  * Debug menu

In the following sub-sections, I will cover in more detail the scope of each system and its components.

##### Window management