# QPL 
QPL (Quick Production Library) is a (windows) C++20 personal utility library with many purposes to help developement.

to use this library download the library and select the **include** folder as your include folder and then write `#include <qpl/qpl.hpp>` in your project.
include the src foulder as source files into your project.

there are 3 defines, that, if written **before** `#include <qpl/qpl.hpp>` will have an effect:

- `#define QPL_USE_INTRINSICS` enabled (Intel) intrinsics and let's you use the optimized `qpl::x64_integer` types.
- `#define QPL_USE_VULKAN` this will enable the Vulkan utilites. You have to include and link the [VUlkan](https://www.khronos.org/vulkan/) library, [glm](https://glm.g-truc.net/0.9.9/index.html) and [GLFW](https://www.glfw.org/) in order to compile your program.
- `#define QPL_USE_SFML` this will enable the SFML utilies. You have to inlcude and link the [SFML](https://www.sfml-dev.org/) library in order to compile your program.
