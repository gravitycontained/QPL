#ifndef QPL_NEURALNET_HPP
#define QPL_NEURALNET_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <vector>
#include <array>
#include <string>
#include <qpl/vardef.hpp>
#include <qpl/random.hpp>
#include <qpl/string.hpp>

namespace qpl {
	class neuron;
	class neural_connection {
	public:
		neural_connection(qpl::f64 weight = 0.0) {
			this->init(weight);
		}
		QPLDLL void init(qpl::f64 weight);
		QPLDLL void set_weight(qpl::f64 weight);
		QPLDLL qpl::f64 get_weight() const;
		QPLDLL void set_delta_weight(qpl::f64 dweight);
		QPLDLL qpl::f64 get_delta_weight() const;
		QPLDLL void add_weight(qpl::f64 deltaweight);

		friend class neuron;
	private:
		qpl::f64 m_weight = 0.0;
		qpl::f64 m_delta_weight = 0.0;
	};

	class neuron {
	public:
		neuron() {

		}
		QPLDLL void set_index(qpl::u32 i);
		QPLDLL qpl::u32 get_index() const;
		QPLDLL void set_number_of_connections(qpl::u32 n);
		QPLDLL qpl::size get_number_of_connections() const;
		QPLDLL void set_output(qpl::f64 val);
		QPLDLL qpl::f64 get_output() const;
		QPLDLL qpl::f64 get_weighted_output(int index) const;
		QPLDLL neural_connection& get_connection(qpl::u32 i);
		QPLDLL const neural_connection& get_connection(qpl::u32 i) const;

		QPLDLL static qpl::f64 transfer_func(qpl::f64 n);
		QPLDLL static qpl::f64 transfer_func_derivative(qpl::f64 n);

		QPLDLL void calculate_output(const std::vector<neuron>& prev_layer);
		QPLDLL void show_expected_output(qpl::f64 excpeced_output);
		QPLDLL void learn_from_next_layer(const std::vector<neuron>& next_layer);
		QPLDLL void teach_previous_layer(std::vector<neuron>& previous_layer, double eta, double alpha);

		QPLDLL void load_from_stream(std::stringstream& stream);
		QPLDLL std::string info_string() const;
	private:
		qpl::f64 m_gradient = 0.0;
		qpl::f64 m_output = 0.0;
		qpl::u32 m_index = 0u;
		std::vector<neural_connection> m_connections;
	};

	class neural_net {
	public:
		neural_net() {
			this->init();
		}

		QPLDLL void set_topology(const std::vector<qpl::u32>& topology, bool create = true);

		QPLDLL qpl::size get_number_of_neurons() const;
		QPLDLL qpl::size get_number_of_connections() const;

		QPLDLL std::vector<qpl::u32> get_topology() const;
		QPLDLL bool created() const;
		QPLDLL void create();
		QPLDLL qpl::size feed_size() const;
		QPLDLL qpl::size out_size() const;
		QPLDLL void feed(const std::vector<qpl::f64>& input);
		template<typename args>
		void feed(qpl::f64 input, const args& pack...) {
			this->feed(std::vector<qpl::f64>({ input, pack }));
		}
		QPLDLL void get_output(std::vector<qpl::f64>& dest);
		QPLDLL std::vector<qpl::f64> get_output();

		QPLDLL void teach(const std::vector<qpl::f64>& excpected_outputs);
		template<typename args>
		void teach(qpl::f64 input, const args& pack...) {
			this->teach(std::vector<qpl::f64>({ input, pack }));
		}

		QPLDLL void full_cycle(const std::vector<qpl::f64>& input, std::vector<qpl::f64>& neural_net_outputs, const std::vector<qpl::f64>& excpected_outputs);

		QPLDLL qpl::f64 get_error() const;
		QPLDLL qpl::f64 get_accuracy() const;
		QPLDLL qpl::f64 get_average_accuracy() const;
		QPLDLL qpl::size get_generation_count() const;

		QPLDLL std::vector<neuron>& operator[](qpl::size index);
		QPLDLL const std::vector<neuron>& operator[](qpl::size index) const;
		QPLDLL std::vector<neuron>& at(qpl::size index);
		QPLDLL const std::vector<neuron>& at(qpl::size index) const;
		QPLDLL qpl::size size() const;

		QPLDLL void load_from_string(const std::string& info);
		QPLDLL std::string info_string() const;


		qpl::f64 eta = 0.15;
		qpl::f64 alpha = 0.5;

