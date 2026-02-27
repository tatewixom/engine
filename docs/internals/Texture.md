# Texture

## Outline

This file is an outline for how I am going to handle textures in _Nuke Engine_. I am going to go through the pipeline of what needs to happen for a valid texture to be handled and drawn, and I am going to go about optimizations regarding the drawing and loading process.

##  Pipeline

To start, the general texture pipeline is this:

TinyglTF loads into memory
&darr;
Extract
&darr;
Convert
&darr;
Upload
&darr;
Store

I'll go through each section and what requirements are needed to happen in order for this to be considered _valid_.

### TinyglTF loads into memory

This section is very simple. All we need is the path of the `.gltf` or `.glb` file. We give it to TinyglTF and it handles the loading process from there, giving us access to everything we need. 

### Extract

This is the most complicated aspect of the initialization process. We need an assortment of things in order to say we have a texture. Of course, we first need to ask whether there even is a texture, if there is none, there's no work to do. 

#### Requirements

If there is a texture, here's what we need:

* Load base color texture & factor
* Load metallic texture & factor 
* Load roughness texture & factor 
* Load emissive texture & factor
* Load normal texture & scale
* Load occlusion texture & strength
* Load samplers for each

Essentially everything here is optional. We can know if we have a certain attribute or not by whether a pointer to the index of the attribute is null or not. 

The first thing one must do is get the index of the texture, and access the texture via `model.textures[index]`.

To get the index of a texture, you first need to get the primitive of the model you are currently working with (via `model.meshes[index]`, where the index is just us iterating through the Mesh vector), then get the material index for your primitive with `primitive.material`. Finally, you can get a `tinygltf::Material&` from using `model.material[matieralIndex]`. This allows you to access all of the above requirements for extracting and storing the data.