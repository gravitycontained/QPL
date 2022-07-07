#pragma once
#ifndef QPL_WINSOCK_HPP
#define QPL_WINSOCK_HPP
#ifdef QPL_WINSOCK

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <string>

namespace qpl::winsock {
	QPLDLL std::string get_website_content(std::string url, qpl::size buffer_size = 10'000u);
}

#endif
#endif