#include <qpl/aes.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/string.hpp>
#include <qpl/memory.hpp>

#include <sstream>

namespace qpl {
	std::unique_ptr<qpl::detail::aes_tables_t> qpl::detail::aes_tables;

	qpl::u8 qpl::detail::galois_field_mul(qpl::u8 x, qpl::u8 y) {
		return (((y & 1) * x) ^
			((y >> 1 & 1) * qpl::detail::aes_tables->mul2[x]) ^
			((y >> 2 & 1) * qpl::detail::aes_tables->mul2[qpl::detail::aes_tables->mul2[x]]) ^
			((y >> 3 & 1) * qpl::detail::aes_tables->mul2[qpl::detail::aes_tables->mul2[qpl::detail::aes_tables->mul2[x]]]) ^
			((y >> 4 & 1) * qpl::detail::aes_tables->mul2[qpl::detail::aes_tables->mul2[qpl::detail::aes_tables->mul2[qpl::detail::aes_tables->mul2[x]]]]));
	}
	void qpl::detail::calculate_mul1() {
		for (int i = 0; i < 256; ++i) {
			qpl::detail::aes_tables->mul1[i] = static_cast<qpl::u8>(i);
		}
	}

	void qpl::detail::calculate_mul2() {
		for (int i = 0; i < 256; ++i) {
			qpl::detail::aes_tables->mul2[i] = static_cast<qpl::u8>((i << 1) ^ ((i & 0x80) ? 0x1B : 0x00));
		}
	}

	void qpl::detail::calculate_mul3() {
		for (int i = 0; i < 256; ++i) {
			qpl::detail::aes_tables->mul3[i] = static_cast<qpl::u8>(qpl::detail::aes_tables->mul2[i] ^ i);
		}
	}

	void qpl::detail::calculate_rcon() {
		qpl::detail::aes_tables->rcon[0] = 0x8d;
		qpl::u8 x = 1;
		for (int i = 1; i < 255; ++i) {
			qpl::detail::aes_tables->rcon[i] = static_cast<qpl::u8>(x);
			x = qpl::detail::aes_tables->mul2[x];
		}
	}

	void qpl::detail::calculate_sbox() {
		qpl::u8 p = 1, q = 1;

		do {
			//multiply p by 2
			p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

			//divide q by 2
			q ^= q << 1;
			q ^= q << 2;
			q ^= q << 4;
			q ^= q & 0x80 ? 0x09 : 0;

			//compute affine transformation
			unsigned char xformed = q ^ qpl::rotate_left(q, 1) ^ qpl::rotate_left(q, 2) ^ qpl::rotate_left(q, 3) ^ qpl::rotate_left(q, 4);
			qpl::detail::aes_tables->sbox[p] = xformed ^ 0x63;

		} while (p - 1);

		//0 is a special case since it has no inverse
		qpl::detail::aes_tables->sbox[0] = 0x63;
	}

	void qpl::detail::calculate_sbox_inv() {
		for (int i = 0; i < 256; ++i) {
			qpl::detail::aes_tables->sbox_inv[qpl::detail::aes_tables->sbox[i]] = static_cast<qpl::u8>(i);
		}
	}

	void qpl::generate_all_aes_tables() {
		if (!qpl::detail::aes_tables) {
			qpl::detail::aes_tables = std::make_unique<detail::aes_tables_t>();
		}
		qpl::detail::calculate_mul1();
		qpl::detail::calculate_mul2();
		qpl::detail::calculate_mul3();
		qpl::detail::calculate_rcon();
		qpl::detail::calculate_sbox();
		qpl::detail::calculate_sbox_inv();
	}

	void qpl::aes::construct() {
		this->m_constructed = true;
		this->set_mode(mode::aes_128);
		if (!qpl::detail::aes_tables) {
			qpl::generate_all_aes_tables();
		}
	}
	void qpl::aes::error_if_not_constructed() const {
		if (!this->m_constructed) {
			throw std::exception(qpl::to_string("qpl::aes: tries to access, but it was not constructed yet. call construct()").c_str());
		}
	}
	void qpl::aes::check_constructed() {
		if (!this->m_constructed) {
			this->construct();
		}
	}

