#pragma once
#include <exception>
#include <common/format.h>

namespace common
{
	template <typename T> struct TypedDataMessageException : std::exception
	{
		T m_data;
		std::string message_;
		template <typename... Ts> TypedDataMessageException(std::string_view fmt, Ts&&... ts)
		{
			message_ = common::format(fmt, ts...);
			// message_ = std::vformat(fmt, std::make_format_args(std::forward<Ts>(ts)...));
		}
		template <typename... Ts> TypedDataMessageException(T data_, std::string_view fmt, Ts&&... ts)
		{
			m_data = data_;
			message_ = common::format(fmt, ts...);
			// message_ = std::vformat(fmt, std::make_format_args(std::forward<Ts>(ts)...));
		}
		const char* what() const noexcept override
		{
			return message_.c_str();
		}
		void set_data(T& t)
		{
			m_data = t;
		}
		const T& get_data() const
		{
			return m_data;
		}
	};
}; // namespace common
