# QPL 
QPL (Quick Production Library)  - Copyright (c) Daniel Rabl (danielrabl1999@gmail.com). 

This is a C++20 personal utility library with many purposes to help developement.
It uses alot of features from the great [SFML library](https://www.sfml-dev.org/) and builds upon that.
Please note that since this a one-man project there can be mistakes in the implementations.
Features like `qpl::aes`, `qpl::huffman_coding` and `qpl::floating_point<mantissa_bits, exponent_bits>`
were implemented for my personal usage. They might have mistakes and might be slow:
I highly recommend installing the feature from a greater source, especially if deployed in a serious environment.

# Setup

To build this library in a static way, include all files (as well as `.cpp` files) and add `#define QPLDLL_EXPORTS` to the pre-processor defines. This will remove the "inconsistent dll linkage" errors. It is build by default with SFML enabled, so you have to include and link the [SFML](https://www.sfml-dev.org/) library in order to compile your program. SFML can be turned off by defining `QPL_NO_SFML` as a preprocessor define.

There are 3 defines, that, if written **before** `#include <qpl/qpl.hpp>` or defined as a preprocessor define will have a different effect. 
`QPL_USE_ALL` to use all 3 below.

- `#define QPL_USE_INTRINSICS` enables (Intel) intrinsics and let's you use the optimized `qpl::x64_integer` types.
- `#define QPL_USE_VULKAN` this will enable the Vulkan utilites. You have to include and link the [Vulkan](https://www.khronos.org/vulkan/) library, [glm](https://glm.g-truc.net/0.9.9/index.html), [GLFW](https://www.glfw.org/) and [stb_image](https://github.com/nothings/stb) in order to compile your program. (`glfw3.lib` `vulkan-1.lib` linker dependencies)
- SFML is enabled by default. use `#define QPL_NO_SFML` to disable SFML utilies. Read at the start of this readme how to include and link SFML.
- `#define QPL_USE_GLEW` enables glew utilities to use 3D utilities with SFML. you need to include and link the glew [library](http://glew.sourceforge.net/). 

then there is also `#define QPL_BIG_LUT` (which is outside of `QPL_USE_ALL`) which loads up 66 tables with 1024 entries for high precision floating point arithmetic (32768 bits of mantissa accuracy). Without `QPL_BIG_LUT` the 66 tables are loaded with 32 entries and therefore have 1024 accurate bits of mantissa. these look up tables contain constant numbers such as pi, e, ln2, sqrt2, and e^x x∈(-31, 31) (excluding e^0)


these are the required linker dependencies:


- release x86/x64:
```
sfml-graphics.lib
sfml-window.lib
sfml-main.lib
sfml-system.lib
sfml-audio.lib
sfml-network.lib
```
- debug x86/x64:
```
sfml-graphics-d.lib
sfml-window-d.lib
sfml-main-d.lib
sfml-system-d.lib
sfml-audio-d.lib
sfml-network-d.lib
```

There are also the compiled GLEW binaries to use 3D utilities with SFML (`qpl-gl.lib`/`qpl-gl.dll` and `qpl-gl-d.lib`/`qpl-gl-d.dll`). You need to `#define QPL_USE_GLEW` for preprocessor and include & link the glew [library](http://glew.sourceforge.net/). Linker dependencies will look as follows:

- release x86/x64:
```
sfml-graphics.lib
sfml-window.lib
sfml-main.lib
sfml-system.lib
sfml-audio.lib
sfml-network.lib
opengl32.lib
glew32.lib
```
- debug x86/x64:
```
sfml-graphics-d.lib
sfml-window-d.lib
sfml-main-d.lib
sfml-system-d.lib
sfml-audio-d.lib
sfml-network-d.lib
opengl32.lib
glew32.lib
```

----


# QPL Hello World:

```cpp
#include <qpl/qpl.hpp>

struct main_state : qsf::base_state {
	void init() override {

	}
	void updating() override {

	}
	void drawing() override {

	}
};

int main() try {
	qsf::framework framework;
	framework.set_title("QPL");
	framework.set_dimension({ 1400u, 950u });

	framework.add_state<main_state>();
	framework.game_loop();
}
catch (std::exception& any) {
	qpl::println("caught exception:\n", any.what());
	qpl::system_pause();
}
```

# QPL GLEW hello world
```cpp
#include <qpl/qpl.hpp>

struct opengl_state : qsf::base_state {

	void init() override {
		this->clear_color = qpl::rgb(30, 30, 40);

		this->cube = qgl::get_cube();
		this->color_gens.resize(this->cube.size());
	}
	void updating() override {
		this->camera_lock.update(this->camera, *this);
		this->update(this->camera);

		for (qpl::size i = 0u; i < this->cube.size(); ++i) {
			this->color_gens[i].update(this->frame_time_f());
			this->cube[i].color = this->color_gens[i].get() * 2;
		}
		this->cube.update();

		if (this->event().key_single_pressed(sf::Keyboard::Escape)) {
			this->pop_this_state();
		}
	}

	void drawing() override {
		this->draw(this->cube, this->camera);
	}

	qgl::vertex_array<qgl::flag_bit_index, qgl::pos3, qgl::frgb> cube;
	std::vector<qpl::cubic_generator_vector3f> color_gens;
	qsf::camera_cursor_lock camera_lock;
	qpl::camera camera;
};

int main() try {
	qsf::framework framework;
	framework.enable_gl();
	framework.set_title("QPL");
	framework.set_dimension({ 1400u, 950u });

	framework.add_state<opengl_state>();
	framework.game_loop();
}
catch (std::exception& any) {
	qpl::println("caught exception:\n", any.what());
	qpl::system_pause();
}
```

---

I need to create a useful features list here, but I don't have the time to explain all features from 70 thousand lines here ... I really wish to to this in the future and write a useful documentation. Please forgive me.
