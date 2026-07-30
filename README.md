# RenderForge

RenderForge is a real-time 3D graphics rendering engine and interactive scene viewer built from scratch using modern C++ (C++17) and OpenGL (3.3 Core Profile). The primary objective of the project is to demonstrate core computer graphics principles, including a custom graphics pipeline, a hierarchical scene graph architecture, data-driven JSON asset management, multi-viewport rendering, and real-time interactive parameter controls.

## About the Project

RenderForge addresses the need for a transparent, modular, and lightweight 3D graphics engine that exposes low-level rendering mechanics without relying on monolithic commercial game engines. Commercial engines often obscure GPU buffer management, transformation matrix cascades, shader bindings, and viewport scissor operations.

The application initializes an OpenGL context using GLFW and GLAD, loads 3D meshes, shaders, textures, and scene object configurations via custom JSON specifications, and executes a real-time frame loop. Within this loop, the engine evaluates spatial transform hierarchies, manages independent viewports with distinct cameras, executes GLSL shaders, and renders 3D geometry. Furthermore, it integrates Dear ImGui to provide runtime control over scene graph nodes, object transforms, camera attachment, and object instantiation.

## Screenshots and Demo

You can click and watch the video.

[![RenderForge](https://img.youtube.com/vi/Djb5ZFyzdWA/maxresdefault.jpg)](https://www.youtube.com/watch?v=Djb5ZFyzdWA)

## Features

* **Custom OpenGL Rendering Pipeline:** Directly manages OpenGL Vertex Buffer Objects (VBO), Index Buffer Objects (IBO), and Vertex Array Objects (VAO) to execute indexed drawing commands (`glDrawElements`).
* **Hierarchical Scene Graph Architecture:** Implements a parent-child object hierarchy (`SceneObject`) where child nodes recursively inherit concatenated world transformation matrices from their parent nodes.
* **Multi-Viewport & Multi-Camera Support:** Renders a 3D scene through multiple independent viewports (`ViewPort`) simultaneously using `glViewport` and `glScissor`, with each viewport bound to an individual perspective camera (`Camera`).
* **Data-Driven JSON Asset System:** Parses shader specifications, texture maps, model hierarchy metadata, and 3D mesh geometry (vertices, UV coordinates, indices) from `.json` and `.mesh` files using `nlohmann/json`.
* **Procedural Primitive Mesh Generation:** Programmatically generates vertex and index data for geometric primitives (circles, cones, cylinders, and composite camera indicators) via `MeshLoader`.
* **Real-Time Graphical User Interface (GUI):** Integrates Dear ImGui to display an interactive scene tree, runtime transformation sliders (position, euler rotation, scale), object removal, shape creation menus, and camera attachment options.
* **Interactive First-Person Camera Controls:** Offers keyboard translation (W/A/S/D) for spatial navigation and right-click mouse dragging for camera pitch and yaw rotations.
* **Dynamic Texture & Shader Management:** Utilizes singleton managers (`TextureManager`, `ShaderManager`) to load, compile, link, cache, and activate GLSL shaders and 2D image textures (`stb_image`) on demand.

## Technologies Used

* **C++ (C++17)** — Core programming language used for object-oriented engine architecture, memory management, and system logic.
* **OpenGL (v3.3 Core Profile)** — Low-level 3D graphics API used for hardware-accelerated rendering, buffer binding, depth testing, and scissor operations.
* **GLFW (v3.4)** — Cross-platform library used for window creation, OpenGL context initialization, and handling keyboard/mouse input callbacks.
* **GLAD** — OpenGL function loading library used to resolve OpenGL function pointers at runtime.
* **GLM (OpenGL Mathematics)** — Mathematics library used for 3D vectors, matrix transformations (translation, rotation, scaling), and Left-Handed perspective projection matrices (`glm::perspectiveLH`).
* **Dear ImGui** — Immediate-mode graphical user interface library used for building real-time scene control windows, transform adjustment sliders, and object property inspectors.
* **nlohmann/json** — Header-only JSON library used to parse mesh data (`.mesh`), model metadata (`.json`), and shader manifests (`.json`).
* **stb_image (stb)** — Public domain single-header image loader used to read image files (`.jpg`, `.png`) into RAM for OpenGL 2D texture generation (`glGenTextures`, `glTexImage2D`, `glGenerateMipmap`).
* **CMake (v3.5+)** — Cross-platform build system generator used to configure project compilation, manage include paths, and link external static/shared libraries.

## Project Structure

```text
RenderForge/
├── CMakeLists.txt             # Main CMake build configuration file
├── README.md                  # Project documentation
├── include/                   # Engine header files (.hpp)
│   ├── camera.hpp             # Perspective camera class definition
│   ├── glwindow.hpp           # GLFW window wrapper & callback declarations
│   ├── indexbuffer.hpp        # OpenGL Index Buffer Object (IBO) wrapper
│   ├── meshloader.hpp         # JSON mesh parser & procedural shape builder
│   ├── model.hpp              # Model class binding VAO, shader & texture
│   ├── scene.hpp              # Scene manager, multi-viewport rendering & ImGui GUI
│   ├── sceneobject.hpp        # Scene graph base node & tree logic
│   ├── shadermanager.hpp      # Shader compilation, linking & caching manager
│   ├── shaderprogram.hpp      # GLSL shader program binding & uniform interface
│   ├── texturemanager.hpp     # Image loading & 2D texture cache manager
│   ├── transform.hpp          # 3D spatial transformation matrix calculations
│   ├── vertexarrayobject.hpp  # OpenGL Vertex Array Object (VAO) wrapper
│   ├── vertexbuffer.hpp       # OpenGL Vertex Buffer Object (VBO) wrapper
│   └── viewport.hpp           # Viewport region rendering & scissor configuration
├── src/                       # Engine source implementation files (.cpp)
│   ├── main.cpp               # Application entry point & scene initialization
│   ├── camera.cpp             # Perspective projection & view matrix updates
│   ├── glad.c                 # GLAD function loader implementation
│   ├── glwindow.cpp           # GLFW window management & main rendering loop
│   ├── meshloader.cpp         # Mesh loading routines & procedural geometry
│   ├── model.cpp              # Model JSON parsing & draw calls
│   ├── scene.cpp              # Scene graph traversal, GUI layout & viewport execution
│   ├── sceneobject.cpp        # Node hierarchy management & matrix propagation
│   ├── shadermanager.cpp      # Shader file parsing & compilation routines
│   ├── texturemanager.cpp     # stb_image integration & OpenGL texture binding
│   ├── transform.cpp          # Extrinsic/intrinsic Euler rotations & matrix composition
│   └── viewport.cpp           # Viewport activation & background buffer clears
├── shaders/                   # GLSL shader files & JSON definitions
│   ├── UnlitTexturedShader.json
│   └── UnlitTexturedShader/
│       ├── unlittextureshader_vertex.glsl
│       └── unlittextureshader_fragment.glsl
├── models/                    # Model definition JSON files (Cube, Wall, Floor, etc.)
├── meshes/                    # Pre-defined JSON mesh geometry files (.mesh)
├── images/                    # Texture image files (.jpg, .png)
├── images-in-program/         # Application screenshots used for documentation
└── libraries/                 # Third-party dependencies (GLFW, GLM, ImGui, nlohmann, stb)
```

## Technical Details

### 1. Scene Graph Architecture & Matrix Propagation
RenderForge models spatial object hierarchies using a composite design pattern implemented in `SceneObject`. Every renderable component or group node inherits from `SceneObject` and stores pointers to its children (`m_childs`).

Transformations are calculated hierarchically during scene traversal:
* Each node computes its local `worldMatrix` in `Transform::update()` by combining translation (`mtxTranslate`), rotation (`rotationMatrix`), and scaling (`mtxScale`):
  $$\text{worldMatrix} = \text{mtxTranslate} \times \text{rotationMatrix} \times \text{mtxScale}$$
* Matrix transformations cascade down the scene hierarchy via `SceneObject::updateChilds()`. A parent node multiplies its combined matrix by its local world matrix and passes the result to its children:
  $$\text{childCombinedMatrix} = \text{parentCombinedMatrix} \times \text{parentWorldMatrix}$$
* When rendering a `Model`, the final model matrix uploaded to the GPU shader uniform is:
  $$\text{FinalWorld} = \text{parentCombinedMatrix} \times \text{worldMatrix}$$

### 2. Intrinsic vs. Extrinsic Euler Rotations
The engine's `Transform` class supports two rotation modes specified by `isInstrinsic`:
* **Extrinsic Rotations:** Computes global-axis rotation matrices by applying sequential rotations around fixed world axes:
  $$\mathbf{R} = \mathbf{R}_z(\theta_z) \mathbf{R}_y(\theta_y) \mathbf{R}_x(\theta_x)$$
* **Intrinsic Rotations:** Computes body-fixed rotations by dynamically transforming local axis vectors (`vecRight`, `vecUp`, `vecLook`) according to incremental yaw, pitch, and roll angles. The normalized directional vectors form the columns of the final rotation matrix $\mathbf{R}$.

### 3. Multi-Viewport & Multi-Camera Rendering System
The `ViewPort` class abstracts sub-regions of the application window:
* Each viewport defines screen bounds $(x, y, \text{width}, \text{height})$ and a target clear color.
* During the frame render loop (`Scene::draw()`), `ViewPort::activateScissor()` invokes `glScissor(x, y, width, height)` to constrain clearing operations (`glClear`) to that specific rectangular area.
* `glViewport(x, y, width, height)` is called, and the viewport's attached `Camera` uploads its perspective view-projection matrix ($\mathbf{P} \times \mathbf{V}$) to the active GLSL shader.
* This setup allows rendering the same 3D world simultaneously from different camera viewpoints (e.g., a primary main-view camera and a secondary top-right corner picture-in-picture camera).

### 4. Data-Driven JSON Asset System & Singleton Resource Caching
To decouple code logic from hardcoded assets:
* **`MeshLoader`:** Reads `.mesh` JSON files containing vertex position/texture attributes and element index lists. It initializes OpenGL `VertexBuffer` and `IndexBuffer` objects, configures attribute pointers via `VertexArrayObject`, and caches VAO instances in an `std::unordered_map` to prevent redundant memory allocations.
* **`ShaderManager`:** Reads shader JSON manifests specifying GLSL vertex/fragment filenames and uniform parameter names (`uWorldMatrix`, `uProjectionMatrix`). It compiles GLSL source code, links programs via `ShaderProgram`, queries uniform locations, and caches compiled programs.
* **`TextureManager`:** Reads image files via `stb_image`, transfers raw pixel buffers to GPU memory via `glTexImage2D`, generates mipmaps (`glGenerateMipmap`), and caches OpenGL texture handles by filename.

## Installation and Execution

### Prerequisites
* **Operating System:** Windows OS (pre-configured CMake setup targets Windows development environments).
* **Build Tool:** CMake (version 3.5 or higher).
* **Compiler:** C++17 compliant C++ compiler (e.g., MSVC, GCC, Clang).

### Build Steps

1. **Clone the repository:**
   ```bash
   git clone https://github.com/MuhammedYusufOngel/RenderForge.git
   cd RenderForge
   ```

2. **Generate build files via CMake:**
   ```bash
   cmake CMakeLists.txt -B build
   ```

3. **Compile the binary:**
   ```bash
   cmake --build build
   ```

4. **Execute the application:**
   ```bash
   cd build
   .\Debug\build.exe
   ```
   *(Note: The working directory must allow relative path resolving to `../models`, `../meshes`, `../shaders`, and `../images`)*.

## Usage

Once the application launches:
1. **Camera Controls:**
   * **`W` Key:** Move camera forward along local look vector.
   * **`S` Key:** Move camera backward.
   * **`A` Key:** Strafe camera left along local right vector.
   * **`D` Key:** Strafe camera right.
   * **Right Mouse Button + Drag:** Rotate camera orientation (yaw and pitch).

2. **Graphical Interface Controls (Dear ImGui):**
   * **Scene Window:** Displays the hierarchy tree (`TreeNode`) of scene nodes starting from `Root`. Clicking an object selects it.
   * **Properties Window:**
     * **Transformation:** Modify selected object's `Position` $(x, y, z)$, `Rotation` $(\theta_x, \theta_y, \theta_z)$, and `Scale` $(s_x, s_y, s_z)$ via interactive float sliders.
     * **Create Shape:** Select a primitive type (Cube, Pyramid, Camera, Cylinder) from the dropdown menu and click **Create Shape** to attach a new node under the selected object.
     * **Attach to Camera:** Binds the selected object's spatial transform directly to a camera attached to the secondary viewport.
     * **Remove Button:** Recursively deletes the selected object and all child nodes from the scene graph.

## Development Process

During the development process of this project, AI-assisted tools were utilized for code suggestions, error diagnostics, technical research, and workflow acceleration. The core engine architecture, technical decision-making, component integrations, testing, and feature implementations were evaluated, designed, and executed by the developer.

## Future Improvements

* **Illumination & Lighting System:** Implement active directional, point, and spot lighting models (Phong/Blinn-Phong illumination) within GLSL fragment shaders (currently `SceneObjectType::Light` enum exists, but active shaders operate in unlit textured mode).
* **Material System Expansion:** Extend shader and model JSON configurations to support specular maps, normal maps, metallic/roughness values, and ambient occlusion parameters.
* **Standard 3D Asset Format Importing:** Expand `MeshLoader` to parse standard 3D asset formats (such as OBJ or FBX using the bundled `OpenFBX` library).
* **Spatial Partitioning & Frustum Culling:** Integrate Bounding Volume Hierarchies (BVH) or Octrees alongside view-frustum culling to optimize rendering efficiency for complex scenes.
* **Post-Processing Framebuffer Effects:** Introduce Framebuffer Objects (FBO) for off-screen rendering and post-processing filters such as bloom, tone mapping, and anti-aliasing.

## Developer

* **Developer:** Muhammed Yusuf Öngel
* **GitHub:** [https://github.com/MuhammedYusufOngel]
* **LinkedIn:** [https://www.linkedin.com/in/muhammed-yusuf-öngel-56a399302/]
* **Portfolio:** [https://muhammedyusufongel.github.io]
