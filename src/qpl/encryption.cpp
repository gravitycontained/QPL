#include <qpl/encryption.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/string.hpp>
#include <qpl/time.hpp>

#include <sstream>

namespace qpl {
	std::unique_ptr<qpl::detail::aes_tables_t> qpl::detail::aes_tables_deprecated;

	void qpl::detail::calculate_mul1() {
		for (qpl::size i = 0u; i < qpl::detail::aes_tables_deprecated->mul1.size(); ++i) {
			qpl::detail::aes_tables_deprecated->mul1[i] = static_cast<qpl::u8>(i);
		}
	}

	void qpl::detail::calculate_mul2() {
		for (qpl::size i = 0u; i < qpl::detail::aes_tables_deprecated->mul2.size(); ++i) {
			qpl::detail::aes_tables_deprecated->mul2[i] = static_cast<qpl::u8>((i << 1) ^ ((i & 0x80) ? 0x1B : 0x00));
		}
	}

	void qpl::detail::calculate_mul3() {
		for (qpl::size i = 0u; i < qpl::detail::aes_tables_deprecated->mul3.size(); ++i) {
			qpl::detail::aes_tables_deprecated->mul3[i] = static_cast<qpl::u8>(qpl::detail::aes_tables_deprecated->mul2[i] ^ i);
		}
	}

	void qpl::detail::calculate_rcon() {
		qpl::detail::aes_tables_deprecated->rcon[0] = 0x8d;
		qpl::u8 x = 1;
		for (qpl::size i = 1u; i < 255u; ++i) {
			qpl::detail::aes_tables_deprecated->rcon[i] = static_cast<qpl::u8>(x);
			x = qpl::detail::aes_tables_deprecated->mul2[x];
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
			qpl::detail::aes_tables_deprecated->sbox[p] = xformed ^ 0x63;

		} while (p - 1);

		//0 is a special case since it has no inverse
		qpl::detail::aes_tables_deprecated->sbox[0] = 0x63;
	}

	void qpl::detail::calculate_sbox_inv() {
		for (qpl::size i = 0u; i < qpl::detail::aes_tables_deprecated->sbox_inv.size(); ++i) {
			qpl::detail::aes_tables_deprecated->sbox_inv[qpl::detail::aes_tables_deprecated->sbox[i]] = static_cast<qpl::u8>(i);
		}
	}

