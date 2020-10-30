#include <qpl/neural_net.hpp>

#include <qpl/algorithm.hpp>
#include <qpl/random.hpp>
#include <sstream>
#include <charconv>
#include <iostream>

namespace qpl {
	void neural_connection::init(qpl::f64 weight) {
		this->m_weight = weight;
		this->m_delta_weight = 0.0;
	}
	void neural_connection::set_weight(qpl::f64 weight) {
		this->m_weight = weight;
	}
	qpl::f64 neural_connection::get_weight() const {
		return this->m_weight;
	}
	void neural_connection::set_delta_weight(qpl::f64 dweight) {
		this->m_delta_weight = dweight;
	}
	qpl::f64 neural_connection::get_delta_weight() const {
		return this->m_delta_weight;
	}
	void neural_connection::add_weight(qpl::f64 deltaweight) {
		this->m_weight += deltaweight;
	}

	void neuron::set_index(qpl::u32 i) {
		this->m_index = i;
	}
	qpl::u32 neuron::get_index() const {
		return this->m_index;
	}
	void neuron::set_number_of_connections(qpl::u32 n) {
		this->m_connections.resize(n);
	}
	std::size_t neuron::get_number_of_connections() const {
		return this->m_connections.size();
	}
	void neuron::set_output(qpl::f64 val) {
		this->m_output = val;
	}
	qpl::f64 neuron::get_output() const {
		return this->m_output;
	}
	qpl::f64 neuron::get_weighted_output(int index) const {
		return this->m_output * this->m_connections[index].get_weight();
	}
	neural_connection& neuron::get_connection(qpl::u32 i) {
		return this->m_connections[i];
	}
	const neural_connection& neuron::get_connection(qpl::u32 i) const {
		return this->m_connections[i];
	}

	qpl::f64 neuron::transfer_func(qpl::f64 n) {
		//return (n < 0 ? 0.01 : n);
		return std::tanh(n);
	}
	qpl::f64 neuron::transfer_func_derivative(qpl::f64 n) {
		return 1.0 - n * n;
	}

	void neuron::calculate_output(const std::vector<neuron>& prev_layer) {
		qpl::f64 sum = 0.0;
		for (std::size_t i = 0; i < prev_layer.size(); ++i) {
			sum += prev_layer[i].get_weighted_output(this->m_index);
		}
		this->m_output = neuron::transfer_func(sum);
	}

	void neuron::show_expected_output(qpl::f64 excpeced_output) {
		qpl::f64 wrong = excpeced_output - this->m_output;
		//this->m_gradient = wrong;
		this->m_gradient = wrong * neuron::transfer_func_derivative(this->m_output); //für XOR! böse zeile
	}

	void neuron::learn_from_next_layer(const std::vector<neuron>& next_layer) {
		qpl::f64 sum = 0.0;
		for (std::size_t i = 0; i < next_layer.size() - 1; ++i) {
			sum += this->m_connections[i].get_weight() * next_layer[i].m_gradient;
		}
		//this->m_gradient = sum;
		this->m_gradient = sum * neuron::transfer_func_derivative(this->m_output); //für XOR! böse zeile
	}
	void neuron::teach_previous_layer(std::vector<neuron>& previous_layer, double eta, double alpha) {
		for (std::size_t i = 0; i < previous_layer.size(); ++i) {
			qpl::f64 old_delta_weight = previous_layer[i].m_connections[this->m_index].get_delta_weight();
			qpl::f64 individual_input = eta * previous_layer[i].m_output * this->m_gradient;
			qpl::f64 momentum = alpha * old_delta_weight;
			qpl::f64 new_delta_weight = individual_input + momentum;

			previous_layer[i].m_connections[this->m_index].set_delta_weight(new_delta_weight);
			previous_layer[i].m_connections[this->m_index].add_weight(new_delta_weight);
		}
	}
	void neuron::load_from_stream(std::stringstream& stream) {
		std::string line;
		std::getline(stream, line);

		std::array<qpl::u8, 4> result;
		unsigned ctr = 0u;
		for (int i = 0; i < line.size(); ++i) {
			if (line[i] == ' ') {
				result[ctr++] = i;
			}
		}
		
		qpl::size size;
		std::from_chars(line.data(), line.data() + result[0], this->m_index);
		std::from_chars(line.data() + result[1], line.data() + result[2], this->m_gradient);
		std::from_chars(line.data() + result[2], line.data() + result[3], this->m_output);
		std::from_chars(line.data() + result[3], line.data() + line.size(), size);

		this->m_connections.resize(size);

		for (auto& i : this->m_connections) {
			std::getline(stream, line);
			qpl::u8 result = 0u;		
			for (int i = 0; i < line.size(); ++i) {
				if (line[i] == ' ') {
					result = i;
					break;
				}
			}

			std::from_chars(line.data(), line.data() + result, i.m_weight);
			std::from_chars(line.data(), line.data() + result, i.m_delta_weight);
		}
	}
	std::string neuron::info_string() const {
		std::ostringstream stream;
		
		stream << this->m_index << ' ' << this->m_gradient << ' ' << this->m_output << ' '  << this->m_connections.size() << '\n';
		for (const auto& i : this->m_connections) {
			stream << i.get_weight() << ' ' << i.get_delta_weight() << '\n';
		}
		return stream.str();
	}



