#pragma once

#include "mn/Context.h"
#include "mn/Fmt.h"

namespace mn
{
	template<typename... TArgs>
	inline static void
	log_debug(const char* fmt, TArgs&&... args)
	{
		#ifdef DEBUG
		auto msg = mn::strf(fmt, args...);
		log_debug_str(msg.ptr);
		mn::str_free(msg);
		#endif
	}

	template<typename... TArgs>
	inline static void
	log_info(const char* fmt, TArgs&&... args)
	{
		auto msg = mn::strf(fmt, args...);
		log_info_str(msg.ptr);
		mn::str_free(msg);
	}

	template<typename... TArgs>
	inline static void
	log_warning(const char* fmt, TArgs&&... args)
	{
		auto msg = mn::strf(fmt, args...);
		log_warning_str(msg.ptr);
		mn::str_free(msg);
	}

	template<typename... TArgs>
	inline static void
	log_error(const char* fmt, TArgs&&... args)
	{
		auto msg = mn::strf(fmt, args...);
		log_error_str(msg.ptr);
		mn::str_free(msg);
	}

	template<typename... TArgs>
	[[noreturn]] inline static void
	log_critical(const char* fmt, TArgs&&... args)
	{
		auto msg = mn::strf(fmt, args...);
		log_critical_str(msg.ptr);
		mn::str_free(msg);
		abort();
	}

	template<typename... TArgs>
	inline static void
	log_ensure(bool expr, const char* fmt, TArgs&&... args)
	{
		if (expr == false)
			log_critical(fmt, args...);
	}
}