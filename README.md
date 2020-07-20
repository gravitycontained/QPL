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

simple fast **random number generater**.
it uses a good setup: a global instance's constructor fills all bits of `std::mt19937` types via `std::random_device` and seed_sequences.

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

fixed precision `qpl::integer<BITS, SIGN>`:

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