	void neural_net::init() {
		this->m_neurons_created = false;
		this->m_generation_counter = 0u;
		this->m_accuracy_sum = 0.0;
		this->m_error = 0.0;
	}
	void neural_net::set_topology(const std::vector<qpl::u32>& topology, bool create) {
		this->m_neurons.resize(topology.size());
		for (std::size_t i = 0; i < this->m_neurons.size(); ++i) {
			this->m_neurons[i].resize(topology[i] + 1);
		}
		if (create) {
			this->create();
		}
	}

	std::size_t neural_net::get_number_of_neurons() const {
		auto top = this->get_topology();
		auto sum = qpl::u32{};
		for (auto i : top) {
			sum += i;
		}
		return sum;
	}
	std::size_t neural_net::get_number_of_connections() const {
		std::size_t sum = 0u;
		for (std::size_t i = 0; i < this->m_neurons.size(); ++i) {
			for (std::size_t j = 0; j < this->m_neurons[i].size(); ++j) {
				sum += this->m_neurons[i][j].get_number_of_connections();
			}
		}
		return sum;
	}

	std::vector<qpl::u32> neural_net::get_topology() const {
		std::vector<qpl::u32> result(this->m_neurons.size());
		for (std::size_t i = 0u; i < result.size(); ++i) {
			result[i] = static_cast<qpl::u32>(m_neurons[i].size() - 1);
		}
		return result;
	}
	bool neural_net::created() const {
		return this->m_neurons_created;
	}
	void neural_net::create() {
		for (qpl::u32 i = 0; i < this->m_neurons.size(); ++i) {
			auto connection_count = static_cast<int>(i == this->m_neurons.size() - 1 ? 0 : this->m_neurons[i + 1].size() - 1);
			for (qpl::u32 j = 0; j < this->m_neurons[i].size(); ++j) {
				this->m_neurons[i][j].set_index(j);
				this->m_neurons[i][j].set_number_of_connections(connection_count);
				for (int c = 0; c < connection_count; ++c) {
					this->m_neurons[i][j].get_connection(c).set_weight(qpl::random(-1.0, 1.0));
					//this->m_neurons[i][j].get_connection(c).set_weight(qpl::random(-1.0, 1.0) * std::sqrt(1 / this->m_neurons[i].size()));
					//this->m_neurons[i][j].get_connection(c).set_weight(qpl::random(0.0, this->input_layer().size() - 1.0) * std::sqrt(2 / this->input_layer().size()));
				}
			}
			this->m_neurons[i].back().set_output(1.0); //bias
		}
		this->m_neurons_created = true;
	}
	qpl::size neural_net::feed_size() const {
		return this->m_neurons.front().size() - 1;
	}
	qpl::size neural_net::out_size() const {
		return this->m_neurons.back().size() - 1;
	}
	void neural_net::feed(const std::vector<qpl::f64>& input) {
		for (std::size_t i = 0; i < input.size(); ++i) {
			this->input_layer()[i].set_output(input[i]);
		}
		for (std::size_t i = 1; i < this->m_neurons.size(); ++i) {
			auto& prev_layer = this->m_neurons[i - 1];
			for (std::size_t j = 0; j < this->m_neurons[i].size() - 1; ++j) {
				this->m_neurons[i][j].calculate_output(prev_layer);
			}
		}
	}
	void neural_net::get_output(std::vector<qpl::f64>& dest) {
		dest.resize(this->output_layer().size() - 1);
		for (std::size_t i = 0; i < dest.size(); ++i) {
			dest[i] = this->output_layer()[i].get_output();
		}
	}
	std::vector<qpl::f64> neural_net::get_output() {
		std::vector<qpl::f64> result(this->output_layer().size() - 1);
		for (std::size_t i = 0; i < result.size(); ++i) {
			result[i] = this->output_layer()[i].get_output();
		}
		return result;
	}

