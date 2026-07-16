# Computer Graphics Project 2

This project is an OpenGL-based 3D graphics application written in C++. It demonstrates scene rendering, camera movement, textured models, multiple viewports, and basic GUI integration using ImGui.

## Features

- 3D scene rendering with OpenGL
- Camera movement and mouse-based look control
- Textured objects loaded from model and mesh assets
- Multiple viewports in a single scene
- GUI integration with ImGui
- CMake-based build system

## Project Structure

- src/: C++ source files
- include/: Header files
- models/, meshes/, shaders/, images/: Scene and asset files
- images-in-program/: Screenshot-style images used by the program
- libraries/: Third-party dependencies such as GLFW, GLM, ImGui, and JSON
- build/: Generated build files

## Requirements

- CMake 3.5 or newer
- A C++ compiler with C++17 support
- Windows environment is assumed for the current setup

## Build Instructions

From the repository root, run:

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

If you are using the provided Visual Studio build setup, the project can also be configured through the existing CMake project files.

## Controls

- W / A / S / D: Move the camera
- Right mouse button + drag: Rotate the camera view

## Notes

The scene and model data are loaded from the assets located in the models, meshes, shaders, and images folders. The project uses a custom rendering pipeline built around OpenGL and GLFW.

## Program Screenshots

The images in the images-in-program folder are included below as program output examples:

- image_1.png
- image_2.png
- image_3.png
