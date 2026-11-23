# ML Game Engine
> This project was in early development and is no longer being worked on. \
> Right now I am working on [my path tracer](https://github.com/BCaven/OfflineRaytracingGPU/)

This is a game engine built in C++ with CUDA and LibTorch support
designed to provide a sandbox for ML-first techniques.

It is also designed to test different rendering techniques with a simple render API that is easy to extend.

# design philosophy

All aspects of the engine are instanced and use factories with simple APIs for developers to use without worrying about memory management or organization.

Core aspects of the engine (e.g., the game loop, render engine, game objects, shaders, etc.) utilize OOP inheritance to make swapping between different objects and structures (such as switching the scene graph datastructure) is as simple as changing the type in a factory.

The engine is also designed to provide a test bed for training and deploying machine learning techniques by integrating LibTorch directly into the engine so all data is potentially available to ML models as inputs or GT outputs.

# Why this exists

This engine was created from frustration while working with Unreal Engine 5, specifically when implementing computer vision features in virtual production workflows.

# Roadmap:

The plan is to use this engine to test real-time rendering algorithms. \
On the list so far:
- [ ] Radiance Cascades
- [ ] ReSTIR Hardware RT
- [ ] Neural Rendering