	void qpl::generate_all_aes_tables() {
		if (!qpl::detail::aes_tables_deprecated) {
			qpl::detail::aes_tables_deprecated = std::make_unique<detail::aes_tables_t>();
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
		this->set_mode(mode::_128);
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

	std::string qpl::aes::encrypted(const std::array<qpl::u8, 16>& message, const std::string& key) {
		this->check_constructed();
		this->set_state(message);
		this->set_key(key);
		this->expand_key();
		this->cipher();
		return this->get_message();
	}
	std::string qpl::aes::encrypted(const qpl::u8* message, qpl::size size, const std::string& key) {
		this->check_constructed();
		this->set_key(key);
		this->expand_key();

		std::array<qpl::u8, 16> last_block;
		std::array<qpl::u8, 16> input_block;
		bool first_block = true;

		std::string result;
		result.reserve((((size ? size - 1 : 0) / this->m_state.size()) + 1) * this->m_state.size());

		for (qpl::size i = 0u; i < size / this->m_state.size(); ++i) {
			if (first_block) {
				for (qpl::size c = 0u; c < this->m_state.size(); ++c) {
					input_block[c] = message[i * this->m_state.size() + c];
				}
				first_block = false;
			}
			else {
				for (qpl::size c = 0u; c < this->m_state.size(); ++c) {
					input_block[c] = last_block[c] ^ message[i * this->m_state.size() + c];
				}
			}

			this->set_state(input_block);

			this->cipher();

			result.append(reinterpret_cast<const char*>(this->m_state.data()), this->m_state.size());
			std::memcpy(last_block.data(), this->m_state.data(), this->m_state.size());
		}
		if (size % this->m_state.size()) {
			if (first_block) {
				for (qpl::size c = 0u; c < this->m_state.size(); ++c) {
					if (c >= size % this->m_state.size()) {
						input_block[c] = qpl::u8{};
					}
					else {
						input_block[c] = message[(size / this->m_state.size()) * this->m_state.size() + c];
					}
				}
			}
			else {
				for (qpl::size c = 0u; c < this->m_state.size(); ++c) {
					if (c >= size % this->m_state.size()) {
						input_block[c] = last_block[c] ^ qpl::u8{};
					}
					else {
						input_block[c] = last_block[c] ^ message[(size / this->m_state.size()) * this->m_state.size() + c];
					}
				}
			}

			this->set_state(input_block);
			this->cipher();
			result.append(reinterpret_cast<const char*>(this->m_state.data()), this->m_state.size());
		}

		return result;
	}
	std::string qpl::aes::encrypted(const std::string& message, const std::string& key) {
		//auto s = this->encrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length(), key);
		//return std::string(std::move(reinterpret_cast<const char*>(s.data())), s.size());
		return this->encrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length(), key);
	}
	std::wstring qpl::aes::encrypted(const std::wstring& message, const std::string& key) {
		auto s = this->encrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length() * (sizeof(wchar_t) / sizeof(char)), key);
		return std::wstring(std::move(reinterpret_cast<const wchar_t*>(s.data())), s.size() / (sizeof(wchar_t) / sizeof(char)));
	}
	std::string qpl::aes::encrypted(const std::vector<char>& message, const std::string& key) {
		auto s = this->encrypted(reinterpret_cast<const qpl::u8*>(message.data()), message.size(), key);
		return std::string(std::move(reinterpret_cast<const char*>(s.data())), s.size());
	}
	std::wstring qpl::aes::encrypted(const std::vector<wchar_t>& message, const std::string& key) {
		auto s = this->encrypted(reinterpret_cast<const qpl::u8*>(message.data()), message.size() * (sizeof(wchar_t) / sizeof(char)), key);
		return std::wstring(std::move(reinterpret_cast<const wchar_t*>(s.data())), s.size() / (sizeof(wchar_t) / sizeof(char)));
	}
	void qpl::aes::encrypt(std::string& message, const std::string& key) {
		message = this->encrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length(), key);
		//qpl::container_memory_to_string(s, message);
	}
	void qpl::aes::encrypt(std::wstring& message, const std::string& key) {
		auto s = this->encrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length()* (sizeof(wchar_t) / sizeof(char)), key);
		qpl::container_memory_to_wstring(s, message);
	}

	std::string qpl::aes::decrypted(const std::array<qpl::u8, 16>& message, const std::string& key) {
		this->check_constructed();
		this->set_state(message);
		this->set_key(key);
		this->expand_key();
		this->decipher();
		return this->get_message();
	}
	std::string qpl::aes::decrypted(const qpl::u8* message, qpl::size size, const std::string& key, bool remove_null_terminations) {
		this->check_constructed();
		this->set_key(key);
		this->expand_key();

		std::array<qpl::u8, 16> last_block;
		std::array<qpl::u8, 16> input_block;
		std::array<qpl::u8, 16> output_block;
		bool first_block = true;

		std::string result;
		//result.reserve((((size ? size - 1 : 0) / 16) + 1) * 16);
		result.reserve(size);

		qpl::size length = size;
		for (qpl::size i = 0u; i < length / this->m_state.size(); ++i) {
			for (qpl::size c = 0u; c < this->m_state.size(); ++c) {
				input_block[c] = message[i * this->m_state.size() + c];
			}
			this->set_state(input_block);
			this->decipher();
			if (first_block) {
				for (qpl::size c = 0u; c < this->m_state.size(); ++c) {
					output_block[c] = this->m_state[c];
				}
				first_block = false;
			}
			else {
				for (qpl::size c = 0u; c < this->m_state.size(); ++c) {
					output_block[c] = last_block[c] ^ this->m_state[c];
				}
			}
			for (qpl::size i = 0u; i < this->m_state.size(); ++i) {
				result.push_back(output_block[i]);
			}
			std::memcpy(last_block.data(), input_block.data(), last_block.size());
		}
		if (size % this->m_state.size()) {
			for (qpl::size c = 0; c < this->m_state.size(); ++c) {
				input_block[c] = c >= size ? 0 : message[(size / this->m_state.size()) * this->m_state.size() + c];
			}

			this->set_state(input_block);
			this->decipher();
			if (first_block) {
				for (qpl::size c = 0u; c < this->m_state.size(); ++c) {
					if (c >= size) {
						output_block[c] = qpl::u8{};
					}
					else {
						output_block[c] = this->m_state[c];
					}
				}
			}
			else {
				for (qpl::size c = 0u; c < this->m_state.size(); ++c) {
					if (c >= size) {
						output_block[c] = last_block[c] ^ qpl::u8{};
					}
					else {
						output_block[c] = last_block[c] ^ this->m_state[c];
					}
				}
			}
			for (qpl::size i = 0u; i < this->m_state.size(); ++i) {
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
		return qpl::aes::decrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.length(), key, remove_null_terminations);
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
		message = qpl::aes::decrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.size(), key, remove_null_terminations);
	}
	void qpl::aes::decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations) {
		auto s = qpl::aes::decrypted(reinterpret_cast<const qpl::u8*>(message.c_str()), message.size()* (sizeof(wchar_t) / sizeof(char)), key, remove_null_terminations);
		qpl::container_memory_to_wstring(s, message);
	}


	void qpl::aes::set_mode(mode m) {
		this->check_constructed();
		this->m_mode = m;

		if (m == mode::_128) {
			this->m_key_size = 16u;
			this->m_cipher_rounds = 10u;
		}
		else if (m == mode::_192) {
			this->m_key_size = 24u;
			this->m_cipher_rounds = 12u;
		}
		else if (m == mode::_256) {
			this->m_key_size = 32u;
			this->m_cipher_rounds = 14u;
		}
		this->m_round_key_size = this->m_state.size() * (this->m_cipher_rounds + 1);

		this->m_key.resize(this->m_key_size);
		this->m_round_key.resize(this->m_round_key_size);
	}
	qpl::size qpl::aes::get_cipher_rounds() const {
		return this->m_cipher_rounds;
	}
	void qpl::aes::set_cipher_rounds(qpl::size count) {
		this->check_constructed();
		this->m_cipher_rounds = count;
		this->m_round_key_size = this->m_state.size() * (this->m_cipher_rounds + 1);
		this->m_round_key.resize(this->m_round_key_size);
	}

	void qpl::aes::copy_message(qpl::u8* newState) const {
		this->error_if_not_constructed();
		for (qpl::u32 i = 0u; i < this->m_state.size(); ++i) {
			newState[i] = this->m_state[i];
		}
	}

	std::string qpl::aes::get_message() const {
		this->error_if_not_constructed();
		std::string result;
		result.reserve(this->m_state.size());
		for (qpl::size i = 0u; i < this->m_state.size(); ++i) {
			result += this->m_state[i];
		}
		return result;
	}
	bool qpl::aes::is_constructed() const {
		return this->m_constructed;
	}

	void qpl::aes::set_state(const std::array<qpl::u8, 16>& state) {
		this->m_state = state;
	}

	void qpl::aes::set_state(const std::string state) {
		qpl::size i = 0;
		for (; i < qpl::min(state.length(), this->m_state.size()); ++i) {
			this->m_state[i] = state[i];
		}
		for (; i < this->m_state.size(); ++i) {
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

			for (qpl::u32 i = 0u; i < 4u; ++i) {
				helperWORD[i] = this->m_round_key[(bytes_generated - 4) + i];
			}

			if (bytes_generated % this->m_key_size == 0) {
				//function rotWORD()
				qpl::u8 temp = helperWORD[0];
				helperWORD[0] = helperWORD[1];
				helperWORD[1] = helperWORD[2];
				helperWORD[2] = helperWORD[3];
				helperWORD[3] = temp;

				for (qpl::size i = 0u; i < 4u; ++i) {
					helperWORD[i] = qpl::detail::aes_tables::sbox[helperWORD[i]];
				}

				helperWORD[0] ^= qpl::detail::aes_tables::rcon[bytes_generated / this->m_key_size];
			}

			if (this->m_key_size == 32u && bytes_generated % this->m_key_size == 16) {
				for (qpl::u32 i = 0u; i < 4u; ++i) {
					helperWORD[i] = qpl::detail::aes_tables::sbox[helperWORD[i]];
				}
			}

			for (qpl::u32 i = 0u; i < 4u; ++i) {
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
		this->add_round_key(this->m_cipher_rounds);
	}

	void qpl::aes::decipher() {
		this->add_round_key(this->m_cipher_rounds);
		for (auto round = qpl::i32_cast(this->m_cipher_rounds - 1); round > 0; --round) {
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
		for (qpl::u32 i = 0u; i < this->m_state.size(); ++i) {
			this->m_state[i] ^= this->m_round_key[rounds * this->m_state.size() + i];
		}
	}

	void qpl::aes::sub_bytes() {
		for (qpl::u32 i = 0u; i < this->m_state.size(); ++i) {
			this->m_state[i] = qpl::detail::aes_tables::sbox[this->m_state[i]];
		}
	}

	void qpl::aes::unsub_bytes() {
		for (qpl::u32 i = 0u; i < this->m_state.size(); ++i) {
			this->m_state[i] = qpl::detail::aes_tables::sbox_inv[this->m_state[i]];
		}
	}

	void qpl::aes::shift_rows() {
		std::array<u8, 4u> temp;
		for (qpl::u32 row = 1u; row < 4u; ++row) {
			for (qpl::u32 i = 0u; i < 4u; ++i) {
				temp[i] = this->m_state[((i + row) % 4u) * 4u + row];
			}
			for (qpl::u32 i = 0u; i < 4u; ++i) {
				this->m_state[i * 4u + row] = temp[i];
			}
		}
	}

	void qpl::aes::unshift_rows() {
		std::array<u8, 4u> temp;
		for (qpl::u32 row = 1u; row < 4u; ++row) {
			for (qpl::u32 i = 0u; i < 4u; ++i) {
				temp[i] = this->m_state[((4u + i - row) % 4u) * 4u + row];
			}
			for (qpl::u32 i = 0u; i < 4u; ++i) {
				this->m_state[i * 4u + row] = temp[i];
			}
		}
	}

	void qpl::aes::mix_columns() {
		for (qpl::u32 columns = 0u; columns < 4u; ++columns) {

			auto index0 = columns * 4 + 0;
			auto index1 = columns * 4 + 1;
			auto index2 = columns * 4 + 2;
			auto index3 = columns * 4 + 3;

			std::array<qpl::u8, 4u> temp;
			temp[0] = qpl::detail::aes_tables::mul2[this->m_state[index0]] ^ qpl::detail::aes_tables::mul3[this->m_state[index1]] ^ qpl::detail::aes_tables::mul1[this->m_state[index2]] ^ qpl::detail::aes_tables::mul1[this->m_state[index3]];
			temp[1] = qpl::detail::aes_tables::mul1[this->m_state[index0]] ^ qpl::detail::aes_tables::mul2[this->m_state[index1]] ^ qpl::detail::aes_tables::mul3[this->m_state[index2]] ^ qpl::detail::aes_tables::mul1[this->m_state[index3]];
			temp[2] = qpl::detail::aes_tables::mul1[this->m_state[index0]] ^ qpl::detail::aes_tables::mul1[this->m_state[index1]] ^ qpl::detail::aes_tables::mul2[this->m_state[index2]] ^ qpl::detail::aes_tables::mul3[this->m_state[index3]];
			temp[3] = qpl::detail::aes_tables::mul3[this->m_state[index0]] ^ qpl::detail::aes_tables::mul1[this->m_state[index1]] ^ qpl::detail::aes_tables::mul1[this->m_state[index2]] ^ qpl::detail::aes_tables::mul2[this->m_state[index3]];

			for (qpl::u32 i = 0; i < 4; ++i) {
				this->m_state[columns * 4 + i] = temp[i];
			}
		}
	}

	void qpl::aes::unmix_columns() {
		for (qpl::u32 columns = 0u; columns < 4u; ++columns) {

			auto index0 = columns * 4 + 0;
			auto index1 = columns * 4 + 1;
			auto index2 = columns * 4 + 2;
			auto index3 = columns * 4 + 3;

			std::array<qpl::u8, 4u> temp;
			temp[0] = qpl::detail::aes_tables::mul14[this->m_state[index0]] ^ qpl::detail::aes_tables::mul11[this->m_state[index1]] ^ qpl::detail::aes_tables::mul13[this->m_state[index2]] ^ qpl::detail::aes_tables::mul9 [this->m_state[index3]];
			temp[1] = qpl::detail::aes_tables::mul9 [this->m_state[index0]] ^ qpl::detail::aes_tables::mul14[this->m_state[index1]] ^ qpl::detail::aes_tables::mul11[this->m_state[index2]] ^ qpl::detail::aes_tables::mul13[this->m_state[index3]];
			temp[2] = qpl::detail::aes_tables::mul13[this->m_state[index0]] ^ qpl::detail::aes_tables::mul9 [this->m_state[index1]] ^ qpl::detail::aes_tables::mul14[this->m_state[index2]] ^ qpl::detail::aes_tables::mul11[this->m_state[index3]];
			temp[3] = qpl::detail::aes_tables::mul11[this->m_state[index0]] ^ qpl::detail::aes_tables::mul13[this->m_state[index1]] ^ qpl::detail::aes_tables::mul9 [this->m_state[index2]] ^ qpl::detail::aes_tables::mul14[this->m_state[index3]];

			for (qpl::u32 i = 0u; i < 4u; ++i) {
				this->m_state[columns * 4u + i] = temp[i];
			}
		}
	}
	

	qpl::aes qpl::detail::aes(false);

	std::string qpl::aes_256_encrypted(const std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		return qpl::detail::aes.encrypted(message, key);
	}
	std::string qpl::aes_192_encrypted(const std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		return qpl::detail::aes.encrypted(message, key);
	}
	std::string qpl::aes_128_encrypted(const std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		return qpl::detail::aes.encrypted(message, key);
	}

	std::string qpl::aes_256_encrypted(const std::string& message, const std::array<qpl::u64, 4>& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		std::string s;
		qpl::container_memory_to_string(key, s);
		return qpl::detail::aes.encrypted(message, s);
	}
	std::string qpl::aes_192_encrypted(const std::string& message, const std::array<qpl::u64, 3>& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		std::string s;
		qpl::container_memory_to_string(key, s);
		return qpl::detail::aes.encrypted(message, s);
	}
	std::string qpl::aes_128_encrypted(const std::string& message, const std::array<qpl::u64, 2>& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		std::string s;
		qpl::container_memory_to_string(key, s);
		return qpl::detail::aes.encrypted(message, s);
	}

	std::string qpl::aes_256_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}
	std::string qpl::aes_192_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}
	std::string qpl::aes_128_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}


	std::string qpl::aes_256_decrypted(const std::string& message, const std::array<qpl::u64, 4>& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		std::string s;
		qpl::container_memory_to_string(key, s);
		return qpl::detail::aes.decrypted(message, s, remove_null_terminations);
	}
	std::string qpl::aes_192_decrypted(const std::string& message, const std::array<qpl::u64, 3>& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		std::string s;
		qpl::container_memory_to_string(key, s);
		return qpl::detail::aes.decrypted(message, s, remove_null_terminations);
	}
	std::string qpl::aes_128_decrypted(const std::string& message, const std::array<qpl::u64, 2>& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		std::string s;
		qpl::container_memory_to_string(key, s);
		return qpl::detail::aes.decrypted(message, s, remove_null_terminations);
	}

	void qpl::aes_256_encrypt(std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		qpl::detail::aes.encrypt(message, key);
	}
	void qpl::aes_192_encrypt(std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		qpl::detail::aes.encrypt(message, key);
	}
	void qpl::aes_128_encrypt(std::string& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		qpl::detail::aes.encrypt(message, key);
	}

	void qpl::aes_256_encrypt(std::string& message, const std::array<qpl::u64, 4>& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		std::string s;
		qpl::container_memory_to_string(key, s);
		qpl::detail::aes.encrypt(message, s);
	}
	void qpl::aes_192_encrypt(std::string& message, const std::array<qpl::u64, 3>& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		std::string s;
		qpl::container_memory_to_string(key, s);
		qpl::detail::aes.encrypt(message, s);
	}
	void qpl::aes_128_encrypt(std::string& message, const std::array<qpl::u64, 2>& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		std::string s;
		qpl::container_memory_to_string(key, s);
		qpl::detail::aes.encrypt(message, s);
	}

	void qpl::aes_256_decrypt(std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}
	void qpl::aes_192_decrypt(std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}
	void qpl::aes_128_decrypt(std::string& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}

	void qpl::aes_256_decrypt(std::string& message, const std::array<qpl::u64, 4>& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		std::string s;
		qpl::container_memory_to_string(key, s);
		qpl::detail::aes.decrypt(message, s, remove_null_terminations);
	}
	void qpl::aes_192_decrypt(std::string& message, const std::array<qpl::u64, 3>& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		std::string s;
		qpl::container_memory_to_string(key, s);
		qpl::detail::aes.decrypt(message, s, remove_null_terminations);
	}
	void qpl::aes_128_decrypt(std::string& message, const std::array<qpl::u64, 2>& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		std::string s;
		qpl::container_memory_to_string(key, s);
		qpl::detail::aes.decrypt(message, s, remove_null_terminations);
	}
	std::wstring qpl::aes_256_encrypted(const std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		return qpl::detail::aes.encrypted(message, key);
	}
	std::wstring qpl::aes_192_encrypted(const std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		return qpl::detail::aes.encrypted(message, key);
	}
	std::wstring qpl::aes_128_encrypted(const std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		return qpl::detail::aes.encrypted(message, key);
	}

	std::wstring qpl::aes_256_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}
	std::wstring qpl::aes_192_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}
	std::wstring qpl::aes_128_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		return qpl::detail::aes.decrypted(message, key, remove_null_terminations);
	}

	void qpl::aes_256_encrypt(std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		qpl::detail::aes.encrypt(message, key);
	}
	void qpl::aes_192_encrypt(std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		qpl::detail::aes.encrypt(message, key);
	}
	void qpl::aes_128_encrypt(std::wstring& message, const std::string& key) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
		qpl::detail::aes.encrypt(message, key);
	}

	void qpl::aes_256_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_256);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}
	void qpl::aes_192_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_192);
		qpl::detail::aes.decrypt(message, key, remove_null_terminations);
	}
	void qpl::aes_128_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations) {
		qpl::detail::aes.set_mode(qpl::aes::mode::_128);
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
	std::string qpl::decrypt(const std::string& message, const std::string& key, bool remove_null_terminations) {
		return qpl::aes_256_decrypted(message, key, remove_null_terminations);
	}

	void qpl::aes_128_encrypt_keep_size(std::string& string, const std::string& key) {
		auto n = ((string.length() - 1) / 16);
		auto diff = qpl::u8_cast((n + 1) * 16u - string.length());
		qpl::aes_128_encrypt(string, key);

		auto size = string.length();
		string.resize(size + 1);
		memcpy(string.data() + size, &diff, 1u);
	}
	std::string qpl::aes_128_encrypted_keep_size(std::string string, const std::string& key) {
		qpl::aes_128_encrypt_keep_size(string, key);
		return string;
	}
	void qpl::aes_128_decrypt_keep_size(std::string& string, const std::string& key) {
		qpl::u8 diff;
		memcpy(&diff, &string.back(), 1u);
		string.pop_back();

		qpl::aes_128_decrypt(string, key);
		string.resize(string.length() - diff);
	}
	std::string qpl::aes_128_decrypted_keep_size(std::string string, const std::string& key) {
		qpl::aes_128_decrypt_keep_size(string, key);
		return string;
	}
	void qpl::aes_192_encrypt_keep_size(std::string& string, const std::string& key) {
		auto n = ((string.length() - 1) / 16);
		auto diff = qpl::u8_cast((n + 1) * 16u - string.length());
		qpl::aes_192_encrypt(string, key);

		auto size = string.length();
		string.resize(size + 1);
		memcpy(string.data() + size, &diff, 1u);
	}
	std::string qpl::aes_192_encrypted_keep_size(std::string string, const std::string& key) {
		qpl::aes_192_encrypt_keep_size(string, key);
		return string;
	}
	void qpl::aes_192_decrypt_keep_size(std::string& string, const std::string& key) {
		qpl::u8 diff;
		memcpy(&diff, &string.back(), 1u);
		string.pop_back();

		qpl::aes_192_decrypt(string, key);
		string.resize(string.length() - diff);
	}
	std::string qpl::aes_192_decrypted_keep_size(std::string string, const std::string& key) {
		qpl::aes_192_decrypt_keep_size(string, key);
		return string;
	}

	void qpl::aes_256_encrypt_keep_size(std::string& string, const std::string& key) {
		auto n = ((string.length() - 1) / 16);
		auto diff = qpl::u8_cast((n + 1) * 16u - string.length());
		qpl::aes_256_encrypt(string, key);

		auto size = string.length();
		string.resize(size + 1);
		memcpy(string.data() + size, &diff, 1u);
	}
	std::string qpl::aes_256_encrypted_keep_size(std::string string, const std::string& key) {
		qpl::aes_256_encrypt_keep_size(string, key);
		return string;
	}
	void qpl::aes_256_decrypt_keep_size(std::string& string, const std::string& key) {
		qpl::u8 diff;
		memcpy(&diff, &string.back(), 1u);
		string.pop_back();

		qpl::aes_256_decrypt(string, key);
		string.resize(string.length() - diff);
	}
	std::string qpl::aes_256_decrypted_keep_size(std::string string, const std::string& key) {
		qpl::aes_256_decrypt_keep_size(string, key);
		return string;
	}

	void qpl::encrypt_keep_size(std::string& string, const std::string& key, qpl::aes::mode mode) {
		switch (mode) {
		case qpl::aes::mode::_128:
			qpl::aes_128_encrypt_keep_size(string, key);
			break;
		case qpl::aes::mode::_192:
			qpl::aes_192_encrypt_keep_size(string, key);
			break;
		case qpl::aes::mode::_256:
			qpl::aes_256_encrypt_keep_size(string, key);
			break;
		}
	}
	std::string qpl::encrypted_keep_size(std::string string, const std::string& key, qpl::aes::mode mode) {
		switch (mode) {
		case qpl::aes::mode::_128:
			return qpl::aes_128_encrypted_keep_size(string, key);
			break;
		case qpl::aes::mode::_192:
			return qpl::aes_192_encrypted_keep_size(string, key);
			break;
		case qpl::aes::mode::_256:
			return qpl::aes_256_encrypted_keep_size(string, key);
			break;
		}
		return "";
	}
	void qpl::decrypt_keep_size(std::string& string, const std::string& key, qpl::aes::mode mode) {
		switch (mode) {
		case qpl::aes::mode::_128:
			qpl::aes_128_decrypt_keep_size(string, key);
			break;
		case qpl::aes::mode::_192:
			qpl::aes_192_decrypt_keep_size(string, key);
			break;
		case qpl::aes::mode::_256:
			qpl::aes_256_decrypt_keep_size(string, key);
			break;
		}
	}
	std::string qpl::decrypted_keep_size(std::string string, const std::string& key, qpl::aes::mode mode) {
		switch (mode) {
		case qpl::aes::mode::_128:
			return qpl::aes_128_decrypted_keep_size(string, key);
			break;
		case qpl::aes::mode::_192:
			return qpl::aes_192_decrypted_keep_size(string, key);
			break;
		case qpl::aes::mode::_256:
			return qpl::aes_256_decrypted_keep_size(string, key);
			break;
		}
		return "";
	}


#ifdef QPL_CIPHER
	qpl::cipherN<cipher_config{ 4, 1, 64, 64 } > qpl::detail::cipher512_quick;
	qpl::cipherN<cipher_config{ 4, 3, 64, 64 } > qpl::detail::cipher512;
	qpl::cipherN<cipher_config{ 4, 12, 64, 64 } > qpl::detail::cipher512_save;


	std::string qpl::encrypt512_quick(const std::string& message, const std::string& key) {
		return qpl::detail::cipher512_quick.encrypt(message, key);
	}
	std::string qpl::decrypt512_quick(const std::string& message, const std::string& key) {
		return qpl::detail::cipher512_quick.decrypt(message, key);
	}
	std::string qpl::encrypt512(const std::string& message, const std::string& key) {
		return qpl::detail::cipher512.encrypt(message, key);
	}
	std::string qpl::decrypt512(const std::string& message, const std::string& key) {
		return qpl::detail::cipher512.decrypt(message, key);
	}
	std::string qpl::encrypt512_save(const std::string& message, const std::string& key) {
		return qpl::detail::cipher512_save.encrypt(message, key);
	}
	std::string qpl::decrypt512_save(const std::string& message, const std::string& key) {
		return qpl::detail::cipher512_save.decrypt(message, key);
	}
#endif
}