	private:
		QPLDLL void init();

		std::vector<neuron>& input_layer();
		std::vector<neuron>& output_layer();

		std::vector<std::vector<neuron>> m_neurons;
		qpl::f64 m_error;
		qpl::f64 m_accuracy_sum;
		bool m_neurons_created;
		qpl::size m_generation_counter;
	};

	namespace NN1 {
		class neural_connection {
		public:
			neural_connection(double weight = 0.0) {
				this->init(weight);
			}
			QPLDLL void init(double weight);
			QPLDLL void set_weight(double weight);
			QPLDLL double get_weight() const;
			QPLDLL void set_delta_weight(double dweight);
			QPLDLL double get_delta_weight() const;
			QPLDLL void add_weight(double deltaweight);
		private:
			double m_weight;
			double m_delta_weight;
		};

		class neuron {
		public:
			neuron() {

			}
			QPLDLL void set_index(unsigned i);
			QPLDLL unsigned get_index() const;
			QPLDLL void set_number_of_connections(unsigned n);
			QPLDLL std::size_t get_number_of_connections() const;
			QPLDLL void set_output(double val);
			QPLDLL double get_output() const;
			QPLDLL double get_weighted_output(int index) const;
			QPLDLL neural_connection& get_connection(unsigned i);
			QPLDLL const neural_connection& get_connection(unsigned i) const;

			QPLDLL static double transfer_func(double n);
			QPLDLL static double transfer_func_derivative(double n);

			QPLDLL void calculate_output(const std::vector<neuron>& prev_layer);
			QPLDLL void show_excpected_output(double excpeced_output);
			QPLDLL void learn_from_next_layer(const std::vector<neuron>& next_layer);
			QPLDLL void teach_previous_layer(std::vector<neuron>& previous_layer, qpl::f64 eta, qpl::f64 alpha);
		private:

			double m_gradient = 0.0;
			double m_output = 0.0;
			unsigned m_index = 0u;
			std::vector<neural_connection> m_connections;
		};


		class neural_net {
		public:
			neural_net() {
				this->init();
			}

			QPLDLL void set_topology(const std::vector<unsigned>& topology);

			QPLDLL std::size_t get_number_of_neurons() const;
			QPLDLL std::size_t get_number_of_connections() const;

			QPLDLL std::vector<unsigned> get_topology() const;
			QPLDLL bool created() const;
			QPLDLL void create();
			QPLDLL void feed(const std::vector<double>& input);
			QPLDLL void feed(double input);
			template<typename args>
			void feed(double input, const args& pack...) {
				this->feed(std::vector<double>({ input, pack }));
			}
			QPLDLL void get_output(std::vector<double>& dest);

			QPLDLL void teach(const std::vector<double>& excpected_outputs);
			QPLDLL void teach(double input);
			template<typename args>
			void teach(double input, const args& pack...) {
				this->teach(std::vector<double>({ input, pack }));
			}

			QPLDLL double get_error() const;
			QPLDLL double get_accuracy() const;
			QPLDLL double get_average_accuracy() const;
			QPLDLL std::size_t get_generation_count() const;

			QPLDLL std::vector<neuron>& operator[](unsigned index);
			QPLDLL const std::vector<neuron>& operator[](unsigned index) const;
			QPLDLL std::vector<neuron>& at(unsigned index);
			QPLDLL const std::vector<neuron>& at(unsigned index) const;
			QPLDLL std::size_t size() const;

			qpl::f64 eta = 0.15;
			qpl::f64 alpha = 0.5;
		private:
			QPLDLL void init();

			std::vector<neuron>& input_layer();
			std::vector<neuron>& output_layer();

			std::vector<std::vector<neuron>> m_neurons;
			double m_error;
			bool m_neurons_created;
			std::size_t m_generation_counter;
			qpl::f64 accuracy_sum = 0.0;
		};
	}

	namespace NN2 {
		qpl::f64 activation_function(qpl::f64 n);
		qpl::f64 normal_distribution();

		struct synapse {
			qpl::f64 weight;
			qpl::f64 delta_weight;
		};

		struct neuron {
			qpl::f64 gradient;
			qpl::f64 output;
			qpl::f64 bias;
			qpl::f64 momentum = 0.0;

			std::vector<synapse> synapses;
		};

		struct layer {
			std::vector<neuron> neurons;


			QPLDLL void calculate_gradients(layer& previous_layer);
			QPLDLL void calculate_weights(const layer& previous_layer, qpl::f64 eta, qpl::f64 alpha);
		};

