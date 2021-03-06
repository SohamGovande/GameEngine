# OpenGL Game Engine

Features:

- Diffuse lighting ("regular" lighting, which makes sides facing away from the light darker and sides facing toward the light brighter)
- Specular lighting (reflects light from light sources)
- Specular mapping (determines which parts of a model are reflective)
- Normal mapping (adds intricate detail to low-poly models with miniscule performance reprecussions)
- Parallax mapping (adds fake depth to two-dimensional surfaces)
- Flexible JSON model system (see the .JSON files in res/registries, and res/entities)
- Uses my new Binary File Format from my own custom file format converter, which you can find on [this page](https://github.com/SohamGovande/3D-Model-File-Format).
- Terrain multitexturing
- Heightmap procedural terrain generation
- Binary file IO, from my library which you can find on [this page](https://github.com/SohamGovande/Binary-IO).
- Mesh Simplification (Soon, I'm going to implement support for level-of-detail mapping, allowing me to optimize rendering speeds.)
- Framebuffer API
- Efficient resource manager

Planned features:

- Deferred renderer
- Post processing effects
- Custom allocators for the resource manager
- Terrain normal mapping
- More realistic terrain generation
- Water

Hope you enjoy toying with this game engine! The main file is at game/game/src/Application.cpp
