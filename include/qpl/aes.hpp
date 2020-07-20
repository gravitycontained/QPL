#ifndef QPL_AES_HPP
#define QPL_AES_HPP
#pragma once
#include <cstdlib>
#include <memory>
#include <vector>
#include <string>

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>

namespace qpl {
		namespace detail {
			struct aes_tables_t {
				qpl::u8 mul1[256];
				qpl::u8 mul2[256];
				qpl::u8 mul3[256];
				qpl::u8 rcon[255];
				qpl::u8 sbox[256];
				qpl::u8 sbox_inv[256];

				bool created = false;
			};
			QPLDLL extern qpl::detail::aes_tables_t aes_tables;

			QPLDLL qpl::u8 galois_field_mul(qpl::u8 x, qpl::u8 y);


			QPLDLL void calculate_mul1();
			QPLDLL void calculate_mul2();
			QPLDLL void calculate_mul3();
			QPLDLL void calculate_rcon();

			QPLDLL void calculate_sbox();
			QPLDLL void calculate_sbox_inv();
		}

		QPLDLL void generate_all_aes_tables();

		class AES {
		public:
			enum class mode {
				AES_128, AES_192, AES_256
			};

			AES() {
				this->construct();
			}
			~AES() {
				this->destroy();
			}

			QPLDLL std::string encrypted(const qpl::u8 message[16], const std::string& key);
			QPLDLL std::vector<qpl::u8> encrypted(const qpl::u8* message, qpl::size size, const std::string& key);
			QPLDLL std::string encrypted(const std::string& message, const std::string& key);
			QPLDLL std::wstring encrypted(const std::wstring& message, const std::string& key);
			QPLDLL std::string encrypted(const std::vector<char>& message, const std::string& key);
			QPLDLL std::wstring encrypted(const std::vector<wchar_t>& message, const std::string& key);

			QPLDLL void encrypt(std::string& message, const std::string& key);
			QPLDLL void encrypt(std::wstring& message, const std::string& key);

			QPLDLL std::string decrypted(const qpl::u8 message[16], const std::string& key);
			QPLDLL std::vector<qpl::u8> decrypted(const qpl::u8* message, qpl::size size, const std::string& key, bool remove_null_terminations = false);
			QPLDLL std::string decrypted(const std::string& message, const std::string& key, bool remove_null_terminations = false);
			QPLDLL std::wstring decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations = false);
			QPLDLL std::string decrypted(const std::vector<char>& message, const std::string& key, bool remove_null_terminations = false);
			QPLDLL std::wstring decrypted(const std::vector<wchar_t>& message, const std::string& key, bool remove_null_terminations = false);

			QPLDLL void decrypt(std::string& message, const std::string& key, bool remove_null_terminations = false);
			QPLDLL void decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations = false);

			QPLDLL void set_mode(mode m);
			QPLDLL qpl::size get_cipher_rounds() const;
			QPLDLL void set_cipher_rounds(qpl::size count);
			QPLDLL void copy_message(qpl::u8* newState) const;
			QPLDLL std::string get_message() const;

		private:
			QPLDLL void construct();
			QPLDLL void destroy();

			QPLDLL void set_state(const qpl::u8 state[16]);
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


			mode m_mode;
			qpl::u8 m_state[16];
			qpl::u8* m_key;
			qpl::u8* m_round_key;

			qpl::size m_key_size;
			qpl::size m_cipher_rounds;
			qpl::size m_round_key_size;

		};
		namespace detail {
			QPLDLL extern qpl::AES aes;
		}

		QPLDLL std::string AES_256_encrypted(const std::string& message, const std::string& key);
		QPLDLL std::string AES_192_encrypted(const std::string& message, const std::string& key);
		QPLDLL std::string AES_128_encrypted(const std::string& message, const std::string& key);

		QPLDLL std::string AES_256_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL std::string AES_192_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL std::string AES_128_decrypted(const std::string& message, const std::string& key, bool remove_null_terminations = false);

		QPLDLL void AES_256_encrypt(std::string& message, const std::string& key);
		QPLDLL void AES_192_encrypt(std::string& message, const std::string& key);
		QPLDLL void AES_128_encrypt(std::string& message, const std::string& key);
		
		QPLDLL void AES_256_decrypt(std::string& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL void AES_192_decrypt(std::string& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL void AES_128_decrypt(std::string& message, const std::string& key, bool remove_null_terminations = false);

		QPLDLL std::wstring AES_256_encrypted(const std::wstring& message, const std::string& key);
		QPLDLL std::wstring AES_192_encrypted(const std::wstring& message, const std::string& key);
		QPLDLL std::wstring AES_128_encrypted(const std::wstring& message, const std::string& key);
				
		QPLDLL std::wstring AES_256_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL std::wstring AES_192_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL std::wstring AES_128_decrypted(const std::wstring& message, const std::string& key, bool remove_null_terminations = false);

		QPLDLL void AES_256_encrypt(std::wstring& message, const std::string& key);
		QPLDLL void AES_192_encrypt(std::wstring& message, const std::string& key);
		QPLDLL void AES_128_encrypt(std::wstring& message, const std::string& key);

		QPLDLL void AES_256_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL void AES_192_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations = false);
		QPLDLL void AES_128_decrypt(std::wstring& message, const std::string& key, bool remove_null_terminations = false);

}

#endif