		struct neural_net {
			std::vector<qpl::u32> topology;
			std::vector<layer> layers;
			qpl::f64 eta; 
			qpl::f64 alpha; 
			qpl::size generation_ctr = 0u;
			qpl::f64 accuracy = 0.0;
			qpl::f64 accuracy_sum = 0.0;
			
			QPLDLL qpl::f64 get_accuracy() const;
			QPLDLL qpl::f64 get_average_accuracy() const;

			QPLDLL std::vector<qpl::u32> get_topology() const;
			QPLDLL void set_topology(const std::vector<qpl::u32>& topology);
			QPLDLL void randomize_weights_and_biases();

			QPLDLL void observe_test() const;
			QPLDLL qpl::size generation_count() const;

			QPLDLL layer& input_layer();
			QPLDLL layer& output_layer();

			QPLDLL void show_expected_output(const std::vector<qpl::f64>& expected_output);

			QPLDLL void feed(const std::vector<qpl::f64>& input);
			QPLDLL void get_output(std::vector<qpl::f64>& destination);
			QPLDLL void teach(const std::vector<qpl::f64>& expected_output);
		};
	}


	namespace NN3 {
		qpl::f64 activation_function(qpl::f64 n);
		qpl::f64 normal_distribution();

		struct synapse {
			qpl::f64 weight;
			qpl::f64 delta_weight;
		};

		struct neuron {
			qpl::f64 gradient;
			qpl::f64 output;
			qpl::f64 bias;
			qpl::f64 momentum = 0.0;

			std::vector<synapse> synapses;
		};

		struct layer {
			std::vector<neuron> neurons;


			QPLDLL void calculate_gradients(layer& previous_layer);
			QPLDLL void calculate_weights(const layer& previous_layer, qpl::f64 eta, qpl::f64 alpha);
		};

		struct neural_net {
			std::vector<qpl::u32> topology;
			std::vector<layer> layers;
			qpl::f64 eta;
			qpl::f64 alpha;
			qpl::size generation_ctr = 0u;
			qpl::f64 accuracy = 0.0;
			qpl::f64 accuracy_sum = 0.0;

			QPLDLL qpl::f64 get_accuracy() const;
			QPLDLL qpl::f64 get_average_accuracy() const;

			QPLDLL std::vector<qpl::u32> get_topology() const;
			QPLDLL void set_topology(const std::vector<qpl::u32>& topology);
			QPLDLL void randomize_weights_and_biases();

			QPLDLL void observe_test() const;
			QPLDLL qpl::size generation_count() const;

			QPLDLL layer& input_layer();
			QPLDLL layer& output_layer();

			QPLDLL void show_expected_output(const std::vector<qpl::f64>& expected_output);

			QPLDLL void feed(const std::vector<qpl::f64>& input);
			QPLDLL void get_output(std::vector<qpl::f64>& destination);
			QPLDLL void teach(const std::vector<qpl::f64>& expected_output);
		};
	}

	namespace NNT {
		template<typename T>
		T activation_function(T n) {
			return 1.0 / (1.0 + std::pow(qpl::e, -n));
		}
		template<typename T>
		T normal_distribution() {
			return std::sin(2.0 * qpl::pi * qpl::random(std::numeric_limits<double>::epsilon(), 1.0)) * std::sqrt(-2.0 * std::log(qpl::random(std::numeric_limits<double>::epsilon(), 1.0)));
		}
		template<typename T>
		struct synapse {
			T weight;
			T delta_weight;
		};

		template<typename T>
		struct neuron {
			T gradient;
			T output;
			T bias;
			T momentum = 0.0;

			std::vector<synapse<T>> synapses;
		};

		template<typename T>
		struct layer {
			std::vector<neuron<T>> neurons;


			void calculate_gradients(layer& previous_layer) {
				for (auto& neuron : previous_layer.neurons) {
					neuron.gradient = 0.0;
				}
				for (qpl::u32 n = 0u; n < this->neurons.size(); ++n) {
					for (qpl::u32 s = 0u; s < this->neurons[n].synapses.size(); ++s) {
						previous_layer.neurons[s].gradient += this->neurons[n].gradient * this->neurons[n].synapses[s].weight;
					}
				}
			}
			void calculate_weights(const layer& previous_layer, T eta, T alpha) {
				for (qpl::u32 n = 0u; n < this->neurons.size(); ++n) {
					for (qpl::u32 s = 0u; s < this->neurons[n].synapses.size(); ++s) {
						this->neurons[n].synapses[s].delta_weight = this->neurons[n].gradient * previous_layer.neurons[s].output + this->neurons[n].synapses[s].delta_weight * alpha;
						this->neurons[n].synapses[s].weight += eta * this->neurons[n].synapses[s].delta_weight;

						//todo: civ if it's bad to include momentum in delta_weight
					}
					this->neurons[n].momentum = this->neurons[n].gradient + this->neurons[n].momentum * alpha;
					this->neurons[n].bias += eta * this->neurons[n].gradient;
				}
			}
		};

