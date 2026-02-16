# Introduction

Welcome to the documentation of _Nuke Engine_! If you were searching for a place to learn, contribute, or are just curious about the engine, this is the right place.

In case you didn't read the **README**, _Nuke Engine_ is a light-weight 3D and 2D game engine intended for the developement of a game I am personally making (_Stairs to Hell_), but is open source for other game developers to use. It is under an MIT license, which basically means you can do whatever you want with this code, but I am not liable for anything you do with it (read the LICENSE file in the root of the directory for wordy lawyer-language). 

### Platforms

This engine is targeted for:
* Linux (Debian)
* Windows
* MacOS

If your platform isn't on this list, don't panic. As long as your platform supports

* A modern **C++ compiler** (C++23)
* and **OpenGL**

then you should be able to use this engine. Even if your platform truly can't be supported (possibly macOS in the future), you could still benefit greatly from the data structures and algorithms there is in store. The only real hindrance for full compatability to all platforms is anything **graphics** related (shaders, buffers, model drawing, etc).

Here's a brief list of functionality provided that might pique your interest:

* Event system
* State management system
* Time system
* Camera system
* Audio system

This engine comes with a great set of functionality, intended to be used intuitively, that can jump start your game project with C++.

With all the preliminary knowledge out of the way, why did I make this engine?

## Philosophy and Motivating Problem

Many people have a different idea of what a game engine is. I.e., some people think having a graphical interface for an engine is critical, having scripting functionality is non-negotiable, or some people generally confuse a game engine with a game (they want to make a game and don't care about, for example, how the models are stored and drawn, but decide to make a model renderer that something like Godot does perfectly fine). 

My definition of what this engine is going to be, is simply a C++ library that acts as the structure to make your game work by providing important functionality that a game would entail. For example, some utilities would include an event system, window management system, state manager, etc.

I do want to point out that I do not plan on making this engine with the same rules as Unity and Unreal. Since this is under an MIT license, it is open source and any game made with this engine completely keeps the revenue they earn. We do not charge any hidden fees, royalties, or take any kind of cut. 

Another thing I want to draw attention to is that this isn't meant to be general purpose. I am designing this with the sole-purpose of making _my_ game. However, I want this to be open-source so that other people who are facing the same problem as me can use it. My philosophy around general-purpose engines follows with the statements:

>"If you design for everyone, you design for no one."

or

>"Jack of all trades, master of none."

These adages highlight that for engines like the aforementioned, they aren't particularly good at everything for the average user. Don't get me wrong, you can make amazing games with those engines, but you're completely locked within their environment, which is made for everyone, so it becomes very hard to fit your specific needs without bloat or restrictions. Not to mention, Unreal and Unity take a cut of your game's revenue after a certain amount (I should note that Godot does not do this).

So, this is the reason why I am making this light-weight engine. It combines the minimal functionality that I need for 3D and 2D graphics, but it keeps the control, power, and generality of C++.

Now knowing the philosophy behind the engine, what should you do now?

## Usage

### Prequisites

As with anything complicated in life, you are going to need prerequisites for using this engine. As you should already know, this engine is completely based on C++. It is the backbone that supports the entire structure and you will definitely need to be literate in it. You will also need to be literate in CMake and Git. So, the three recommend prerequisites are:

* C++
* CMake
* Git

If you are a little rusty, or are completely new to C++, then here's some resources I recommend:

#### Beginner

* [Learncpp](www.learncpp.com)

#### Intermediate

* [Leetcode](www.leetcode.com)
* _Effective Modern C++_ by Scott Meyers
* _C++ Software Design_ by Klaus Iglberger
* _C++17 in Detail_ by Bartlomiej Filipek
* _C++ Concurrency in Action_ by Anthony Williams

If you are wanting to use this engine efficiently with git, then I'd recommend reading _Pro Git_ by Scott Chacon.

Since this engine heavily relies on CMake as its build system, then I really recommend learning how to use CMake at:

* [Studyplan](www.studyplan.dev/cmake)

Just ignore the AI profile pictures, apparently the creator got carried away when making them.

### Paths

There are two ways you can start with using this engine. You can go down the path of learning how to _use_ the engine, or how to _contribute_ or _extend_ it. 

If you are more interesting in the just using the functionality and don't care much for contributing to it, I would advise you to go to the **learning** directory within the docs directory (the one you're in now).

If you are instead more interested in contributing to the engine, then it depends on what you're level of understanding is with the engine. For newcomers, I'd still heavily recommend going through the learning docs and only then continuing into the internals. If you have no clue _how_ to use the engine, then knowing _why_ certain things are the way they are is going to be troublesome.

Let's say you came back to this page because you just finished learning how to use the engine, then I would immediately head to the internals and start with the **introduction** document inside the **internals** directory.

Or perhaps you are already experienced with engine or are just simply curious how it works. If so, I'd tell you to head over to the **internals** directory.

My advise for newcomers that haven't an inkling of how to use the engine or what the internals are like, is to start with the **learning** docs. From there you'll be able to gain a high-level understanding of how it works and connect the dots more easily when it comes to learning the internals.

If you want to contribute to the engine, then I'd heavily recommend still starting with the learning path, and only then reading into the internals. Once you have a good grasp of the internals, try and fix problems with the engine that you encountered when learning that weren't intuitive, or that were downright wrong/buggy.

## Conclusion

Now that I have given you some of my doctrine, choose a path to take that suits your needs. Making a game should be intuitive and fun, so following along with the docs should facilitate that experience. 