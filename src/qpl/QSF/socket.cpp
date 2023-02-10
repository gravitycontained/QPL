#include <qpl/QSF/socket.hpp>

#if defined QPL_INTERN_SFML_USE

#include <qpl/system.hpp>
#include <qpl/time.hpp>

namespace qsf {
	void qsf::socket::set_address(sf::IpAddress address, qpl::u32 port) {
		this->ip_address = address;
		this->port = port;
		this->address_set = true;
	}
	void qsf::socket::connect(sf::IpAddress address, qpl::u32 port) {
		this->set_address(address, port);
		this->connect();
	}

	void qsf::socket::connect() {
		if (!this->address_set) {
			throw std::exception("qsf::socket::connect: neither ip_adress nor port set!");
		}
		auto status = this->socket.connect(this->ip_address, this->port);
		this->connected = (status == sf::Socket::Done);
	}
	void qsf::socket::connect_until_connected() {
		qpl::u32 ctr = 0u;
		while (!this->is_connected()) {
			++ctr;
			qpl::print("[", this->ip_address.toString(), " : ", this->port, "] connection attempt ", ctr, " . . .");
			
			auto status = this->socket.connect(this->ip_address, this->port);
			this->connected = (status == sf::Socket::Done);
			if (this->connected) {
				qpl::println(" done");
			}
			else {
				qpl::println();
			}
		}
	}
	void qsf::socket::send(std::string data) {
		if (this->socket.send(data.c_str(), data.size()) != sf::Socket::Done) {
			qpl::println("qsf::socket::send: error sending");
		}
	}
	void qsf::socket::send_and_check_sent_size(std::string data) {
		qpl::size sent;
		this->socket.send(data.c_str(), data.size(), sent);
		if (sent != data.size()) {
			qpl::println("qsf::socket::send: trying to send ", data.size(), " bytes but only ", sent, " bytes arrived");
		}
	}
	bool qsf::socket::listen(qpl::u32 port, sf::IpAddress address) {
		if (this->listener.listen(port, address) != sf::Socket::Done) {
			qpl::println("qsf::socket::listen(", address.toString(), ", ", port, ") : error listening");
			return false;
		}
		return true;
	}
	bool qsf::socket::accept(qsf::socket& other) {
		if (this->listener.accept(other.socket) != sf::Socket::Done) {
			qpl::println("qsf::socket::accept(", other.ip_address.toString(), ") : error accepting");
			return false;
		}
		return true;
	}
	std::string qsf::socket::receive() {

		sf::Packet packet;
		if (this->socket.receive(packet) != sf::Socket::Done) {
			throw std::exception("qsf::socket::receive: error receiving!");
		}

		if (packet.getData()) {
			std::string result(reinterpret_cast<const char*>(packet.getData()), packet.getDataSize());
		}
		return std::string{};
	}
	void qsf::socket::disable_blocking() {
		this->socket.setBlocking(false);
		this->listener.setBlocking(false);
	}
	void qsf::socket::enable_blocking() {
		this->socket.setBlocking(true);
		this->listener.setBlocking(true);
	}
	bool qsf::socket::is_blocking_enabled() const {
		return this->socket.isBlocking();
	}
	bool qsf::socket::is_connected() const {
		return this->connected;
	}
}

#endif