		template<typename T>
		struct neural_net {
			std::vector<qpl::u32> topology;
			std::vector<layer<T>> layers;
			T eta;
			T alpha;
			qpl::size generation_ctr = 0u;
			T accuracy = 0.0;
			T accuracy_sum = 0.0;

			T get_accuracy() const {
				return this->accuracy;
			}
			T get_average_accuracy() const {
				return this->accuracy_sum / this->generation_ctr;
			}

			std::vector<qpl::u32> get_topology() const {
				return this->topology;
			}
			void set_topology(const std::vector<qpl::u32>& topology) {
				this->topology = topology;
				this->layers.resize(topology.size());

				for (qpl::u32 l = 0u; l < this->layers.size(); ++l) {
					this->layers[l].neurons.resize(topology[l]);
				}

				this->randomize_weights_and_biases();
			}
			void randomize_weights_and_biases() {
				this->accuracy_sum = 0.0;
				this->generation_ctr = 0u;


				for (qpl::u32 l = 1u; l < this->layers.size(); ++l) {

					for (auto& neuron : this->layers[l].neurons) {
						neuron.gradient = neuron.output = 0.0;
						neuron.bias = qpl::NN2::normal_distribution() * std::sqrt(1.0 / (this->layers[l - 1].neurons.size() + 1));
						neuron.bias *= 0.2;

						//neuron.bias = qpl::random(-1.0, 1.0);

						neuron.synapses.resize(this->layers[l - 1].neurons.size());

						for (qpl::u32 s = 0u; s < neuron.synapses.size(); ++s) {
							neuron.synapses[s].delta_weight = 0.0;
							//neuron.synapses[s].weight = qpl::random(-1.0, 1.0);
							neuron.synapses[s].weight = qpl::NN2::normal_distribution() * std::sqrt(1.0 / (this->layers[l - 1].neurons.size() + 1));

						}
					}
				}


			}

			void observe_test() const {
				qpl::size l_ctr = 0u;
				for (auto& l : this->layers) {
					qpl::println_repeat("- ", 40);
					qpl::println("<layer ", l_ctr++, ">");
					qpl::size n_ctr = 0u;
					for (auto& n : l.neurons) {
						qpl::print("\n<neuron ", n_ctr++, "> ");
						qpl::println("output = ", n.output, ", gradient = ", n.gradient, ", bias = ", n.bias);
						qpl::size s_ctr = 0u;
						for (auto& s : n.synapses) {
							qpl::print("<synapse ", s_ctr++, "> ");
							qpl::println("weight = ", s.weight, ", delta_weight = ", s.delta_weight);
						}
					}
				}
			}
			qpl::size generation_count() const {
				return this->generation_ctr;
			}

			layer<T>& input_layer() {
				return this->layers.front();
			}
			layer<T>& output_layer() {
				return this->layers.back();
			}

			template<typename T>
			void show_expected_output(const std::vector<T>& expected_output) {
				for (qpl::u32 n = 0u; n < this->output_layer().neurons.size(); ++n) {
					auto momentum = this->output_layer().neurons[n].gradient * alpha;
					this->output_layer().neurons[n].gradient = expected_output[n] - this->output_layer().neurons[n].output + momentum;

				}
			}