	void neural_net::teach(const std::vector<qpl::f64>& excpected_outputs) {
		++this->m_generation_counter;
		this->m_error = 0.0;
		for (std::size_t i = 0; i < this->output_layer().size() - 1; ++i) {
			qpl::f64 wrong = excpected_outputs[i] - this->output_layer()[i].get_output();
			this->m_error += wrong * wrong;
		}
		this->m_error /= (this->output_layer().size() - 1);
		this->m_error = std::sqrt(this->m_error);
		this->m_accuracy_sum += this->get_accuracy();

		for (std::size_t i = 0; i < this->output_layer().size() - 1; ++i) {
			this->output_layer()[i].show_expected_output(excpected_outputs[i]); //answers[n] - output
		}
		for (std::size_t i = this->m_neurons.size() - 2; i > 0; --i) {
			std::vector<neuron>& next_layer = this->m_neurons[i + 1];
			for (std::size_t j = 0; j < this->m_neurons[i].size(); ++j) {
				this->m_neurons[i][j].learn_from_next_layer(next_layer); //gradiants
			}
		}
		for (std::size_t i = this->m_neurons.size() - 1; i > 0; --i) {
			std::vector<neuron>& prev_layer = this->m_neurons[i - 1];
			for (std::size_t j = 0u; j < m_neurons[i].size() - 1; ++j) {
				this->m_neurons[i][j].teach_previous_layer(prev_layer, this->eta, this->alpha); //delta_weigt
			}
		}
	}
	void neural_net::full_cycle(const std::vector<qpl::f64>& input, std::vector<qpl::f64>& outputs, const std::vector<qpl::f64>& excpected_outputs) {
		this->feed(input);
		this->get_output(outputs);
		this->teach(excpected_outputs);
	}
	qpl::f64 neural_net::get_error() const {
		return this->m_error;
	}
	qpl::f64 neural_net::get_accuracy() const {
		return 1.0 - this->m_error;
	}
	qpl::f64 neural_net::get_average_accuracy() const {
		return this->m_accuracy_sum / this->m_generation_counter;
	}
	std::size_t neural_net::get_generation_count() const {
		return this->m_generation_counter;
	}

	std::vector<neuron>& neural_net::operator[](qpl::u32 index) {
		return this->m_neurons[index];
	}
	const std::vector<neuron>& neural_net::operator[](qpl::u32 index) const {
		return this->m_neurons[index];
	}
	std::vector<neuron>& neural_net::at(qpl::u32 index) {
		return this->m_neurons.at(index);
	}
	const std::vector<neuron>& neural_net::at(qpl::u32 index) const {
		return this->m_neurons.at(index);
	}
	std::size_t neural_net::size() const {
		return this->m_neurons.size();
	}

	void neural_net::load_from_string(const std::string& info) {
		std::stringstream stream;
		stream << info;

		this->m_neurons.clear();

		std::string line;
		while (true) {
			qpl::size size;
			std::getline(stream, line);
			std::from_chars(line.data(), line.data() + line.size(), size);

			this->m_neurons.resize(size);

			for (auto& layer : this->m_neurons) {
				std::getline(stream, line);
				std::from_chars(line.data(), line.data() + line.size(), size);
				layer.resize(size);

				for (auto& neuron : layer) {
					neuron.load_from_stream(stream);
				}
			}
		}
		this->m_neurons_created = true;
	}
	std::string neural_net::info_string() const {
		std::ostringstream stream;

		stream << this->m_neurons.size() << '\n';
		for (const auto& layer : this->m_neurons) {
			stream << layer.size() << '\n';
			for (const auto& neuron : layer) {
				stream << neuron.info_string();
			}
		}
		return stream.str();
	}
	std::vector<neuron>& neural_net::input_layer() {
		return this->m_neurons[0];
	}
	std::vector<neuron>& neural_net::output_layer() {
		return this->m_neurons.back();
	}

