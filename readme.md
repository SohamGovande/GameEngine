# OpenGL Game Engine

For a while now, I've been integrating my passions for learning graphics programming with OpenGL, as well as the intricacies of C++ by coding this OpenGL game engine.

Features:

- Diffuse lighting ("regular" lighting, which makes sides facing away from the light darker and sides facing toward the light brighter)
- Specular lighting (reflects light from light sources)
- Specular mapping (determines which parts of a model are reflective)
- Normal mapping (adds intricate detail to low-poly models with miniscule performance reprecussions)
- Flexible JSON model system (see the .JSON files in res/registries, and res/entities)
- Uses my new Binary File Format from my own custom file format converter, which you can find on [this page](https://github.com/Matrix4f/3D-Model-File-Format).
- Terrain multitexturing
- Heightmap procedural terrain generation
- Binary file IO, from my library which you can find on [this page](https://github.com/Matrix4f/Binary-IO).
- Mesh Simplification :D Soon, I'm going to implement support for level-of-detail mapping, allowing me to optimize 