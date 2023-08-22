#pragma once
#include <stdarg.h>

#include <codecvt>
#include <locale>
#include <sstream>
#include <string>
#include <type_traits>

#include "LoggerImp.h"

#ifdef _WIN32
#include <winrt/base.h>
#endif

// Dummy type for immediate flushing
struct __flush {
};

template <class OSS, class T>
concept oss_support_t = requires(OSS oss, T t) { oss << t; };

class LogStreamStorage : private std::ostringstream	 // TODO: consider using wide version
{
public:
	using base_t = std::ostringstream;

	LogStreamStorage(LOG_LEVEL level) : m_Level(level) {}

	template <class T>
	requires oss_support_t<base_t, T>
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
	requires oss_support_t<base_t, T>
	LogStreamStorage& operator<<(T val)
	{
		if (m_Level > m_MaxLevel) {
			return *this;
		}

		*(static_cast<base_t*>(this)) << val;
		return *this;
	}

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
	requires oss_support_t<LogStreamStorage::base_t, T>
	LogStreamStorage operator<<(T val)	// factory
	{
		return LogStreamStorage(LVL, val);
	}

	LogStreamStorage operator<<(std::wstring_view val)
	{
		return LogStreamStorage(LVL, std::move(to_utf8(val)));
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
			buf = buf.c_str();	// reset the size
			LogStreamStorage(LVL, std::move(buf));
		}
		va_end(args);

		// TODO: Create constexpr check for printf specifiers
	}

	void operator()(std::wstring_view format, ...)
	{
		if (format.empty() || LVL > LogStreamStorage::get_level()) {
			return;
		}

		va_list args;
		va_start(args, format);
		auto size = std::vswprintf(nullptr, 0, format.data(), args);
		if (size > format.size()) {
			std::wstring buf(size, '\0');
			size = std::vswprintf(buf.data(), buf.size(), format.data(), args);
			// msvc calc the vsnprintf size with null terminator
			buf = buf.c_str();	// reset the size
			LogStreamStorage(LVL, std::move(to_utf8(buf)));
		}
		va_end(args);

		// TODO: Create constexpr check for printf specifiers
	}

private:
	std::string to_utf8(std::wstring_view ws)
	{
#ifdef _WIN32
		// if windows
		return winrt::to_string(ws);
#endif
	}
};
