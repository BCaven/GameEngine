# Blake_GameEngine

This is a game engine built in c++ with CUDA and libtorch support
designed to provide a sandbox for ML-first techniques.

It is also designed to test different rendering techniques with a simple render API that is easy to extend.

# design philosophy

All aspects of the engine are instanced and use factories with simple APIs for developers to use without worrying about memory management or organization.

Core aspects of the engine (e.g., the game loop, render engine, game objects, shaders, etc.) utilize OOP principles to make swapping between different objects and structures (such as switching the scene graph datastructure) is as simple as changing the type in a factory.

# api considerations

More to come.

