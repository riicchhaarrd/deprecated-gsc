#pragma once
#include <string>
#include <algorithm>

namespace filesystem
{
	class filepath
	{
		std::string m_str;

	  public:
		static std::string delimeter()
		{
			return "/";
		}

		filepath()
		{
		}

		template <typename T> filepath(const T& s) noexcept : m_str(s)
		{
			std::replace(m_str.begin(), m_str.end(), '\\', '/');
		}

		bool endswith(const std::string& s) const
		{
			size_t l = s.length();
			size_t maxlen = m_str.length();
			if (l > maxlen)
				return false;
			for (size_t i = 0; i < l; ++i)
				if (m_str[maxlen - i] != s[i])
					return false;
			return true;
		}

		bool startswith(const std::string& s) const
		{
			size_t l = s.length();
			size_t maxlen = m_str.length();
			if (l > maxlen)
				return false;
			for (size_t i = 0; i < l; ++i)
				if (m_str[i] != s[i])
					return false;
			return true;
		}

		bool empty() const
		{
			return m_str.empty();
		}

		filepath& operator+=(const std::string& rhs)
		{
			m_str += rhs;
			return *this;
		}

		operator std::string() const
		{
			return m_str;
		}

		filepath combine(const filepath& other)
		{
			filepath p(*this);
			// do we have a seperator at the end?
			if (!p.empty() && !p.endswith(filepath::delimeter()) && !other.startswith(filepath::delimeter()))
				p += filepath::delimeter();
			p += other;
			return p;
		}

		filepath absolute_directory() const noexcept
		{
			std::string copy = m_str;
			auto pos = copy.find_last_of(filepath::delimeter());
			if (pos == std::string::npos)
				return filepath(copy);
			copy.erase(copy.begin() + pos, copy.end());
			return filepath(copy);
		}

		std::string extension() const
		{
			auto pos = m_str.find_last_of('.');
			if (pos == std::string::npos)
				return "";
			return m_str.substr(pos + 1);
		}

		const char* c_str() const noexcept
		{
			return m_str.c_str();
		}

		const std::string& str() const noexcept
		{
			return m_str;
		}
	};
}; // namespace filesystem