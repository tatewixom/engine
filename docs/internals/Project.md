# Project

Welcome to the _Project_ text in the internals! This file is meant to describe the current relevant actions to take on this project. For example, there may be a subsection for "cleaning up example.cpp", and it will give a high-level overview of what needs to be done.

This will be divided into epics, issues, and tickets respectively. There should usually only be one epic listed at a time, but there may be more depending on the context.

## Epics

### Model

In `Model.cpp`, there needs to be a huge redesign of everything, mostly having to do with the recursiveness of how models are loaded. This is a problem because if a large model is loaded in, there is a possibility of stack-overflow. This obviously isn't acceptable runtime behavior, so an implementation with _iteration_ is desirable. 

## Tickets

### Change OpenGL Version

Currently, this project is using OpenGL and GLSL version `3.3`. This is incredibly old, as the latest version, `4.6`, was released in 2017. So, I believe that given _8 years_ there should be wide support for this version on systems who actually use OpenGL. 

To make this effective, there needs to be a switch in the GLSL file versions, along with the core profile that's initialized at the beginning of the program. There may also need to be some revisions in the fragment and vertex shader code.

### Deprecate macOS

This project should no longer support macOS because it is the major factor holding back the OpenGL capabilities and is simply too much of a headache to work with. From now on, after cleaning up necessary files that have macOS implementation, only Linux and Windows shall be supported. 

Key changes for this are minimal. The `README.md` needs to be updated, along with other `.md` files, and the `File.cpp` implementation may need some revision.

## Issues

### Camera Starving

There is an issue with Wayland where if the player is moving using the keyboard, then they can't move the camera. This has been solved using a hybrid approach of polling events will callbacks, and storing the state in custom classes. To finish this, there needs to be some unit tests. 