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
	void qsf::socket::set_address_connect(sf::IpAddress address, qpl::u32 port) {
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
		if (!this->connected) {
			throw std::exception("qsf::socket::send: not connected!");
		}
		qpl::size sent;
		this->socket.send(data.c_str(), data.size(), sent);
		if (sent != data.size()) {
			qpl::println("qsf::socket::send: trying to send ", data.size(), " bytes but only ", sent, " bytes arrived");
		}
	}
	std::string qsf::socket::receive() {
		if (!this->connected) {
			throw std::exception("qsf::socket::receive: not connected!");
		}

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
	}
	void qsf::socket::enable_blocking() {
		this->socket.setBlocking(true);
	}
	bool qsf::socket::is_blocking_enabled() const {
		return this->socket.isBlocking();
	}
	bool qsf::socket::is_connected() const {
		return this->connected;
	}
}

#endif