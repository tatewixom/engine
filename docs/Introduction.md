# Introduction

Welcome to the documentation of _Nuke Engine_! If you were searching for a place to learn, contribute, or are just curious about the engine, this is the right place.

In case you didn't read the **README**, _Nuke Engine_ is a light-weight 3D game engine intended for the developement of a game I am personally making (_Stairs to Hell_), but is open source for other game developers to use. It is under an MIT license, which basically means you can do whatever you want with this code, but I am not liable for anything you do with it (read the LICENSE file in the root of the directory for wordy lawyer-language). 

### Platforms

This engine is targeted for:
* Linux (Debian)
* Windows
* MacOS

If your platform isn't on this list, don't panic. As long as your platform supports,

* A modern **C++ compiler** (C++23)
* and **OpenGL**

then you should be able to use this engine. Even if your platform truly can't support (possibly macOS in the future), you could still benefit greatly from the data structures and algorithms I have created. 

Here's a brief list of functionality provided that might pique your interest:

* Event system
* State management system
* Time system
* Camera system
* Audio system

The list could go on further, but this engine comes with a great set of functionality, intended to be used intuitively, that can jump start your game project with C++.

With all the preliminary knowledge out of the way, why did I make this engine?

## Philosophy and Motivating Problem

Many people have a different idea of what a game engine is. I.e., some people think have a graphical interface for an engine is critical, or perhaps having scripting functionality is non-negotiable. 

My definition, of what this engine is going to be, is simply a C++ library that acts as the structure to make your game work by providing important functionality that a game would entail. For example, some utilities would include an event system, window management system, state manager, etc.

I do want to point out that I do not plan on making this engine remotely similar to what Unity, Unreal, Godot, and other engines offer in their suite of functionality. This is simply meant to be _bare bones_ in terms of 3D graphics. 

Another thing I want to draw attention to is that this isn't meant to be general purpose. I am designing this with the sole-purpose of making _my_ game. I want this to be open-source so that other people who are facing the same problem as me can use it. My philosophy around general-purpose engines follows with the statements:

>"If you design for everyone, you design for no one."

or

>"Jack of all trades, master of none."

These adages highlight that for engines like the aforementioned, they aren't particularly good at everything for the average user. Don't get me wrong, you can make amazing games with those engines, but you're completely locked within their environment, which is made for everyone, so it becomes very hard to fit your specific needs without bloat or restrictions.

So, this is the reason why I am making this light-weight engine. It combines the minimal functionality that I need for 3D graphics and model manipulation, but it keeps the control and power of C++.

Now knowing the philosophy behind the engine, what should you do now?

## Paths

There are two ways you can start with using this engine. You can go down the path of learning how to _use_ the engine, or how to _contribute_ or _extend_ it. 

If you are more interesting in the actually using the functionality and don't care much for the internals of it, I would advise you to go to the **learning** directory within the docs directory (the one you're in now).

Perhaps you are already experienced with engine or are just simply curious how it works, then I'd tell you to head over to the **internals** directory.

My advise for newcomers that haven't an inkling of how to use the engine or what the internals are like, is to start with the **learning** docs. From there you'll be able to gain a high-level understanding of how it works and connect the dots more easily when it comes to learning the internals.

If you want to contribute to the engine, then I'd heavily recommend still starting with the learning path, and only then reading into the internals. Once you have a good grasp of the internals, try and fix problems with the engine that you encountered when learning that weren't intuitive, or that were downright wrong/buggy.

## Conclusion

Now that I have given you some of my doctrine, choose a path to take that suits your needs. Making a game should be intuitive and fun, so following along with the docs should facilitate that experience. 