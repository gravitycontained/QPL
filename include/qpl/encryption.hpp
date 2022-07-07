#ifndef QPL_ENCRYPTION_HPP
#define QPL_ENCRYPTION_HPP
#pragma once
#include <cstdlib>
#include <memory>
#include <array>
#include <vector>
#include <string>

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/memory.hpp>

namespace qpl {

	namespace detail {
		struct aes_tables_t {
			std::array<qpl::u8, 256> mul1;
			std::array<qpl::u8, 256> mul2;
			std::array<qpl::u8, 256> mul3;
			std::array<qpl::u8, 255> rcon;
			std::array<qpl::u8, 256> sbox;
			std::array<qpl::u8, 256> sbox_inv;
		};
		QPLDLL extern std::unique_ptr<qpl::detail::aes_tables_t> aes_tables;


		QPLDLL qpl::u8 galois_field_mul(qpl::u8 x, qpl::u8 y);


		QPLDLL void calculate_mul1();
		QPLDLL void calculate_mul2();
		QPLDLL void calculate_mul3();
		QPLDLL void calculate_rcon();

		QPLDLL void calculate_sbox();
		QPLDLL void calculate_sbox_inv();
	}

	QPLDLL void generate_all_aes_tables();

	class aes {
	public:
		enum class mode {
			_128, _192, _256
		};

		aes(bool construct = true) {
			this->m_state.fill(0u);
			if (construct) {
				this->construct();
			}
		}

		QPLDLL std::string encrypted(const std::array<qpl::u8, 16>& message, const std::string& key);
		QPLDLL std::vector<qpl::u8> encrypted(const qpl::u8* message, qpl::size size, const std::string& key);
		QPLDLL std::string encrypted(const std::string& message, const std::string& key);
		QPLDLL std::wstring encrypted(const std::wstring& message, const std::string& key);
		QPLDLL std::string encrypted(const std::vector<char>& message, const std::string& key);
		QPLDLL std::wstring encrypted(const std::vector<wchar_t>& message, const std::string& key);

		QPLDLL void encrypt(std::string& message, const std::string& key);
		QPLDLL void encrypt(std::wstring& message, const std::string& key);

