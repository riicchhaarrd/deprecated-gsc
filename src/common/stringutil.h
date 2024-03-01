#pragma once
#include <string>
#include <sstream>

namespace util
{
	namespace string
	{
		static std::string replace_escape_sequence_with_newline(std::string str)
		{
			std::string::size_type index = 0;
			while ((index = str.find("\\n", index)) != std::string::npos)
			{
				str.replace(index, 2, "\n");
				++index;
			}
			return str;
		}
		static std::string replace_newline_with_escape_sequence(const std::string& str)
		{
			size_t n = 0;
			std::string output;
			{
				std::stringstream ss(str);
				std::string line;
				while (std::getline(ss, line, '\n'))
				{
					++n;
				}
			}
			{
				std::stringstream ss(str);
				std::string line;
				size_t i = 0;
				while (std::getline(ss, line, '\n'))
				{
					output += line;
					if (i < n - 1)
						output += "\\n";
					++i;
				}
			}
			return output;
		}
		static std::string to_lower(const std::string& str)
		{
			std::string s;
			for (auto& c : str)
			{
				if(c >= 'A' && c <= 'Z')
					s.push_back('a' + (c - 'A'));
				else
					s.push_back(c);
			}
			return s;
		}
		static std::string to_upper(const std::string& str)
		{
			std::string s;
			for (auto& c : str)
			{
				if (c >= 'a' && c <= 'z')
					s.push_back('A' + (c - 'a'));
				else
					s.push_back(c);
			}
			return s;
		}
	};
};