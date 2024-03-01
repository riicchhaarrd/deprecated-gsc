#ifndef COMMON_FORMAT_H
#define COMMON_FORMAT_H
#include <exception>
#include <sstream>
#include <string>
#include <vector>

namespace common
{
	struct Formatter
	{
		std::string_view fmt;
		std::vector<std::string> arguments;

		Formatter(std::string_view fmt_) : fmt(fmt_)
		{
		}

		// first build arguments, then we can refer to them with either {} or {1} etc
		// you could use ostream/stringstream to go through them in one go

		template <typename T> void pass(const T& t)
		{
			std::stringstream ss;
			ss << t;
			arguments.push_back(ss.str());
		}

		void expand()
		{
		}

		template <typename T, typename... Args> void expand(const T& t, const Args&... args)
		{
			pass(t);
			expand(args...);
		}

		std::string build_string()
		{
			std::stringstream ss;
			size_t n = fmt.size();
			size_t arg = 0;
			for (size_t i = 0; i < n; ++i)
			{
				if (fmt[i] == '{' && fmt[i + 1] == '}')
				{
					if (arg >= arguments.size())
						throw std::out_of_range("arg out of range for arguments");
					ss << arguments[arg];
					++arg;
					++i;
				}
				else
				{
					ss << fmt[i]; // this is kinda slow, maybe scan for { and then just pass the whole buffer instead
				}
			}
			return ss.str();
		}
	};

	template <typename... Ts> std::string format(const std::string_view fmt, Ts... ts)
	{
		Formatter f(fmt);
		f.expand(ts...);
		return f.build_string();
	}

	static std::string format(const std::string_view fmt)
	{
		return std::string(fmt);
	}
}; // namespace common
#endif