			template<typename T>
			void feed(const std::vector<T>& input) {
				//set input
				for (qpl::u32 n = 0u; n < this->input_layer().neurons.size(); ++n) {
					this->input_layer().neurons[n].output = input[n];

				}

				//feed forward
				for (qpl::u32 l = 1u; l < this->layers.size(); ++l) {
					for (qpl::u32 n = 0u; n < this->layers[l].neurons.size(); ++n) {
						T sum = 0.0;
						for (qpl::u32 s = 0u; s < this->layers[l].neurons[n].synapses.size(); ++s) {
							sum += (this->layers[l].neurons[n].synapses[s].weight * this->layers[l - 1].neurons[s].output);
						}
						this->layers[l].neurons[n].output = qpl::NNT::activation_function(sum + this->layers[l].neurons[n].bias);
					}
				}
			}
			template<typename T>
			void get_output(std::vector<T>& destination) {
				for (qpl::u32 n = 0u; n < this->output_layer().neurons.size(); ++n) {
					destination[n] = this->output_layer().neurons[n].output;
				}
			}
			template<typename T>
			void teach(const std::vector<T>& expected_output) {
				this->show_expected_output(expected_output);

				T error_sum = 0.0;
				for (auto& neuron : this->output_layer().neurons) {
					error_sum += neuron.gradient * neuron.gradient;
				}
				error_sum /= this->output_layer().neurons.size();
				error_sum = std::sqrt(error_sum);
				this->accuracy = (1 - error_sum);
				this->accuracy_sum += this->accuracy;
				++this->generation_ctr;

				for (qpl::u32 l = this->layers.size() - 1; l > 1u; --l) {
					this->layers[l].calculate_gradients(this->layers[l - 1]);
				}
				for (qpl::u32 l = this->layers.size() - 1; l > 0u; --l) {
					this->layers[l].calculate_weights(this->layers[l - 1], this->eta, this->alpha);
				}
			}
		};
	}


	struct static_neural_connection {
		QPLDLL std::string info_string() const;

		qpl::f64 weight = 0.0;
		qpl::f64 delta_weight = 0.0;
	};

	struct static_neuron {
		static_neuron() {

		}
		QPLDLL std::string info_string() const;

		static qpl::f64 eta;
		static qpl::f64 alpha;

		qpl::f64 gradient = 0.0;
		qpl::f64 output = 0.0;
	};

	namespace detail {
		template<qpl::size... N>
		constexpr auto neuron_count_impl() {
			return ((N + 1) + ...);
		}

		template<qpl::size A, qpl::size B, qpl::size... N>
		constexpr qpl::size connection_count_impl() {
			if constexpr (sizeof...(N) == 0) {
				return (A + 1) * (B + 1);
			}
			else {
				return (A + 1) * (B + 1) + connection_count_impl<B, N...>();
			}
		}

		template<qpl::size first, qpl::size... N>
		constexpr qpl::size input_size_impl() {
			return first;
		}
		template<qpl::size first, qpl::size... N>
		constexpr qpl::size output_size_impl() {
			if constexpr (sizeof...(N) == 0) {
				return first;
			}
			else {
				return output_size_impl<N...>();
			}
		}
	}


	template<qpl::size... N>
	struct static_neural_net {
		constexpr static qpl::size layer_count = sizeof...(N);
		constexpr static qpl::size neuron_count = detail::neuron_count_impl<N...>();
		constexpr static qpl::size connection_count = detail::connection_count_impl<N...>();
		constexpr static qpl::size input_size = detail::input_size_impl<N...>();
		constexpr static qpl::size output_size = detail::output_size_impl<N...>();

		static_neural_net() {
			this->create();
		}

		void create() {
			std::vector<qpl::size> init_list = { N... };
			auto neuron_sum = qpl::size{};
			auto connection_sum = qpl::size{};
			for (auto i = qpl::size{}; i < qpl::static_neural_net<N...>::layer_count; ++i) {

				this->topology[i] = init_list[i];
				this->neuron_ranges[i] = neuron_sum;
				this->connection_ranges[i] = connection_sum;


				neuron_sum += init_list[i] + 1;
				if (i != qpl::static_neural_net<N...>::layer_count - 1) {
					connection_sum += ((init_list[i + 1] + 1) * (init_list[i] + 1));
				}
			}
			this->neuron_ranges.back() = neuron_sum;
			this->connection_ranges.back() = connection_sum;

			for (auto& i : this->connections) {
				i.weight = qpl::random(-1.0, 1.0);
			}
			for (int i = 0; i < this->neuron_ranges.size() - 1; ++i) {
				this->neurons[this->neuron_ranges[i + 1] - 1].output = 1.0;
			}
		}

