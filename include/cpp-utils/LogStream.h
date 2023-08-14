#pragma once
#include <stdarg.h>

#include <sstream>
#include <string>

#include "LoggerImp.h"

// Dummy type for immediate flushing
struct __flush {
};

class LogStreamStorage : private std::ostringstream
{
	using base_t = std::ostringstream;

public:
	LogStreamStorage(LOG_LEVEL level) : m_Level(level) {}

	template <class T>
	LogStreamStorage(LOG_LEVEL level, T val) : LogStreamStorage(level)
	{
		if (m_Level > m_MaxLevel) {
			return;
		}

		*(static_cast<base_t*>(this)) << val;
	}

	~LogStreamStorage()
	{
		if (m_Level > m_MaxLevel) {
			return;
		}

		m_Log.log(m_Level, base_t::str());
	}

	template <class T>
	LogStreamStorage& operator<<(T val)
	{
		if (m_Level > m_MaxLevel) {
			return *this;
		}

		*(static_cast<base_t*>(this)) << val;
		return *this;
	}

	template <>
	LogStreamStorage& operator<<(__flush val)
	{
		if (m_Level > m_MaxLevel) {
			return *this;
		}

		m_Log.log(m_Level, base_t::str());
		return *this;
	}

	void flush()
	{
		if (m_Level > m_MaxLevel) {
			return;
		}

		m_Log.log(m_Level, base_t::str());
		base_t::clear();
	}

	static inline void set_level(LOG_LEVEL level)
	{
		m_MaxLevel = level;
	}
	static inline LOG_LEVEL get_level()
	{
		return m_MaxLevel;
	}

private:
	LOG_LEVEL m_Level;
	static LOG_LEVEL m_MaxLevel;

	LoggerImp& m_Log = LoggerImp::instance();
};

template <LOG_LEVEL LVL>
class LogStream
{
public:
	template <class T>
	LogStreamStorage operator<<(T val)	// factory
	{
		return LogStreamStorage(LVL, val);
	}

	void operator()(std::string_view format, ...)
	{
		if (format.empty() || LVL > LogStreamStorage::get_level()) {
			return;
		}

		va_list args;
		va_start(args, format);
		auto size = std::vsnprintf(nullptr, 0, format.data(), args);
		if (size > format.size()) {
			std::string buf(size, '\0');
			size = std::vsnprintf(buf.data(), buf.size(), format.data(), args);
			// msvc calc the vsnprintf size with null terminator
			buf = buf.c_str(); // reset the size
			LogStreamStorage(LVL, std::string_view(buf));
		}
		va_end(args);

		// if constexpr (validate_format<std::tuple<Args...>>(format)) {
		//	return;
		// }

		// LogStreamStorage lss(LVL);

		// lss << format.substr(last);

		// print log
	}

private:
	// template <class Args>
	// consteval static bool validate_format(const std::string_view format)
	//{
	//	size_t last = 0;
	//	size_t idx = 0;
	//	for (size_t pos = format.find('%', 0); pos != std::string_view::npos;
	//		 pos = format.find('%', last), idx++) {
	//		switch (format[pos++]) {
	//			case 'd': {
	//				auto b = std::same_as<std::tuple_element_t<idx, Args>, int>;
	//				break;
	//			}
	//			default:
	//				break;
	//		}

	//		last = pos;
	//	}
	//	return true;
	//}
};
