#ifdef QPL_WINSOCK

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <qpl/winsock.hpp>
#include <qpl/string.hpp>
#include <windows.h>


namespace qpl::winsock {
	std::string qpl::winsock::get_website_content(std::string url, qpl::size buffer_size) {
		std::string website_content;
		WSADATA wsa_data;
		SOCKET web_socket;
		SOCKADDR_IN socket_address;


		qpl::size line_count = 0u;
		qpl::size row_count = 0u;

		struct hostent* host;
		std::string get_http;

		get_http += "GET / HTTP/1.1\r\nHost: ";
		get_http += url;
		get_http += "\r\nconnection: close\r\n\r\n";

		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
			qpl::println("WSAStartup failed (", WSAGetLastError(), ")");
			return "";
		}

		web_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		host = gethostbyname(url.c_str());

		if (host == nullptr) {
			qpl::println("couldn't get host with URL: ", url, " (", WSAGetLastError(), ")");
			return "";
		}

		socket_address.sin_port = htons(80);
		socket_address.sin_family = AF_INET;
		socket_address.sin_addr.s_addr = *((unsigned long*)host->h_addr);

		if (connect(web_socket, reinterpret_cast<SOCKADDR*>(&socket_address), sizeof(socket_address)) != 0) {
			qpl::println("could not connect to URL: ", url, " (", WSAGetLastError(), ")");
			return "";
		}

		send(web_socket, get_http.c_str(), get_http.length(), 0);

		std::string buffer;
		buffer.resize(buffer_size);


		qpl::i32 received_size;
		while ((received_size = recv(web_socket, buffer.data(), buffer.size(), 0)) > 0) {
			for (qpl::size i = 0u; buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r'; ++i) {
				website_content += buffer[i];
			}
		}
		closesocket(web_socket);
		WSACleanup();
		return website_content;
	}
}
#endif