		void feed(const std::array<qpl::f64, input_size>& input) {
			for (qpl::size i = qpl::size{}; i < input_size; ++i) {
				this->neurons[i].output = input[i];
			}

			for (qpl::size i = qpl::size{ 1 }; i < layer_count; ++i) {
				for (qpl::size n = this->neuron_ranges[i]; n < this->neuron_ranges[i + 1] - 1; ++n) {
					qpl::f64 sum = 0.0;
					for (qpl::size p = this->neuron_ranges[i - 1]; p < this->neuron_ranges[i]; ++p) {
						auto connection_index = (this->connection_ranges[i - 1] + (p - this->neuron_ranges[i - 1]) * (this->topology[i] + 1)) + (n - this->neuron_ranges[i]);
						sum += this->neurons[p].output * this->connections[connection_index].weight;
					}
					//this->neurons[n].output = sum;
					this->neurons[n].output = neuron::transfer_func(sum);
				}
			}
		}
		void get_output(std::array<qpl::f64, output_size>& dest) {
			for (qpl::size i = 0; i < output_size; ++i) {
				dest[i] = this->neurons[this->neuron_ranges[layer_count - 1]].output;
			}
		}
		void teach(const std::array<qpl::f64, output_size>& expected_outputs) {

			++this->generation_counter;
			this->error = 0.0;

			for (qpl::size i = 0; i < output_size; ++i) {
				auto wrong = expected_outputs[i] - this->neurons[this->neuron_ranges[layer_count - 1] + i].output;
				this->error += wrong * wrong;
			}


			this->error /= output_size;
			this->error = std::sqrt(this->error);
			this->accuracy_sum += (1.0 - this->error);

			for (qpl::size i = 0; i < output_size; ++i) {
				auto output_index = this->neuron_ranges[layer_count - 1] + i;
				auto wrong = expected_outputs[i] - this->neurons[output_index].output;
				this->neurons[output_index].gradient = wrong * qpl::neuron::transfer_func_derivative(this->neurons[output_index].output);
			}
			for (qpl::size i = layer_count - 2; i > 0; --i) {
				for (qpl::size n = this->neuron_ranges[i]; n < this->neuron_ranges[i + 1]; ++n) {
					qpl::f64 sum = 0.0;
					for (qpl::size p = this->neuron_ranges[i + 1]; p < this->neuron_ranges[i + 2]; ++p) {
						auto connection_index = (this->connection_ranges[i] + (p - this->neuron_ranges[i + 1]) + (n - this->neuron_ranges[i]) * (this->topology[i + 1] + 1));
						sum += this->neurons[p].gradient * this->connections[connection_index].weight;
					}
					//this->neurons[n].gradient = sum;
					this->neurons[n].gradient = sum * neuron::transfer_func_derivative(this->neurons[n].output);
				}
			}
			for (qpl::size i = layer_count - 1; i > 0; --i) {
				for (qpl::size n = this->neuron_ranges[i]; n < this->neuron_ranges[i + 1] - 1; ++n) {
					for (qpl::size p = this->neuron_ranges[i - 1]; p < this->neuron_ranges[i]; ++p) {
						auto connection_index = (this->connection_ranges[i - 1] + (p - this->neuron_ranges[i - 1]) * (this->topology[i] + 1)) + (n - this->neuron_ranges[i]);


						qpl::f64 old_delta_weight = this->connections[connection_index].delta_weight;
						qpl::f64 individual_input = this->eta * this->neurons[p].output * this->neurons[n].gradient;
						qpl::f64 momentum = this->alpha * old_delta_weight;
						qpl::f64 new_delta_weight = individual_input + momentum;
						this->connections[connection_index].delta_weight = new_delta_weight;
						this->connections[connection_index].weight += new_delta_weight;
					}
				}
			}
		}


		qpl::f64 accuracy() const {
			return this->accuracy_sum / this->generation_counter;
		}
		qpl::f64 get_average_accuracy() const {
			return this->accuracy_sum / this->generation_counter;
		}

		std::string info_string() const {
			std::ostringstream stream;
			stream << qpl::container_to_string(this->topology);
			for (auto& i : this->neurons) {
				stream << i.info_string() << '\n';
			}
			stream << '\n';
			for (auto& i : this->connections) {
				stream << i.info_string() << '\n';
			}
			return stream.str();
		}

		std::array<qpl::static_neuron, neuron_count> neurons;
		std::array<qpl::static_neural_connection, connection_count> connections;
		std::array<qpl::size, layer_count + 1> neuron_ranges;
		std::array<qpl::size, layer_count> connection_ranges;
		std::array<qpl::size, layer_count> topology;

		qpl::f64 eta = 0.15;
		qpl::f64 alpha = 0.5;
		qpl::f64 error = 0.0;
		qpl::f64 accuracy_sum = 0.0;
		qpl::size generation_counter = 0;

	};
}


#endif
