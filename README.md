# QPL 
QPL (Quick Production Library)  - Copyright (c) Daniel Rabl (danielrabl1999@gmail.com). 

This is a C++20 personal utility library with many purposes to help developement.
It uses alot of features from the great [SFML library](https://www.sfml-dev.org/) and builds upon that.
Please note that since this a one-man project there can be mistakes in the implementations.
Features like `qpl::aes`, `qpl::huffman_coding` and `qpl::floating_point<mantissa_bits, exponent_bits>`
were implemented for my personal usage. They might have mistakes and might be slow:
I highly recommend installing the feature from a greater source, especially if deployed in a serious environment.

# License
This is free for any use (commercial or personal, proprietary or open-source). The source code is distributed under the [zlib/png license](https://opensource.org/licenses/Zlib)

# Setup

To use this library download the zip and include the include folder. Copy the `qpl.dll` and `qpl-d.dll` into your project folder. It is build with SFML enabled, so you you have to include and link the [SFML](https://www.sfml-dev.org/) library in order to compile your program. 
these are the required linker dependencies:


release x86/x64:
```
sfml-graphics.lib
sfml-window.lib
sfml-main.lib
sfml-system.lib
sfml-audio.lib
sfml-network.lib
qpl.lib
```
debug x86/x64:
```
sfml-graphics-d.lib
sfml-window-d.lib
sfml-main-d.lib
sfml-system-d.lib
sfml-audio-d.lib
sfml-network-d.lib
qpl-d.lib
```

When building yourself, there are 3 defines, that, if written **before** `#include <qpl/qpl.hpp>` or defined as a preprocessor define will have a different effect. 
`QPL_USE_ALL` to use all 3 below.

- `#define QPL_USE_INTRINSICS` enables (Intel) intrinsics and let's you use the optimized `qpl::x64_integer` types.
- `#define QPL_USE_VULKAN` this will enable the Vulkan utilites. You have to include and link the [Vulkan](https://www.khronos.org/vulkan/) library, [glm](https://glm.g-truc.net/0.9.9/index.html), [GLFW](https://www.glfw.org/) and [stb_image](https://github.com/nothings/stb) in order to compile your program. (`glfw3.lib` `vulkan-1.lib` linker dependencies)
- SFML is enabled by default. use `#define QPL_NO_SFML` to disable SFML utilies. Read at the start of this readme how to include and link SFML.

then there is also `#define QPL_BIG_LUT` (which is outside of `QPL_USE_ALL`) which loads up 66 tables with 1024 entries for high precision floating point arithmetic (32768 bits of mantissa accuracy). Without `QPL_BIG_LUT` the 66 tables are loaded with 32 entries and therefore have 1024 accurate bits of mantissa. these look up tables contain constant numbers such as pi, e, ln2, sqrt2, and e^x x∈(-31, 31) (excluding e^0)

If you want to use this library in a static way, include all files (as well as `.cpp` files) and add `#define QPLDLL_EXPORTS` to the pre-processor defines. This will remove the "inconsistent dll linkage" errors.

# Utilities

**qpl::println**:
```cpp
qpl::println("Hello World");
qpl::println("a", 5.5, 'x');
qpl::println(std::vector{ 1, 2, 3 });
qpl::println(std::array{ 4, 5, 6 });
```

output:
```
Hello World
a5.5x
{1, 2, 3}
{4, 5, 6}
```

**random**:
```cpp
auto n = qpl::random(1, 100);
auto f = qpl::random(0.5, 2.0);

auto v = std::vector{ 1, 2, 3 };
qpl::shuffle(v);

auto e = qpl::random_element(v);
	
qpl::println("n = ", n);
qpl::println("f = ", f);
qpl::println("v = ", v);
qpl::println("e = ", e);
```

**string**:
```cpp
qpl::println(qpl::to_string("Hello World"));
qpl::println(qpl::to_string("a", 5.5, 'x'));
qpl::println(qpl::to_string(std::vector{ 1, 2, 3 }));
qpl::println(qpl::to_string(std::array{ 4, 5, 6 }));
qpl::println(qpl::hex_string(5910));
qpl::println(qpl::binary_string(5910));
qpl::println(qpl::base_string(5910, 7));
qpl::println(qpl::string_cast<qpl::u32>("5012"));
qpl::println(qpl::string_cast<qpl::u32>(std::vector{"123", "700", "1337"}));
qpl::println(qpl::split_string("a b c d e f g", ' '));
qpl::println(qpl::split_string("I.\nListen.\n.To.\nYou.\n", ".\n"));
qpl::println(qpl::to_upper("uppercase"));
qpl::println(qpl::to_lower("LOWERCASE"));
qpl::println(qpl::random_lowercase_number_string(50));
for (qpl::u32 i = 0u; i < 5; ++i) {
	auto s = qpl::random_lowercase_uppercase_string(qpl::random(10, 15));
	qpl::println(qpl::str_rspaced(s, 20, ' '));
}
for (qpl::u32 i = 0u; i < 5; ++i) {
	auto n = std::pow(10, qpl::random(2.0, 30.0));
	qpl::println(qpl::str_rspaced(qpl::big_number_string(n), 20, '_'));
}
```
output:
```
Hello World
a5.5x
{1, 2, 3}
{4, 5, 6}
0x1716
1011100010110
23142
5012
{123, 700, 1337}
{a, b, c, d, e, f, g}
{I, Listen, To, You}
UPPERCASE
lowercase
w0v39gk7zy1qu21fdgq24x5e5n3j97z18264248241nl8a3l0c
     VKGtlKdifaUWbjj
     JxNoqBkjZXNeRkx
      hkCVtrAMlBmQbt
        yUMsJlUmAttc
       oOzfflfuqLKrs
___________252.06trn
_____________353.09k
___________39.42sext
__________10.14quint
____________1.09quad
```

**more examples will appear here soon**

----------

**Graph example**: 

```cpp
#include <qpl/qpl.hpp>

struct game_state : qsf::base_state {

	void init() override {
		this->graph.dimension = this->dimension();
		this->graph.font = "arial";

		this->graph.get_simple_graph("random").color = qsf::rgb::grey;
		this->graph.get_simple_graph("ema").color = qsf::rgb::green;

		qpl::f64 value = 0.5;
		this->ema.time_period = 10;
		for (qpl::u32 i = 0u; i < 100; ++i) {
			value += qpl::random(-0.1, 0.1);
			this->ema.add_value(value);

			this->graph.get_simple_graph("random").add_data(value);
			this->graph.get_simple_graph("ema").add_data(this->ema.get_average());
		}
	}
	void updating() override {
		this->graph.update(this->event);
	}
	void drawing() override {
		this->draw(this->graph);
	}

	qsf::vgraph graph;
	qpl::exponential_moving_average ema;
};


int main() {
	qsf::framework framework;
	framework.set_title("Graph");
	framework.add_font("arial", "resources/arial.ttf");
	framework.set_dimension({ 1280u, 720u });

	framework.add_state<game_state>();
	framework.game_loop();
}
```


produces an interactive graph:

![possible output](https://i.imgur.com/gbDwKyY.png)
