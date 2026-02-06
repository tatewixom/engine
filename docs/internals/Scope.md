# Scope

The scope of the engine is the hardest part, because making the functionality can mostly be trivial once the guidelines are set. 

### Platform

To reiterate, the systems I am targeting are:

1. Linux (Debian)
2. Windows
3. macOS

I ordered them in descending order of importance. Linux, being the target platform, at the top, and macOS at the bottom. 

I want to target the big three, but with macOS usually switching to their own proprietary hardware and software, it becomes increasingly hard to accommodate for it. Luckily, macOS provides support for OpenGL up until version 4.1 (though it comes with it's own bugs), which is far above what version I am targeting. Unluckily, it seems that macOS may completely lose support for OpenGL in favor of their **Metal** graphics API in the future. If that may happen, then support for macOS for this engine will cease.

### Functionality

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