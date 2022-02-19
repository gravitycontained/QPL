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

---

# Main Utilities

there are many utilities, here I show main features.

**print to console**

`qpl::println` offers flexible ways to print to console:

```cpp
qpl::println("Hello World");

qpl::println("abc", 123, 'x', -500.0);

qpl::println(std::vector{ 1, 2, 3, 4 });

qpl::println(std::make_tuple("hello", 123, 'x', -500.0));
```

output:

```
Hello World
abc123x-500
{1, 2, 3, 4}
{hello, 123, x, -500}
```

**console input**

```cpp
qpl::print("enter text > ");
auto text = qpl::get_input();

qpl::print("enter number > ");
auto number = qpl::get_input<int>();

qpl::print("enter secret > ");
auto secret = qpl::get_hidden_input();

qpl::println();
qpl::println("text = ", text);
qpl::println("number = ", number);
qpl::println("secret = ", secret);
````

output:

```cpp
enter text > hello world
enter number > 123
enter secret > ****

text = hello world
number = 123
secret = cats
```

**fundamental typedefs:
```cpp
//char type:
qpl::char_type c = 'a';

//type size
qpl::size s = 100u;
static_assert(qpl::is_same<qpl::size, std::size_t>());

//8 bit ints:
qpl::i8 i8 = 'a'; 
qpl::u8 u8 = 'b'; 

//16 bit ints:
qpl::i16 i16 = -50;
qpl::u16 u16 = 50u;

//32 bit ints:
qpl::i32 i32 = -100'000'000;
qpl::u32 u32 = 100'000'000u;

//64 bit ints:
qpl::i64 i64 = -100'000'000'000;
qpl::u64 u64 = 100'000'000'000u;

//floats:
qpl::f32 f32 = 50.0f;
qpl::f64 f64 = 1e100;
```

**number type traits**

```cpp
auto type_info = []<typename T>(T n) {

	qpl::println("T = ", qpl::type_name<T>());
	qpl_vprintln(qpl::bits_in_type<T>());
	qpl_vprintln(qpl::bytes_in_type<T>());
	qpl_vprintln(qpl::is_arithmetic<T>());

	if constexpr (qpl::is_arithmetic<T>()) {

		qpl_vprintln(qpl::is_integer<T>());
		qpl_vprintln(qpl::is_floating_point<T>());
		qpl_vprintln(qpl::type_min<T>());
		qpl_vprintln(qpl::type_max<T>());

		if constexpr (qpl::is_integer<T>()) {

			qpl_vprintln(qpl::is_signed<T>());
			qpl_vprintln(qpl::is_unsigned<T>());
		}
	}

	qpl::println();
};

type_info(qpl::i8{ });
type_info(qpl::u32{ });
type_info(qpl::f64{ });
```

output:
```
T = signed char
qpl::bits_in_type<T>(): 8
qpl::bytes_in_type<T>(): 1
qpl::is_arithmetic<T>(): 1
qpl::is_integer<T>(): 1
qpl::is_floating_point<T>(): 0
qpl::type_min<T>(): Ç
qpl::type_max<T>(): ⌂
qpl::is_signed<T>(): 1
qpl::is_unsigned<T>(): 1

T = unsigned int
qpl::bits_in_type<T>(): 32
qpl::bytes_in_type<T>(): 4
qpl::is_arithmetic<T>(): 1
qpl::is_integer<T>(): 1
qpl::is_floating_point<T>(): 0
qpl::type_min<T>(): 0
qpl::type_max<T>(): 4294967295
qpl::is_signed<T>(): 0
qpl::is_unsigned<T>(): 1

