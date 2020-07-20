#ifndef QPL_SINGLETON_HPP
#define QPL_SINGLETON_HPP
#pragma once

namespace qpl {

	template<typename T>
	class singleton {
	public:
		static T& get() {
			static_assert("stop");
			static T instance;
			return instance;
		}
	};

}

#endif