	void qpl::NN1::neural_connection::init(double weight) {
		this->m_weight = weight;
		this->m_delta_weight = 0.0;
	}
	void qpl::NN1::neural_connection::set_weight(double weight) {
		this->m_weight = weight;
	}
	double qpl::NN1::neural_connection::get_weight() const {
		return this->m_weight;
	}
	void qpl::NN1::neural_connection::set_delta_weight(double dweight) {
		this->m_delta_weight = dweight;
	}
	double qpl::NN1::neural_connection::get_delta_weight() const {
		return this->m_delta_weight;
	}
	void qpl::NN1::neural_connection::add_weight(double deltaweight) {
		this->m_weight += deltaweight;
	}

	void qpl::NN1::neuron::set_index(unsigned i) {
		this->m_index = i;
	}
	unsigned qpl::NN1::neuron::get_index() const {
		return this->m_index;
	}
	void qpl::NN1::neuron::set_number_of_connections(unsigned n) {
		this->m_connections.resize(n);
	}
	std::size_t qpl::NN1::neuron::get_number_of_connections() const {
		return this->m_connections.size();
	}
	void qpl::NN1::neuron::set_output(double val) {
		this->m_output = val;
	}
	double qpl::NN1::neuron::get_output() const {
		return this->m_output;
	}
	double qpl::NN1::neuron::get_weighted_output(int index) const {
		return this->m_output * this->m_connections[index].get_weight();
	}
	qpl::NN1::neural_connection& qpl::NN1::neuron::get_connection(unsigned i) {
		return this->m_connections[i];
	}
	const qpl::NN1::neural_connection& qpl::NN1::neuron::get_connection(unsigned i) const {
		return this->m_connections[i];
	}

	double qpl::NN1::neuron::transfer_func(double n) {
		return std::tanh(n);
	}
	double qpl::NN1::neuron::transfer_func_derivative(double n) {
		return 1.0 - n * n;
	}

	void qpl::NN1::neuron::calculate_output(const std::vector<neuron>& prev_layer) {
		double sum = 0.0;
		for (std::size_t i = 0; i < prev_layer.size(); ++i) {
			sum += prev_layer[i].get_weighted_output(this->m_index);
		}
		this->m_output = qpl::NN1::neuron::transfer_func(sum);
	}
	void qpl::NN1::neuron::show_excpected_output(double excpeced_output) {
		double wrong = excpeced_output - m_output;
		this->m_gradient = wrong * transfer_func_derivative(this->m_output);
	}
	void qpl::NN1::neuron::learn_from_next_layer(const std::vector<neuron>& next_layer) {
		double sum = 0.0;
		for (std::size_t i = 0; i < next_layer.size() - 1; ++i) {
			sum += this->m_connections[i].get_weight() * next_layer[i].m_gradient;
		}
		this->m_gradient = sum * qpl::NN1::neuron::transfer_func_derivative(this->m_output);
	}
	void qpl::NN1::neuron::teach_previous_layer(std::vector<neuron>& previous_layer, qpl::f64 eta, qpl::f64 alpha) {
		for (std::size_t i = 0; i < previous_layer.size(); ++i) {
			double old_delta_weight = previous_layer[i].m_connections[this->m_index].get_delta_weight();
			double individual_input = eta * previous_layer[i].m_output * this->m_gradient;
			double momentum = alpha * old_delta_weight;
			double new_delta_weight = individual_input + momentum;

			previous_layer[i].m_connections[this->m_index].set_delta_weight(new_delta_weight);
			previous_layer[i].m_connections[this->m_index].add_weight(new_delta_weight);
		}
	}


