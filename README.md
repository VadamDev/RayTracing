<h1 align="center">Ray Tracing</h1>
<p align="center">A rudimentary (and slow) software ray(path)tracer</a>. Made from scratch in C++</p>

### Features
- Live ECS based editor.
- Scene system with json based load/save
- GPU accelerated (compute shader) raytracer with Spheres, Boxes and Triangle intersections
- .obj model loading
- Depth of Field
- Naive, ray diverge based Anti-Aliasing

#### Planned
- SAH BVH (see: https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/)
- Multiple Importance Sampling (see: https://lisyarus.github.io/blog/posts/multiple-importance-sampling.html / https://www.youtube.com/watch?v=p772XkEnEIU)

## Dependencies
- [spdlog](https://github.com/gabime/spdlog) - 1.16.0
- [glad](https://github.com/dav1dde/glad) - 0.1.36
- [glm](https://github.com/g-truc/glm) - 1.0.2
- [glfw](https://github.com/glfw/glfw) - 3.4
- [entt](https://github.com/skypjack/entt) - 3.16.0
- [nlohmann_json](https://github.com/nlohmann/json) - 3.12.0
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) - 2.0.0rc13
- [ImGui](https://github.com/ocornut/imgui) - 1.91.9
- [portable-file-dialogs](https://github.com/samhocevar/portable-file-dialogs) - 0.1.0

*Dependencies are managed using Vcpkg and CMake*

## Resources Used

- The only, "Ray Tracing in One Weekend": https://raytracing.github.io/books/RayTracingInOneWeekend.html
- Sebastian Lague's raytracing serie: https://www.youtube.com/playlist?list=PLFt_AvWsXl0dlgwe4JQ0oZuleqOTjmox3
- PCG random number generators in glsl: https://observablehq.com/@riccardoscalco/pcg-random-number-generators-in-glsl
- DomNomNom's Ray-AABB intersection: https://gist.github.com/DomNomNom/46bb1ce47f68d255fd5d
- Jacob Gordiak's raytracing serie: https://www.youtube.com/@JacobGordiak
- The Cherno's game engine youtube serie: https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT
