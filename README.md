# QPL 
QPL (Quick Production Library) is a (Windows) C++20 personal utility library with many purposes to help developement. Written by Daniel Rabl (danielrabl1999@gmail.com).

# Setup

to use this library download the library and select the **include** folder as your include folder and then write `#include <qpl/qpl.hpp>` in your project.
include the src foulder as source files into your project.

there are 3 defines, that, if written **before** `#include <qpl/qpl.hpp>` or defined as a preprocessor define will have an effect. `QPL_USE_ALL` to use all of below.

- `#define QPL_USE_INTRINSICS` enabled (Intel) intrinsics and let's you use the optimized `qpl::x64_integer` types.
- `#define QPL_USE_VULKAN` this will enable the Vulkan utilites. You have to include and link the [Vulkan](https://www.khronos.org/vulkan/) library, [glm](https://glm.g-truc.net/0.9.9/index.html), [GLFW](https://www.glfw.org/) and [stb_image](https://github.com/nothings/stb) in order to compile your program. (`glfw3.lib` `vulkan-1.lib` linker dependencies)
- `#define QPL_USE_SFML` this will enable the SFML utilies. You have to inlcude and link the [SFML](https://www.sfml-dev.org/) library in order to compile your program. (`sfml-graphics.lib` `sfml-window.lib` `sfml-main.lib` `sfml-system.lib` `sfml-audio.lib` `sfml-network.lib` linker dependencies. `-d` before `.lib` for debug files)


# Utilities

simple fast **random number generator**.
it uses a global instance's constructor to randomize all bits of `std::mt19937` types via `std::random_device` and seed_sequences.

```cpp
auto a = qpl::random(-5, 10);
auto b = qpl::random(0.1, 0.7);
auto c = qpl::random(qpl::f32_min, 0.7f);

static_assert(qpl::is_same<decltype(a), int>());
static_assert(qpl::is_same<decltype(b), double>());
static_assert(qpl::is_same<decltype(c), float>());
  ```
  
----------

**any precision number arithmetic**.

fixed precision signed / unsigned integer classes `qpl::integer<BITS, SIGN>`:

```cpp
qpl::i512 a = 0; //using qpl::i512 = qpl::integer<512, 1>;
qpl::i512 b = 0; 

a.randomize_bits_logarithmic();
b.randomize_bits_logarithmic();

qpl_vprintln(a);
qpl_vprintln(b);

auto add = a + b; 
auto sub = a - b; 
auto mul = a * b; 
auto div = a / b; 
auto mod = a % b; 

qpl_vprintln(add);
qpl_vprintln(sub);
qpl_vprintln(mul);
qpl_vprintln(div);
qpl_vprintln(mod);
```

possible output:
![possible output](https://i.imgur.com/XnJuSly.png)

(the multiplication overflowed here correctly, sign bit was overwritten)

----------

fixed precision floating_point class `qpl::floating_point<EXPONENT_BITS, MANTISSA_BITS>`:

```cpp
auto a = qpl::f512::random(0, 100); //using qpl::f512 = qpl::floating_point<32, 512>;
auto b = qpl::f512::random(0, 100);

qpl_vprintln(a);
qpl_vprintln(b);

auto add = a + b;
auto sub = a - b;
auto mul = a * b;
auto div = a / b;
auto pow = a ^ b;
auto sqta = a.sqrted();
auto sqtb = b.sqrted();
auto inva = a.inverted();
auto invb = b.inverted();

qpl_vprintln(add);
qpl_vprintln(sub);
qpl_vprintln(mul);
qpl_vprintln(div);
qpl_vprintln(pow);
qpl_vprintln(sqta);
qpl_vprintln(sqtb);
qpl_vprintln(inva);
qpl_vprintln(invb);
```

possible output:
![possible output](https://i.imgur.com/kPAGnUM.png)

----------

**neural nets**:

```cpp
qpl::neural_net net;
net.set_topology({ 2, 2, 1 });

std::vector<qpl::f64> input(2);
std::vector<qpl::f64> output(1);
std::vector<qpl::f64> net_output(1);

while (true) {

    input[0] = qpl::random(0.0, 0.5);
    input[1] = qpl::random(0.0, 0.5);
    output[0] = input[0] + input[1];

    net.feed(input);
    net.get_output(net_output);
    net.teach(output);

    if (qpl::get_time_signal(0.001)) {
        qpl::print(qpl::to_string_precision(5, input[0]), " + ");
        qpl::print(qpl::to_string_precision(5, input[1]), " = ");
        qpl::print(qpl::to_string_precision(5, net_output[0]));
        qpl::print(" ", qpl::str_spaced(net.get_error() * 100, 10), "% wrong - ");
        qpl::println(qpl::foreground::light_green, qpl::to_string_precision(5, net.get_average_accuracy() * 100), "% average accuracy");
    }
}
```

possible output:

![possible output](https://i.imgur.com/sUflTEL.png)

----------
    
**benchmarking**:

```cpp
template<typename T>
bool is_prime(T value) {
	qpl::begin_benchmark("is_prime", "< 5 check");
	if (value < 5u) {
		qpl::end_benchmark();
		return value == 2u || value == 3u;
	}
	qpl::end_benchmark();

	qpl::begin_benchmark("is_prime", "setup");
	qpl::u32 add = 2u;
	auto sqrt = std::sqrt(value);
	qpl::end_benchmark();


	for (qpl::u32 i = 5u; i < sqrt; i += add) {
		qpl::begin_benchmark("is_prime", "divisibility check");
		if (value % i == 0) {
			qpl::end_benchmark();
			return false;
		}
		qpl::end_benchmark();

		qpl::begin_benchmark("is_prime", "subtract");
		add = 6u - add;
		qpl::end_benchmark();
	}
	return true;
}


int main() {
	while (true) {
		auto n = qpl::random(1, 100'000);
		auto b = is_prime(n);

		if (qpl::get_time_signal(0.2)) {
			qpl::clear_console();
			qpl::print_benchmark();
		}
	}
}
```
possible output:

![possible output](https://i.imgur.com/r4Qen97.png)

Benchmarks shouldn't be used inside of eachother at critical code pieces, as they have their own execution cost.
They also could lead to branch prediction misses and therefore falsify the actual results.

----------

**2D - SFML example**: 
`QPL_USE_SFML` as preprocessor define

```cpp
#include <qpl/qpl.hpp>

struct game_state : qsf::base_state {

	void init() override {
		this->lines.thickness = 10;
	}
	void updating() override {
		if (this->event.left_mouse_clicked()) {
			auto pos = this->event.mouse_position();
			auto radius = qpl::random(20, 30);
			auto color = qsf::get_random_color();


			this->lines.add_thick_line(pos, color);
			this->circles.add_circle(pos, radius, color);
		}
	}
	void drawing() override {
		this->draw(this->lines);
		this->draw(this->circles);
	}

	qsf::vcircles circles;
	qsf::vthick_lines lines;
};


int main() {


	qsf::framework framework;
	framework.set_title("SFML lines & circles!");
	framework.set_dimension({ 1280u, 720u });

	framework.add_state<game_state>();
	framework.gameloop();
}
```

clicking on screen addds connected cirlcles. possible output:

![possible output](https://i.imgur.com/Yek7ojA.png)
