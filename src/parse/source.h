#pragma once
#include <string>

namespace parse
{
	class source
	{
		std::string m_buffer;
		std::string m_path;

	  public:
		source()
		{
		}
		source(const std::string& path, const std::string& buf) : m_path(path), m_buffer(buf)
		{
		}

		const int length() const
		{
			return m_buffer.size();
		}

		const int operator[](const size_t index) const
		{
			if (index >= m_buffer.size())
				return -1;
			return m_buffer[index];
		}

		int line_number(int cur) const
		{
			int n = 0;
			for (int i = 0; i < cur; ++i)
				if (m_buffer[i] == '\n')
					++n;
			return n;
		}

		std::string extract_string(int start, int n) const
		{
			if (start >= 0 && n > 0)
				return m_buffer.substr(start, n);
			return "";
		}
	};
};