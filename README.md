<div align="center">

# 🎯 Simple CPU Ray Tracer

**A CPU-based ray tracer built from scratch in C++ using openFrameworks**

Implementation of the core ray tracing algorithms presented in *Fundamentals of Computer Graphics*, featuring multiple geometric primitives, Lambertian shading, shadows, object transformations, and interactive camera controls.

<br>

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B&logoColor=white)
![openFrameworks](https://img.shields.io/badge/openFrameworks-0.12.1-009B3A)
![Renderer](https://img.shields.io/badge/Renderer-CPU-orange)
![Platform](https://img.shields.io/badge/Platform-macOS-lightgrey)
![Status](https://img.shields.io/badge/Status-Completed-success)

</div>

---

# 🖼️ Render with perspective rays

<p align="center">
    <img src="assets/render_perspective.png" width="900" alt="Final Render with perspective rays">
</p>

> **Academic Project**
>
> This project was developed as part of a university Computer Graphics assignment. The implementation follows the concepts presented in *Fundamentals of Computer Graphics* by Peter Shirley while being written from scratch in C++ using openFrameworks.

---

# 📖 Overview

This project implements a simple CPU ray tracer capable of rendering multiple geometric primitives with Lambertian illumination and shadow rays.

For each pixel on the screen, a ray is generated from a virtual camera and tested against every object in the scene. The closest intersection is colored and shaded using diffuse lighting while additional shadow rays determine whether each point is illuminated or occluded.

The primary goal of this project was to better understand the mathematics and algorithms behind ray tracing rather than developing a production renderer.

---

# ✨ Features and Technical Highlights

- ✅ Perspective and Parrallel camera rays
- ✅ Sphere intersection
- ✅ Ellipsoid intersection
- ✅ Plane intersection
- ✅ Cube intersection
- ✅ Lambertian diffuse shading
- ✅ Shadow rays
- ✅ Object translation
- ✅ Object rotation
- ✅ Object scaling
- ✅ Mouse object picking
- ✅ Camera pan
- ✅ Camera orbit
- ✅ Automatic re-rendering after changes in scene
- ✅ Render time measurement
- ✅ Self-build math library --> 3DMath
- ✅ Objects with local coordinates

---

# 🖼️ Render with parallel rays

<p align="center">
    <img src="assets/render_parallel.png" width="80%" alt="Final Render with parallel rays">
</p>

---

# 🧠 How It Works

For every pixel, the renderer generates a ray from the active camera (`Camera::ray_tracing`) and tests it against every object in the scene (`Object3D::hit`).

Each object stores both its transformation matrix and its inverse. Rather than transforming the geometry itself, the incoming ray is transformed into the object's local coordinate system before performing the intersection test. This allows each primitive to keep a simple analytical intersection equation while supporting arbitrary translation, rotation, and scaling.

After finding the closest intersection, the renderer casts a shadow ray toward the point light source. If no object blocks the light, Lambertian diffuse illumination is computed using the surface normal and light direction.

---

# 🕹️ Controls

## Camera

| Key | Action |
|------|--------|
| `p` | Toggle Perspective / Orthographic |
| `w a s d` | Pan camera on x and y axis |
| `q e` | Pan camera on z axis |
| `i k` | Orbit vertically |
| `j l` | Orbit horizontally |
| `Shift + r` | Reset camera |

---

## Object Selection & Transformation

*(Select an object first with the mouse.)*

| Key | Action |
|------|--------|
| Mouse Click | Select object |
| Arrow Keys  | Translate object on x and y axis |
| `f b` | Translate object on z axis |
| `n` / `m` | Scale Up / Down |
| `x` `y` `z` | Rotate around X / Y / Z axis|
| `Shift + x / y / z` | Rotate in opposite direction|
| `r` | Reset selected object |

---

# 📂 Project Structure

```text
src
├── main.cpp
├── ofApp.cpp
├── ofApp.h
└── 3DMath.h
```

| File | Description |
|------|-------------|
| `main.cpp` | openFrameworks entry point |
| `ofApp.cpp/.h` | Scene setup, rendering, input handling |
| `3DMath.h` | Math library, Camera, Ray, Object3D and all primitives |

---

# 🛠️ Requirements & Setup

### Requirements

- C++17
- openFrameworks **0.12.1**
- Xcode (macOS)

Tested on:

- macOS
- Apple M1 (2020)

---

### Build

```bash
git clone https://github.com/JPab-yeipi/raytracer-from-scratch.git
```

Move the project inside:

```text
openFrameworks/apps/myApps/your_project/src/
```

Then:

1. Open the `.xcodeproj`
2. Build
3. Run (`⌘ + R`)

---

# 📚 References

- Peter Shirley, Steve Marschner *Fundamentals of Computer Graphics*
- Course material from Universidad Panamericana

---

# ⚠️ Notes & Limitations

- Rendering is performed entirely on the CPU.
- No GPU acceleration or shaders are used.
- Single point light source.
- No reflections or refractions.
- No anti-aliasing.
- Intended as an educational implementation focused on understanding ray tracing fundamentals.

---

<div align="center">

**Made with C++ ❤️ and openFrameworks**

</div>
