#pragma once
#ifndef QPL_EXCEPTION
#define QPL_EXCEPTION

#include <qpl/qpldeclspec.hpp>
#include <qpl/string.hpp>
#include <string>
#include <exception>

namespace qpl {
	struct exception : std::exception {
		std::string string;
		template<typename... Args>
		exception(Args... args) {
			this->string = qpl::to_string(args...);
		}
		const char* what() const override {
			return this->string.c_str();
		}
	};
}

#endif