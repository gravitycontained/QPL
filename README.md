# QPL 
QPL (Quick Production Library) is a (windows) C++20 personal utility library with many purposes to help developement.

# Setup

to use this library download the library and select the **include** folder as your include folder and then write `#include <qpl/qpl.hpp>` in your project.
include the src foulder as source files into your project.

there are 3 defines, that, if written **before** `#include <qpl/qpl.hpp>` will have an effect:

- `#define QPL_USE_INTRINSICS` enabled (Intel) intrinsics and let's you use the optimized `qpl::x64_integer` types.
- `#define QPL_USE_VULKAN` this will enable the Vulkan utilites. You have to include and link the [Vulkan](https://www.khronos.org/vulkan/) library, [glm](https://glm.g-truc.net/0.9.9/index.html) and [GLFW](https://www.glfw.org/) in order to compile your program.
- `#define QPL_USE_SFML` this will enable the SFML utilies. You have to inlcude and link the [SFML](https://www.sfml-dev.org/) library in order to compile your program.


# Utilities

----------

`qpl::print` / `qpl::println`

a simpler print to `cout` with some utilies.
Example:

```cpp
	qpl::println(qpl::foreground::light_purple, qpl::background::blue, "Hello Word");
```

![possible output](https://i.imgur.com/JOq8M5I.png)

----------

simple fast random number generater:

```cpp
	auto a = qpl::random(-5, 10);
	auto b = qpl::random(0.1, 0.7);
	auto c = qpl::random(qpl::f32_min, 0.7f);

	static_assert(qpl::is_same<decltype(a), int>());
	static_assert(qpl::is_same<decltype(b), double>());
	static_assert(qpl::is_same<decltype(c), float>());
  ```
  
----------