	void qpl::NN1::neural_net::init() {
		this->m_neurons_created = false;
		this->m_generation_counter = 0u;
	}
	void qpl::NN1::neural_net::set_topology(const std::vector<unsigned>& topology) {
		this->m_neurons.resize(topology.size());
		for (std::size_t i = 0; i < this->m_neurons.size(); ++i) {
			this->m_neurons[i].resize(topology[i] + 1);
		}
		this->create();
	}

	std::size_t qpl::NN1::neural_net::get_number_of_neurons() const {
		auto top = this->get_topology();

		return qpl::container_sum(top);
	}
	std::size_t qpl::NN1::neural_net::get_number_of_connections() const {
		std::size_t sum = 0u;
		for (std::size_t i = 0; i < this->m_neurons.size(); ++i) {
			for (std::size_t j = 0; j < this->m_neurons[i].size(); ++j) {
				sum += this->m_neurons[i][j].get_number_of_connections();
			}
		}
		return sum;
	}

	std::vector<unsigned> qpl::NN1::neural_net::get_topology() const {
		std::vector<unsigned> result(this->m_neurons.size());
		for (std::size_t i = 0u; i < result.size(); ++i) {
			result[i] = static_cast<unsigned>(m_neurons[i].size() - 1);
		}
		return result;
	}
	bool qpl::NN1::neural_net::created() const {
		return this->m_neurons_created;
	}
	void qpl::NN1::neural_net::create() {
		for (unsigned i = 0; i < this->m_neurons.size(); ++i) {
			int connection_count = static_cast<int>(i == this->m_neurons.size() - 1 ? 0 : m_neurons[i + 1].size() - 1);
			for (unsigned j = 0; j < this->m_neurons[i].size(); ++j) {
				this->m_neurons[i][j].set_index(j);
				this->m_neurons[i][j].set_number_of_connections(connection_count);
				for (int c = 0; c < connection_count; ++c) {
					this->m_neurons[i][j].get_connection(c).set_weight(qpl::random(0.0, 1.0));
				}
			}
			m_neurons[i].back().set_output(1.0); //bias
		}
		m_neurons_created = true;
	}
	void qpl::NN1::neural_net::feed(const std::vector<double>& input) {
		for (std::size_t i = 0; i < input.size(); ++i) {
			if (std::isnan(input[i])) {
				qpl::println("nan");
				this->input_layer()[i].set_output(0);
			}
			else {
				this->input_layer()[i].set_output(input[i]);
			}
		}
		for (std::size_t i = 1; i < this->m_neurons.size(); ++i) {
			auto& prev_layer = this->m_neurons[i - 1];
			for (std::size_t j = 0; j < this->m_neurons[i].size() - 1; ++j) {
				this->m_neurons[i][j].calculate_output(prev_layer);
			}
		}
	}
	void qpl::NN1::neural_net::feed(double input) {
		this->feed(std::vector<double>({ input }));
	}
	void qpl::NN1::neural_net::get_output(std::vector<double>& dest) {
		dest.resize(this->output_layer().size() - 1);
		for (std::size_t i = 0; i < dest.size(); ++i) {
			dest[i] = this->output_layer()[i].get_output();
		}
	}

	void qpl::NN1::neural_net::teach(const std::vector<double>& excpected_outputs) {
		++this->m_generation_counter;
		this->m_error = 0.0;
		for (std::size_t i = 0; i < this->output_layer().size() - 1; ++i) {
			double wrong = excpected_outputs[i] - this->output_layer()[i].get_output();
			this->m_error += wrong * wrong;
		}
		this->m_error /= (this->output_layer().size() - 1);
		this->m_error = std::sqrt(this->m_error);

		this->accuracy_sum += (1.0 - this->m_error);

		for (std::size_t i = 0; i < this->output_layer().size() - 1; ++i) {
			this->output_layer()[i].show_excpected_output(excpected_outputs[i]);
		}
		for (std::size_t i = this->m_neurons.size() - 2; i > 0; --i) {
			std::vector<neuron>& next_layer = this->m_neurons[i + 1];
			for (std::size_t j = 0; j < this->m_neurons[i].size(); ++j) {
				this->m_neurons[i][j].learn_from_next_layer(next_layer);
			}
		}
		for (std::size_t i = this->m_neurons.size() - 1; i > 0; --i) {
			std::vector<neuron>& prev_layer = this->m_neurons[i - 1];
			for (std::size_t j = 0u; j < m_neurons[i].size() - 1; ++j) {
				this->m_neurons[i][j].teach_previous_layer(prev_layer, this->eta, this->alpha);
			}
		}
	}
	void qpl::NN1::neural_net::teach(double input) {
		this->teach(std::vector<double>({ input }));
	}
	double qpl::NN1::neural_net::get_error() const {
		return this->m_error;
	}
	double qpl::NN1::neural_net::get_accuracy() const {
		return 1.0 - this->m_error;
	}
	double qpl::NN1::neural_net::get_average_accuracy() const {
		return this->accuracy_sum / this->m_generation_counter;
	}
	std::size_t qpl::NN1::neural_net::get_generation_count() const {
		return this->m_generation_counter;
	}