T = double
qpl::bits_in_type<T>(): 64
qpl::bytes_in_type<T>(): 8
qpl::is_arithmetic<T>(): 1
qpl::is_integer<T>(): 0
qpl::is_floating_point<T>(): 1
qpl::type_min<T>(): 2.22507e-308
qpl::type_max<T>(): 1.79769e+308
```

**container type traits**
```cpp
auto type_info = []<typename T>(T n) {

	qpl::println("T = ", qpl::type_name<T>());
	qpl::println("is container:            ", qpl::bool_string(qpl::is_container<T>()));

	if constexpr (qpl::is_container<T>()) {

		qpl::println("container subtype name:  ", qpl::type_name<qpl::container_subtype<T>>());
		qpl::println("deepest subtype name:    ", qpl::type_name<qpl::container_deepest_subtype<T>>());
		qpl::println("is contiguous in memory: ", qpl::bool_string(qpl::is_contiguous_container<T>()));
		qpl::println("is salways sorted:       ", qpl::bool_string(qpl::is_sorted_container<T>()));
		qpl::println("is std container:        ", qpl::bool_string(qpl::is_std_container<T>()));


		if constexpr (qpl::is_std_container<T>()) {

			if constexpr (qpl::is_std_array_type<T>())              qpl::println("> it's an std::array");
			if constexpr (qpl::is_std_vector_type<T>())             qpl::println("> it's an std::vector");
			if constexpr (qpl::is_std_deque_type<T>())              qpl::println("> it's an std::deque");
			if constexpr (qpl::is_std_list_type<T>())               qpl::println("> it's an std::list");
			if constexpr (qpl::is_std_forward_list_type<T>())       qpl::println("> it's an std::forward_list");
			if constexpr (qpl::is_std_set_type<T>())                qpl::println("> it's an std::set");
			if constexpr (qpl::is_std_unordered_set_type<T>())      qpl::println("> it's an std::unordered_set");
			if constexpr (qpl::is_std_multiset_type<T>())           qpl::println("> it's an std::multiset");
			if constexpr (qpl::is_std_unordered_multiset_type<T>()) qpl::println("> it's an std::unordered_multiset");
			if constexpr (qpl::is_std_map_type<T>())                qpl::println("> it's an std::map");
			if constexpr (qpl::is_std_unordered_map_type<T>())      qpl::println("> it's an std::unordered_map");
			if constexpr (qpl::is_std_multimap_type<T>())           qpl::println("> it's an std::multimap");
			if constexpr (qpl::is_std_unordered_multimap_type<T>()) qpl::println("> it's an std::unordered_multimap");
		}

		if constexpr (qpl::has_size<T>())             qpl::println(" - has size");
		if constexpr (qpl::has_data<T>())             qpl::println(" - has data");
		if constexpr (qpl::has_at<T>())               qpl::println(" - has at");
		if constexpr (qpl::has_square_brackets<T>())  qpl::println(" - has []");
		if constexpr (qpl::has_resize<T>())           qpl::println(" - has resize");
		if constexpr (qpl::has_reserve<T>())          qpl::println(" - has reserve");
		if constexpr (qpl::has_push_back<T>())        qpl::println(" - has push_back");
		if constexpr (qpl::has_pop_back<T>())         qpl::println(" - has pop_back");
		if constexpr (qpl::has_push_front<T>())       qpl::println(" - has push_front");
		if constexpr (qpl::has_pop_front<T>())        qpl::println(" - has pop_front");
		if constexpr (qpl::has_insert<T>())           qpl::println(" - has insert");


	}

	qpl::println();
};

using V = qpl::f64;
using K = qpl::i32;

type_info(std::vector<V>{});
type_info(std::vector<std::vector<V>>{});
type_info(std::array<V, 4>{});
type_info(std::array<std::vector<V>, 4>{});
type_info(std::deque<V>{});
type_info(std::list<V>{});
type_info(std::set<V>{});
type_info(std::unordered_multimap<K, V>{});
```

output:
```
T = class std::vector<double,class std::allocator<double> >
is container:            true
container subtype name:  double
deepest subtype name:    double
is contiguous in memory: true
is always sorted:        false
is std container:        true
> it's an std::vector
 - has size
 - has data
 - has at
 - has []
 - has resize
 - has reserve
 - has push_back
 - has pop_back

T = class std::vector<class std::vector<double,class std::allocator<double> >,class std::allocator<class std::vector<double,class std::allocator<double> > > >
is container:            true
container subtype name:  class std::vector<double,class std::allocator<double> >
deepest subtype name:    double
is contiguous in memory: true
is always sorted:        false
is std container:        true
> it's an std::vector
 - has size
 - has data
 - has at
 - has []
 - has resize
 - has reserve
 - has push_back
 - has pop_back

T = class std::array<double,4>
is container:            true
container subtype name:  double
deepest subtype name:    double
is contiguous in memory: true
is always sorted:        false
is std container:        true
> it's an std::array
 - has size
 - has data
 - has at
 - has []

T = class std::array<class std::vector<double,class std::allocator<double> >,4>
is container:            true
container subtype name:  class std::vector<double,class std::allocator<double> >
deepest subtype name:    double
is contiguous in memory: true
is always sorted:        false
is std container:        true
> it's an std::array
 - has size
 - has data
 - has at
 - has []

T = class std::deque<double,class std::allocator<double> >
is container:            true
container subtype name:  double
deepest subtype name:    double
is contiguous in memory: false
is always sorted:        false
is std container:        true
> it's an std::deque
 - has size
 - has at
 - has []
 - has resize
 - has push_back
 - has pop_back
 - has push_front
 - has pop_front

T = class std::list<double,class std::allocator<double> >
is container:            true
container subtype name:  double
deepest subtype name:    double
is contiguous in memory: false
is always sorted:        false
is std container:        true
> it's an std::list
 - has size
 - has resize
 - has push_back
 - has pop_back
 - has push_front
 - has pop_front

T = class std::set<double,struct std::less<double>,class std::allocator<double> >
is container:            true
container subtype name:  double
deepest subtype name:    double
is contiguous in memory: false
is always sorted:        true
is std container:        true
> it's an std::set
 - has size
 - has insert

T = class std::unordered_multimap<int,double,struct std::hash<int>,struct std::equal_to<int>,class std::allocator<struct std::pair<int const ,double> > >
is container:            true
container subtype name:  struct std::pair<int const ,double>
deepest subtype name:    struct std::pair<int const ,double>
is contiguous in memory: false
is always sorted:        false
is std container:        true
> it's an std::unordered_multimap
 - has size
 - has reserve
 - has insert
```

**cast to string**

`qpl::to_string` is as flexible as `qpl::println` and returns the resulting `std::string`:

```cpp
qpl::println(qpl::to_string("Hello World"));
qpl::println(qpl::to_string("abc", 123, 'x', -500.0));
qpl::println(qpl::to_string(std::vector{ 1, 2, 3, 4 }));
qpl::println(qpl::to_string(std::make_tuple("hello", 123, 'x', -500.0)));
```

**cast from string**



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