		QPLDLL std::string decrypted(const std::array<qpl::u8, 16>& message, const std::string& key);
		QPLDLL std::vector<qpl::u8> decrypted(const qpl::u8* message, qpl::size size, const std::string& key, bool remove_null_terminations = false);
		QPLDLL std::string decrypted(const std::string& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL std::wstring decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL std::string decrypted(const std::vector<char>& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL std::wstring decrypted(const std::vector<wchar_t>& message, const std::string& key, bool remove_null_terminations = false);

		QPLDLL void decrypt(std::string& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL void decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations = false);

		QPLDLL void set_mode(mode m);
		QPLDLL void set_cipher_rounds(qpl::size count);
		QPLDLL qpl::size get_cipher_rounds() const;
		QPLDLL void copy_message(qpl::u8* newState) const;
		QPLDLL std::string get_message() const;

		QPLDLL bool is_constructed() const;
		QPLDLL void construct();
	private:
		QPLDLL void error_if_not_constructed() const;
		QPLDLL void check_constructed();

		QPLDLL void set_state(const std::array<qpl::u8, 16>& state);
		QPLDLL void set_state(const std::string state);
		QPLDLL void set_key(const std::vector<qpl::u8>& key);
		QPLDLL void set_key(const std::string& key);


		QPLDLL void expand_key();
		QPLDLL void cipher();
		QPLDLL void decipher();
		QPLDLL void add_round_key(qpl::size rounds);
		QPLDLL void sub_bytes();
		QPLDLL void unsub_bytes();
		QPLDLL void shift_rows();
		QPLDLL void unshift_rows();
		QPLDLL void mix_columns();
		QPLDLL void unmix_columns();

		bool m_constructed = false;
		mode m_mode = mode::_128;
		std::array<qpl::u8, 16> m_state;
		std::vector<qpl::u8> m_key;
		std::vector<qpl::u8> m_round_key;

		qpl::size m_key_size = 0u;
		qpl::size m_cipher_rounds = 0u;
		qpl::size m_round_key_size = 0u;

	};
	namespace detail {
		QPLDLL extern qpl::aes aes;
	}

	QPLDLL std::string aes_256_encrypted(const std::string& message, const std::string& key);
	QPLDLL std::string aes_192_encrypted(const std::string& message, const std::string& key);
	QPLDLL std::string aes_128_encrypted(const std::string& message, const std::string& key);

	QPLDLL std::string aes_256_encrypted(const std::string& message, const std::array<qpl::u64, 4>& key);
	QPLDLL std::string aes_192_encrypted(const std::string& message, const std::array<qpl::u64, 3>& key);
	QPLDLL std::string aes_128_encrypted(const std::string& message, const std::array<qpl::u64, 2>& key);

	QPLDLL std::string aes_256_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations = false);
	QPLDLL std::string aes_192_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations = false);
	QPLDLL std::string aes_128_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations = false);

	QPLDLL std::string aes_256_decrypted(const std::string& message, const std::array<qpl::u64, 4>& key, bool remove_null_terminations = false);
	QPLDLL std::string aes_192_decrypted(const std::string& message, const std::array<qpl::u64, 3>& key, bool remove_null_terminations = false);
	QPLDLL std::string aes_128_decrypted(const std::string& message, const std::array<qpl::u64, 2>& key, bool remove_null_terminations = false);

	QPLDLL void aes_256_encrypt(std::string& message, const std::string& key);
	QPLDLL void aes_192_encrypt(std::string& message, const std::string& key);
	QPLDLL void aes_128_encrypt(std::string& message, const std::string& key);

	QPLDLL void aes_256_encrypt(std::string& message, const std::array<qpl::u64, 4>& key);
	QPLDLL void aes_192_encrypt(std::string& message, const std::array<qpl::u64, 3>& key);
	QPLDLL void aes_128_encrypt(std::string& message, const std::array<qpl::u64, 2>& key);
		
	QPLDLL void aes_256_decrypt(std::string& message, const std::string& key, bool remove_null_terminations = false);
	QPLDLL void aes_192_decrypt(std::string& message, const std::string& key, bool remove_null_terminations = false);
	QPLDLL void aes_128_decrypt(std::string& message, const std::string& key, bool remove_null_terminations = false);

	QPLDLL void aes_256_decrypt(std::string& message, const std::array<qpl::u64, 4>& key, bool remove_null_terminations = false);
	QPLDLL void aes_192_decrypt(std::string& message, const std::array<qpl::u64, 3>& key, bool remove_null_terminations = false);
	QPLDLL void aes_128_decrypt(std::string& message, const std::array<qpl::u64, 2>& key, bool remove_null_terminations = false);

	QPLDLL std::wstring aes_256_encrypted(const std::wstring& message, const std::string& key);
	QPLDLL std::wstring aes_192_encrypted(const std::wstring& message, const std::string& key);
	QPLDLL std::wstring aes_128_encrypted(const std::wstring& message, const std::string& key);
				
	QPLDLL std::wstring aes_256_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations = false);
	QPLDLL std::wstring aes_192_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations = false);
	QPLDLL std::wstring aes_128_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations = false);

	QPLDLL void aes_256_encrypt(std::wstring& message, const std::string& key);
	QPLDLL void aes_192_encrypt(std::wstring& message, const std::string& key);
	QPLDLL void aes_128_encrypt(std::wstring& message, const std::string& key);

	QPLDLL void aes_256_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations = false);
	QPLDLL void aes_192_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations = false);
	QPLDLL void aes_128_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations = false);

	QPLDLL void make_string_multiple_of_16_bytes(std::string& string);

	QPLDLL std::string encrypt(const std::string& message, const std::string& key);
	QPLDLL std::string decrypt(const std::string& message, const std::string& key, bool remove_null_terminations = false);

	template<typename T, qpl::size N>
	std::string encrypt(const std::string& message, const std::array<T, N>& key) {
		constexpr auto bit_size = qpl::bits_in_type<T>() * N;
		std::string s;
		qpl::container_memory_to_string(key, s);
		if constexpr (bit_size <= 128) {
			return qpl::aes_128_encrypted(message, s);
		}
		else if constexpr (bit_size <= 192) {
			return qpl::aes_192_encrypted(message, s);
		}
		else {
			return qpl::aes_256_encrypted(message, s);
		}
	}
	template<typename T, qpl::size N>
	std::string decrypt(const std::string& message, const std::array<T, N>& key, bool remove_null_terminations = false) {
		auto bit_size = qpl::bits_in_type<T>() * N;
		std::string s;
		qpl::container_memory_to_string(key, s);
		if (bit_size <= 128) {
			return qpl::aes_128_decrypted(message, s, remove_null_terminations);
		}
		else if (bit_size <= 192) {
			return qpl::aes_192_decrypted(message, s, remove_null_terminations);
		}
		else {
			return qpl::aes_256_decrypted(message, s, remove_null_terminations);
		}
	}
	QPLDLL void aes_128_encrypt_keep_size(std::string& string, const std::string& key);
	QPLDLL std::string aes_128_encrypted_keep_size(std::string string, const std::string& key);
	QPLDLL void aes_128_decrypt_keep_size(std::string& string, const std::string& key);
	QPLDLL std::string aes_128_decrypted_keep_size(std::string string, const std::string& key);

	QPLDLL void aes_192_encrypt_keep_size(std::string& string, const std::string& key);
	QPLDLL std::string aes_192_encrypted_keep_size(std::string string, const std::string& key);
	QPLDLL void aes_192_decrypt_keep_size(std::string& string, const std::string& key);
	QPLDLL std::string aes_192_decrypted_keep_size(std::string string, const std::string& key);

	QPLDLL void aes_256_encrypt_keep_size(std::string& string, const std::string& key);
	QPLDLL std::string aes_256_encrypted_keep_size(std::string string, const std::string& key);
	QPLDLL void aes_256_decrypt_keep_size(std::string& string, const std::string& key);
	QPLDLL std::string aes_256_decrypted_keep_size(std::string string, const std::string& key);

	QPLDLL void encrypt_keep_size(std::string& string, const std::string& key, qpl::aes::mode mode = qpl::aes::mode::_256);
	QPLDLL std::string encrypted_keep_size(std::string string, const std::string& key, qpl::aes::mode mode = qpl::aes::mode::_256);
	QPLDLL void decrypt_keep_size(std::string& string, const std::string& key, qpl::aes::mode mode = qpl::aes::mode::_256);
	QPLDLL std::string decrypted_keep_size(std::string string, const std::string& key, qpl::aes::mode mode = qpl::aes::mode::_256);
}

#endif