	std::vector<qpl::NN1::neuron>& qpl::NN1::neural_net::operator[](unsigned index) {
		return this->m_neurons[index];
	}
	const std::vector<qpl::NN1::neuron>& qpl::NN1::neural_net::operator[](unsigned index) const {
		return this->m_neurons[index];
	}
	std::vector<qpl::NN1::neuron>& qpl::NN1::neural_net::at(unsigned index) {
		return this->m_neurons.at(index);
	}
	const std::vector<qpl::NN1::neuron>& qpl::NN1::neural_net::at(unsigned index) const {
		return this->m_neurons.at(index);
	}
	std::size_t qpl::NN1::neural_net::size() const {
		return this->m_neurons.size();
	}
	std::vector<qpl::NN1::neuron>& qpl::NN1::neural_net::input_layer() {
		return this->m_neurons[0];
	}
	std::vector<qpl::NN1::neuron>& qpl::NN1::neural_net::output_layer() {
		return this->m_neurons.back();
	}

	//-------------------------------------------------------------------------------

	qpl::f64 qpl::NN2::activation_function(qpl::f64 n) {
		return 1.0 / (1.0 + std::exp(-n));
	}
	qpl::f64 qpl::NN2::normal_distribution() {
		return std::sin(2.0 * qpl::pi * qpl::random(std::numeric_limits<double>::epsilon(), 1.0)) * std::sqrt(-2.0 * std::log(qpl::random(std::numeric_limits<double>::epsilon(), 1.0)));
	}

	qpl::f64 qpl::NN2::neural_net::get_accuracy() const {
		return this->accuracy;
	}
	qpl::f64 qpl::NN2::neural_net::get_average_accuracy() const {
		return this->accuracy_sum / this->generation_ctr;
	}

	std::vector<qpl::u32> qpl::NN2::neural_net::get_topology() const {
		return this->topology;
	}

	void qpl::NN2::neural_net::set_topology(const std::vector<qpl::u32>& topology) {
		this->topology = topology;
		this->layers.resize(topology.size());

		for (qpl::u32 l = 0u; l < this->layers.size(); ++l) {
			this->layers[l].neurons.resize(topology[l]);
		}

		this->randomize_weights_and_biases();
	}