	std::string qpl::aes::encrypted(const qpl::u8 message[16], const std::string& key) {
		this->check_constructed();
		this->set_state(message);
		this->set_key(key);
		this->expand_key();
		this->cipher();
		return this->get_message();
	}
	std::vector<qpl::u8> qpl::aes::encrypted(const qpl::u8* message, qpl::size size, const std::string& key) {
		this->check_constructed();
		this->set_key(key);
		this->expand_key();

		qpl::u8 last_block[16];
		qpl::u8 input_block[16];
		bool first_block = true;

		std::vector<qpl::u8> result;
		result.reserve((((size ? size - 1 : 0) / 16) + 1) * 16);

		qpl::size length = size;
		for (qpl::size i = 0u; i < length / 16u; ++i) {
			if (first_block) {
				for (int c = 0; c < 16; ++c) {
					input_block[c] = message[i * 16u + c];
				}
				first_block = false;
			}
			else {
				for (int c = 0; c < 16; ++c) {
					input_block[c] = last_block[c] ^ message[i * 16u + c];
				}
			}
			this->set_state(input_block);
			this->cipher();
			for (int i = 0; i < 16; ++i) {
				result.push_back(this->m_state[i]);
			}
			std::memcpy(last_block, this->m_state, 16);
		}
		if (size % 16) {
			if (first_block) {
				for (int c = 0; c < 16; ++c) {
					if (c >= size % 16) {
						input_block[c] = qpl::u8{};
					}
					else {
						input_block[c] = message[(size / 16) * 16 + c];
					}
				}
			}
			else {
				for (int c = 0; c < 16; ++c) {
					if (c >= size % 16) {
						input_block[c] = last_block[c] ^ qpl::u8{};
					}
					else {
						input_block[c] = last_block[c] ^ message[(size / 16) * 16 + c];
					}
				}
			}
			this->set_state(input_block);
			this->cipher();
			for (int i = 0; i < 16; ++i) {
				result.push_back(this->m_state[i]);
			}
		}

		return result;
	}
	std::string qpl::aes::encrypted(const std::string& message, const std::string& key) {
		auto s = qpl::aes::encrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length(), key);
		return std::string(std::move(reinterpret_cast<const char*>(s.data())), s.size());
	}
	std::wstring qpl::aes::encrypted(const std::wstring& message, const std::string& key) {
		auto s = qpl::aes::encrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length() * (sizeof(wchar_t) / sizeof(char)), key);
		return std::wstring(std::move(reinterpret_cast<const wchar_t*>(s.data())), s.size() / (sizeof(wchar_t) / sizeof(char)));
	}
	std::string qpl::aes::encrypted(const std::vector<char>& message, const std::string& key) {
		auto s = qpl::aes::encrypted(reinterpret_cast<const qpl::u8*>(message.data()), message.size(), key);
		return std::string(std::move(reinterpret_cast<const char*>(s.data())), s.size());
	}
	std::wstring qpl::aes::encrypted(const std::vector<wchar_t>& message, const std::string& key) {
		auto s = qpl::aes::encrypted(reinterpret_cast<const qpl::u8*>(message.data()), message.size() * (sizeof(wchar_t) / sizeof(char)), key);
		return std::wstring(std::move(reinterpret_cast<const wchar_t*>(s.data())), s.size() / (sizeof(wchar_t) / sizeof(char)));
	}
	void qpl::aes::encrypt(std::string& message, const std::string& key) {
		auto s = qpl::aes::encrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length(), key);
		qpl::container_memory_to_string(s, message);
	}
	void qpl::aes::encrypt(std::wstring& message, const std::string& key) {
		auto s = qpl::aes::encrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length()* (sizeof(wchar_t) / sizeof(char)), key);
		qpl::container_memory_to_wstring(s, message);
	}

	std::string qpl::aes::decrypted(const qpl::u8 message[16], const std::string& key) {
		this->check_constructed();
		this->set_state(message);
		this->set_key(key);
		this->expand_key();
		this->decipher();
		return this->get_message();
	}
	std::vector<qpl::u8> qpl::aes::decrypted(const qpl::u8* message, qpl::size size, const std::string& key, bool remove_null_terminations) {
		this->check_constructed();
		this->set_key(key);
		this->expand_key();

		qpl::u8 last_block[16];
		qpl::u8 input_block[16];
		qpl::u8 output_block[16];
		bool first_block = true;

		std::vector<qpl::u8> result;
		//result.reserve((((size ? size - 1 : 0) / 16) + 1) * 16);
		result.reserve(size);

		qpl::size length = size;
		for (qpl::size i = 0; i < length / 16u; ++i) {
			for (int c = 0; c < 16; ++c) {
				input_block[c] = message[i * 16 + c];
			}
			this->set_state(input_block);
			this->decipher();
			if (first_block) {
				for (int c = 0; c < 16; ++c) {
					output_block[c] = this->m_state[c];
				}
				first_block = false;
			}
			else {
				for (int c = 0; c < 16; ++c) {
					output_block[c] = last_block[c] ^ this->m_state[c];
				}
			}
			for (int i = 0; i < 16; ++i) {
				result.push_back(output_block[i]);
			}
			std::memcpy(last_block, input_block, 16u);
		}
		if (size % 16u) {
			for (qpl::size c = 0; c < 16u; ++c) {
				input_block[c] = c >= size ? 0 : message[(size / 16u) * 16u + c];
			}

			this->set_state(input_block);
			this->decipher();
			if (first_block) {
				for (int c = 0; c < 16; ++c) {
					if (c >= size) {
						output_block[c] = qpl::u8{};
					}
					else {
						output_block[c] = this->m_state[c];
					}
				}
			}
			else {
				for (int c = 0; c < 16; ++c) {
					if (c >= size) {
						output_block[c] = last_block[c] ^ qpl::u8{};
					}
					else {
						output_block[c] = last_block[c] ^ this->m_state[c];
					}
				}
			}
			for (int i = 0; i < 16; ++i) {
				result.push_back(output_block[i]);
			}
		}
		if (remove_null_terminations) {
			auto last = result.size() - 1;
			while (last && result[last] == qpl::u8{}) { --last; }
			result.resize(last + 1);
		}
		return result;
	}
	
	std::string qpl::aes::decrypted(const std::string& message, const std::string& key, bool remove_null_terminations) {
		auto s = qpl::aes::decrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length(), key, remove_null_terminations);
		return std::string(std::move(reinterpret_cast<const char*>(s.data())), s.size());
	}
	std::wstring qpl::aes::decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations) {
		auto s = qpl::aes::decrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length() * (sizeof(wchar_t) / sizeof(char)), key, remove_null_terminations);
		return std::wstring(std::move(reinterpret_cast<const wchar_t*>(s.data())), s.size() / (sizeof(wchar_t) / sizeof(char)));
	}
	std::string qpl::aes::decrypted(const std::vector<char>& message, const std::string& key, bool remove_null_terminations) {
		auto s = qpl::aes::decrypted(reinterpret_cast<const qpl::u8*>(message.data()), message.size(), key, remove_null_terminations);
		return std::string(std::move(reinterpret_cast<const char*>(s.data())), s.size());
	}
	std::wstring qpl::aes::decrypted(const std::vector<wchar_t>& message, const std::string& key, bool remove_null_terminations) {
		auto s = qpl::aes::decrypted(reinterpret_cast<const qpl::u8*>(message.data()), message.size() * (sizeof(wchar_t) / sizeof(char)), key, remove_null_terminations);
		return std::wstring(std::move(reinterpret_cast<const wchar_t*>(s.data())), s.size() / (sizeof(wchar_t) / sizeof(char)));
	}

	void qpl::aes::decrypt(std::string& message, const std::string& key, bool remove_null_terminations) {
		auto s = qpl::aes::decrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.size(), key, remove_null_terminations);
		qpl::container_memory_to_string(s, message);
	}
	void qpl::aes::decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations) {
		auto s = qpl::aes::decrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.size()* (sizeof(wchar_t) / sizeof(char)), key, remove_null_terminations);
		qpl::container_memory_to_wstring(s, message);
	}


	void qpl::aes::set_mode(mode m) {
		this->check_constructed();
		this->m_mode = m;

		if (m == mode::aes_128) {
			this->m_key_size = 16u;
			this->m_cipher_rounds = 10u;
		}
		else if (m == mode::aes_192) {
			this->m_key_size = 24u;
			this->m_cipher_rounds = 12u;
		}
		else if (m == mode::aes_256) {
			this->m_key_size = 32u;
			this->m_cipher_rounds = 14u;
		}
		this->m_round_key_size = 16 * (this->m_cipher_rounds + 1);

		this->m_key.resize(this->m_key_size);
		this->m_round_key.resize(this->m_round_key_size);
	}
	qpl::size qpl::aes::get_cipher_rounds() const {
		return this->m_cipher_rounds;
	}
	void qpl::aes::set_cipher_rounds(qpl::size count) {
		this->check_constructed();
		this->m_cipher_rounds = count;
		this->m_round_key_size = 16 * (this->m_cipher_rounds + 1);
		this->m_round_key.resize(this->m_round_key_size);
	}

	void qpl::aes::copy_message(qpl::u8* newState) const {
		this->error_if_not_constructed();
		for (int i = 0; i < 16; ++i) {
			newState[i] = this->m_state[i];
		}
	}

	std::string qpl::aes::get_message() const {
		this->error_if_not_constructed();
		std::string result;
		result.reserve(16);
		for (int i = 0; i < 16; ++i) {
			result += this->m_state[i];
		}
		return result;
	}
	bool qpl::aes::is_constructed() const {
		return this->m_constructed;
	}

	void qpl::aes::set_state(const qpl::u8 state[16]) {
		for (int i = 0; i < 16; ++i) {
			this->m_state[i] = state[i];
		}
	}

	void qpl::aes::set_state(const std::string state) {
		qpl::size i = 0;
		for (; i < qpl::min(state.length(), qpl::size(16u)); ++i) {
			this->m_state[i] = state[i];
		}
		for (; i < 16; ++i) {
			this->m_state[i] = 0u;
		}
	}

	void qpl::aes::set_key(const std::vector<qpl::u8>& key) {
		qpl::size i = 0;
		for (; i < qpl::min(key.size(), this->m_key_size); ++i) {
			this->m_key[i] = key[i];
		}
		for (; i < this->m_key_size; ++i) {
			this->m_key[i] = 0u;
		}
	}

	void qpl::aes::set_key(const std::string& key) {
		qpl::size i = 0;
		for (; i < qpl::min(key.length(), this->m_key_size); ++i) {
			this->m_key[i] = key[i];
		}
		for (; i < this->m_key_size; ++i) {
			this->m_key[i] = 0u;
		}
	}


	void qpl::aes::expand_key() {
		qpl::size bytes_generated = 0;
		qpl::u8 helperWORD[4];

		for (; bytes_generated < this->m_key_size; ++bytes_generated) {
			this->m_round_key[bytes_generated] = this->m_key[bytes_generated];
		}

		for (; bytes_generated < this->m_round_key_size; bytes_generated += 4) {

			for (int i = 0; i < 4; ++i) {
				helperWORD[i] = this->m_round_key[(bytes_generated - 4) + i];
			}

			if (bytes_generated % this->m_key_size == 0) {
				//function rotWORD()
				qpl::u8 temp = helperWORD[0];
				helperWORD[0] = helperWORD[1];
				helperWORD[1] = helperWORD[2];
				helperWORD[2] = helperWORD[3];
				helperWORD[3] = temp;

				for (int i = 0; i < 4; ++i) {
					helperWORD[i] = qpl::detail::aes_tables->sbox[helperWORD[i]];
				}

				helperWORD[0] ^= qpl::detail::aes_tables->rcon[bytes_generated / this->m_key_size];
			}

			if (this->m_key_size == 32u && bytes_generated % this->m_key_size == 16) {
				for (int i = 0; i < 4; ++i) {
					helperWORD[i] = qpl::detail::aes_tables->sbox[helperWORD[i]];
				}
			}

			for (int i = 0; i < 4; ++i) {
				this->m_round_key[bytes_generated + i] = this->m_round_key[(bytes_generated - this->m_key_size) + i] ^ helperWORD[i];
			}
		}
	}

	void qpl::aes::cipher() {
		this->add_round_key(0);

		for (qpl::size round = 1u; round < this->m_cipher_rounds; ++round) {
			this->sub_bytes();
			this->shift_rows();
			this->mix_columns();
			this->add_round_key(round);
		}
		this->sub_bytes();
		this->shift_rows();
		this->add_round_key(static_cast<int>(this->m_cipher_rounds));
	}

	void qpl::aes::decipher() {
		int round = static_cast<int>(this->m_cipher_rounds);

		this->add_round_key(round);
		for (round = static_cast<int>(this->m_cipher_rounds - 1); round > 0; --round) {
			this->unshift_rows();
			this->unsub_bytes();
			this->add_round_key(round);
			this->unmix_columns();
		}

		this->unshift_rows();
		this->unsub_bytes();
		this->add_round_key(0);
	}

	void qpl::aes::add_round_key(qpl::size rounds) {
		for (int i = 0; i < 16; ++i) {
			this->m_state[i] ^= this->m_round_key[rounds * 16 + i];
		}
	}

	void qpl::aes::sub_bytes() {
		for (int i = 0; i < 16; ++i) {
			this->m_state[i] = qpl::detail::aes_tables->sbox[this->m_state[i]];
		}
	}

	void qpl::aes::unsub_bytes() {
		for (int i = 0; i < 16; ++i) {
			this->m_state[i] = qpl::detail::aes_tables->sbox_inv[this->m_state[i]];
		}
	}

	void qpl::aes::shift_rows() {
		for (int row = 1; row < 4; ++row) {
			qpl::u8 temp[4];
			for (int i = 0; i < 4; ++i) {
				temp[i] = this->m_state[((i + row) % 4) * 4 + row];
			}
			for (int i = 0; i < 4; ++i) {
				this->m_state[i * 4 + row] = temp[i];
			}
		}
	}

	void qpl::aes::unshift_rows() {
		for (int row = 1; row < 4; ++row) {
			qpl::u8 temp[4];
			for (int i = 0; i < 4; ++i) {
				temp[i] = this->m_state[((4 + i - row) % 4) * 4 + row];
			}
			for (int i = 0; i < 4; ++i) {
				this->m_state[i * 4 + row] = temp[i];
			}
		}
	}

	void qpl::aes::mix_columns() {
		for (int columns = 0; columns < 4; ++columns) {

			int index0 = columns * 4 + 0;
			int index1 = columns * 4 + 1;
			int index2 = columns * 4 + 2;
			int index3 = columns * 4 + 3;

			qpl::u8 temp[4];
			temp[0] = qpl::detail::aes_tables->mul2[this->m_state[index0]] ^ qpl::detail::aes_tables->mul3[this->m_state[index1]] ^ qpl::detail::aes_tables->mul1[this->m_state[index2]] ^ qpl::detail::aes_tables->mul1[this->m_state[index3]];
			temp[1] = qpl::detail::aes_tables->mul1[this->m_state[index0]] ^ qpl::detail::aes_tables->mul2[this->m_state[index1]] ^ qpl::detail::aes_tables->mul3[this->m_state[index2]] ^ qpl::detail::aes_tables->mul1[this->m_state[index3]];
			temp[2] = qpl::detail::aes_tables->mul1[this->m_state[index0]] ^ qpl::detail::aes_tables->mul1[this->m_state[index1]] ^ qpl::detail::aes_tables->mul2[this->m_state[index2]] ^ qpl::detail::aes_tables->mul3[this->m_state[index3]];
			temp[3] = qpl::detail::aes_tables->mul3[this->m_state[index0]] ^ qpl::detail::aes_tables->mul1[this->m_state[index1]] ^ qpl::detail::aes_tables->mul1[this->m_state[index2]] ^ qpl::detail::aes_tables->mul2[this->m_state[index3]];
																																								
			for (int i = 0; i < 4; ++i) {
				this->m_state[columns * 4 + i] = temp[i];
			}
		}
	}

	void qpl::aes::unmix_columns() {
		for (int columns = 0; columns < 4; ++columns) {

			int index0 = columns * 4 + 0;
			int index1 = columns * 4 + 1;
			int index2 = columns * 4 + 2;
			int index3 = columns * 4 + 3;

			qpl::u8 temp[4];
			temp[0] = qpl::detail::galois_field_mul(this->m_state[index0], 0xe) ^ qpl::detail::galois_field_mul(this->m_state[index1], 0xb) ^ qpl::detail::galois_field_mul(this->m_state[index2], 0xd) ^ qpl::detail::galois_field_mul(this->m_state[index3], 0x9);
			temp[1] = qpl::detail::galois_field_mul(this->m_state[index0], 0x9) ^ qpl::detail::galois_field_mul(this->m_state[index1], 0xe) ^ qpl::detail::galois_field_mul(this->m_state[index2], 0xb) ^ qpl::detail::galois_field_mul(this->m_state[index3], 0xd);
			temp[2] = qpl::detail::galois_field_mul(this->m_state[index0], 0xd) ^ qpl::detail::galois_field_mul(this->m_state[index1], 0x9) ^ qpl::detail::galois_field_mul(this->m_state[index2], 0xe) ^ qpl::detail::galois_field_mul(this->m_state[index3], 0xb);
			temp[3] = qpl::detail::galois_field_mul(this->m_state[index0], 0xb) ^ qpl::detail::galois_field_mul(this->m_state[index1], 0xd) ^ qpl::detail::galois_field_mul(this->m_state[index2], 0x9) ^ qpl::detail::galois_field_mul(this->m_state[index3], 0xe);

			for (int i = 0; i < 4; ++i) {
				this->m_state[columns * 4 + i] = temp[i];
			}
		}
	}
	

	qpl::aes qpl::detail::aes(false);

	std::string qpl::aes_256_encrypted(const std::string& message, const std::string & key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_256);
		return qpl::detail::aes.encrypted(message, key);
	}
	std::string qpl::aes_192_encrypted(const std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_192);
		return qpl::detail::aes.encrypted(message, key);
	}
	std::string qpl::aes_128_encrypted(const std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_128);
		return qpl::detail::aes.encrypted(message, key);
	}

	std::string qpl::aes_256_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_256);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}
	std::string qpl::aes_192_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_192);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}
	std::string qpl::aes_128_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_128);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}

	void qpl::aes_256_encrypt(std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_256);
		qpl::detail::aes.encrypt(message, key);
	}
	void qpl::aes_192_encrypt(std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_192);
		qpl::detail::aes.encrypt(message, key);
	}
	void qpl::aes_128_encrypt(std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_128);
		qpl::detail::aes.encrypt(message, key);
	}
	
	void qpl::aes_256_decrypt(std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_256);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}
	void qpl::aes_192_decrypt(std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_192);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}
	void qpl::aes_128_decrypt(std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_128);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}

	std::wstring qpl::aes_256_encrypted(const std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_256);
		return qpl::detail::aes.encrypted(message, key);
	}
	std::wstring qpl::aes_192_encrypted(const std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_192);
		return qpl::detail::aes.encrypted(message, key);
	}
	std::wstring qpl::aes_128_encrypted(const std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_128);
		return qpl::detail::aes.encrypted(message, key);
	}

	std::wstring qpl::aes_256_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_256);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}
	std::wstring qpl::aes_192_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_192);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}
	std::wstring qpl::aes_128_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_128);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}

	void qpl::aes_256_encrypt(std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_256);
		qpl::detail::aes.encrypt(message, key);
	}
	void qpl::aes_192_encrypt(std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_192);
		qpl::detail::aes.encrypt(message, key);
	}
	void qpl::aes_128_encrypt(std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_128);
		qpl::detail::aes.encrypt(message, key);
	}

	void qpl::aes_256_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_256);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}
	void qpl::aes_192_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_192);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}
	void qpl::aes_128_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::aes_128);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}

	void make_string_multiple_of_16_bytes(std::string& string) {
		auto s = string.size() % 16;
		if (!s) return;
		string.append(std::string(16 - s, '\0'));
	}
	std::string qpl::encrypt(const std::string& message, const std::string& key) {
		return qpl::aes_256_encrypted(message, key);
	}
	std::string qpl::decrypt(const std::string& message, const std::string& key) {
		return qpl::aes_256_decrypted(message, key);
	}
}