	void qpl::NN2::neural_net::randomize_weights_and_biases() {
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
	void qpl::NN2::neural_net::observe_test() const {
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
	qpl::size qpl::NN2::neural_net::generation_count() const {
		return this->generation_ctr;
	}

	qpl::NN2::layer& qpl::NN2::neural_net::input_layer() {
		return this->layers.front();
	}
	qpl::NN2::layer& qpl::NN2::neural_net::output_layer() {
		return this->layers.back();
	}

	void qpl::NN2::neural_net::feed(const std::vector<qpl::f64>& input) {
		//set input
		for (qpl::u32 n = 0u; n < this->input_layer().neurons.size(); ++n) {
			this->input_layer().neurons[n].output = input[n];
			this->input_layer().neurons[n].output = 0.0;
		}

		//feed forward
		for (qpl::u32 l = 0u; l < this->layers.size(); ++l) {
			for (qpl::u32 n = 0u; n < this->layers[l].neurons.size(); ++n) {
				qpl::f64 sum = 0.0;
				for (qpl::u32 s = 0u; s < this->layers[l].neurons[n].synapses.size(); ++s) {
					sum += (this->layers[l].neurons[n].synapses[s].weight * this->layers[l - 1].neurons[s].output);
				}
				this->layers[l].neurons[n].output = qpl::NN2::activation_function(sum + this->layers[l].neurons[n].bias);
			}
		}
	}


	void qpl::NN2::neural_net::show_expected_output(const std::vector<qpl::f64>& expected_output) {
		for (qpl::u32 n = 0u; n < this->output_layer().neurons.size(); ++n) {
			auto momentum = this->output_layer().neurons[n].gradient * alpha;
			this->output_layer().neurons[n].gradient = expected_output[n] - this->output_layer().neurons[n].output + momentum;

		}
	}

	void qpl::NN2::layer::calculate_gradients(layer& previous_layer) {
		for (auto& neuron : previous_layer.neurons) {
			neuron.gradient = 0.0;
		}
		for (qpl::u32 n = 0u; n < this->neurons.size(); ++n) {
			for (qpl::u32 s = 0u; s < this->neurons[n].synapses.size(); ++s) {
				previous_layer.neurons[s].gradient += this->neurons[n].gradient * this->neurons[n].synapses[s].weight;
			}
		}
	}
	void qpl::NN2::layer::calculate_weights(const layer& previous_layer, qpl::f64 eta, qpl::f64 alpha) {
		for (qpl::u32 n = 0u; n < this->neurons.size(); ++n) {
			for (qpl::u32 s = 0u; s < this->neurons[n].synapses.size(); ++s) {
				this->neurons[n].synapses[s].delta_weight = this->neurons[n].gradient * previous_layer.neurons[s].output + this->neurons[n].synapses[s].delta_weight * alpha;
				this->neurons[n].synapses[s].weight += eta * this->neurons[n].synapses[s].delta_weight;

			}
			this->neurons[n].momentum = this->neurons[n].gradient + this->neurons[n].momentum * alpha;
			this->neurons[n].bias += eta * this->neurons[n].gradient;
		}
	}


	void qpl::NN2::neural_net::get_output(std::vector<qpl::f64>& destination) {
		for (qpl::u32 n = 0u; n < this->output_layer().neurons.size(); ++n) {
			destination[n] = this->output_layer().neurons[n].output;
		}
	}
	void qpl::NN2::neural_net::teach(const std::vector<qpl::f64>& expected_output) {
		this->show_expected_output(expected_output);

		qpl::f64 error_sum = 0.0;
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


	//-------------------------------------------------------------------------------

	qpl::f64 qpl::NN3::activation_function(qpl::f64 n) {
		return 1.0 / (1.0 + std::exp(-n));
	}
	qpl::f64 qpl::NN3::normal_distribution() {
		return std::sin(2.0 * qpl::pi * qpl::random(std::numeric_limits<double>::epsilon(), 1.0)) * std::sqrt(-2.0 * std::log(qpl::random(std::numeric_limits<double>::epsilon(), 1.0)));
	}

	qpl::f64 qpl::NN3::neural_net::get_accuracy() const {
		return this->accuracy;
	}
	qpl::f64 qpl::NN3::neural_net::get_average_accuracy() const {
		return this->accuracy_sum / this->generation_ctr;
	}

	std::vector<qpl::u32> qpl::NN3::neural_net::get_topology() const {
		return this->topology;
	}

	void qpl::NN3::neural_net::set_topology(const std::vector<qpl::u32>& topology) {
		this->topology = topology;
		this->layers.resize(topology.size());

		for (qpl::u32 l = 0u; l < this->layers.size(); ++l) {
			this->layers[l].neurons.resize(topology[l]);
		}

		this->randomize_weights_and_biases();
	}

	void qpl::NN3::neural_net::randomize_weights_and_biases() {
		this->accuracy_sum = 0.0;
		this->generation_ctr = 0u;


		for (qpl::u32 l = 1u; l < this->layers.size(); ++l) {

			for (auto& neuron : this->layers[l].neurons) {
				neuron.gradient = neuron.output = 0.0;
				neuron.bias = qpl::NN3::normal_distribution() * std::sqrt(1.0 / (this->layers[l - 1].neurons.size() + 1));
				neuron.bias *= 0.2;

				//neuron.bias = qpl::random(-1.0, 1.0);

				neuron.synapses.resize(this->layers[l - 1].neurons.size());

				for (qpl::u32 s = 0u; s < neuron.synapses.size(); ++s) {
					neuron.synapses[s].delta_weight = 0.0;
					//neuron.synapses[s].weight = qpl::random(-1.0, 1.0);
					neuron.synapses[s].weight = qpl::NN3::normal_distribution() * std::sqrt(1.0 / (this->layers[l - 1].neurons.size() + 1));

				}
			}
		}


	}
	void qpl::NN3::neural_net::observe_test() const {
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
	qpl::size qpl::NN3::neural_net::generation_count() const {
		return this->generation_ctr;
	}

	qpl::NN3::layer& qpl::NN3::neural_net::input_layer() {
		return this->layers.front();
	}
	qpl::NN3::layer& qpl::NN3::neural_net::output_layer() {
		return this->layers.back();
	}

	void qpl::NN3::neural_net::feed(const std::vector<qpl::f64>& input) {
		//set input
		for (qpl::u32 n = 0u; n < this->input_layer().neurons.size(); ++n) {
			this->input_layer().neurons[n].output = input[n];
		}

		//feed forward
		for (qpl::u32 l = 1u; l < this->layers.size(); ++l) {
			for (qpl::u32 n = 0u; n < this->layers[l].neurons.size(); ++n) {
				qpl::f64 sum = 0.0;
				for (qpl::u32 s = 0u; s < this->layers[l].neurons[n].synapses.size(); ++s) {
					sum += (this->layers[l].neurons[n].synapses[s].weight * this->layers[l - 1].neurons[s].output);
				}
				this->layers[l].neurons[n].output = qpl::NN3::activation_function(sum + this->layers[l].neurons[n].bias);
			}
		}
	}


	void qpl::NN3::neural_net::show_expected_output(const std::vector<qpl::f64>& expected_output) {
		for (qpl::u32 n = 0u; n < this->output_layer().neurons.size(); ++n) {
			auto momentum = this->output_layer().neurons[n].gradient * alpha;
			this->output_layer().neurons[n].gradient = expected_output[n] - this->output_layer().neurons[n].output + momentum;

		}
	}

	void qpl::NN3::layer::calculate_gradients(layer& previous_layer) {
		for (auto& neuron : previous_layer.neurons) {
			neuron.gradient = 0.0;
		}
		for (qpl::u32 n = 0u; n < this->neurons.size(); ++n) {
			for (qpl::u32 s = 0u; s < this->neurons[n].synapses.size(); ++s) {
				previous_layer.neurons[s].gradient += this->neurons[n].gradient * this->neurons[n].synapses[s].weight;
			}
		}
	}
	void qpl::NN3::layer::calculate_weights(const layer& previous_layer, qpl::f64 eta, qpl::f64 alpha) {
		for (qpl::u32 n = 0u; n < this->neurons.size(); ++n) {
			for (qpl::u32 s = 0u; s < this->neurons[n].synapses.size(); ++s) {
				this->neurons[n].synapses[s].delta_weight = this->neurons[n].gradient * previous_layer.neurons[s].output + this->neurons[n].synapses[s].delta_weight * alpha;
				this->neurons[n].synapses[s].weight += eta * this->neurons[n].synapses[s].delta_weight;

			}
			this->neurons[n].momentum = this->neurons[n].gradient + this->neurons[n].momentum * alpha;
			this->neurons[n].bias += eta * this->neurons[n].gradient;
		}
	}


	void qpl::NN3::neural_net::get_output(std::vector<qpl::f64>& destination) {
		for (qpl::u32 n = 0u; n < this->output_layer().neurons.size(); ++n) {
			destination[n] = this->output_layer().neurons[n].output;
		}
	}
	void qpl::NN3::neural_net::teach(const std::vector<qpl::f64>& expected_output) {
		this->show_expected_output(expected_output);

		qpl::f64 error_sum = 0.0;
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




	std::string qpl::static_neural_connection::info_string() const {
		return qpl::to_string(this->weight, ", ", this->delta_weight);
	}
	std::string qpl::static_neuron::info_string() const {
		return qpl::to_string(this->output, ", ", this->gradient);
	}

}