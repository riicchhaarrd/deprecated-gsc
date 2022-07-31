#pragma once
#include <ostream>
#include <string>
#include <vector>

struct ostream_indent
{
	int level = 0;
	std::ostream& os;
	std::vector<std::string> indentations;
	std::string indentation;
	ostream_indent(std::ostream& o) : os(o)
	{
	}
	void unindent()
	{
		indent(-1);
	}
	void indent(int dir = 1)
	{
		if (dir > 0)
			indentation.push_back('\t');
		else
			indentation.pop_back();
		level += dir;
	}
	template <typename T> std::ostream& operator>>(T t)
	{
		os << indentation << t;
		return os;
	}
	template <typename T> std::ostream& operator<<(T t)
	{
		os << t;
		return os;
	}
	void save()
	{
		indentations.push_back(indentation);
		indentation.clear();
	}
	void restore()
	{
		indentation = indentations.back();
		indentations.pop_back();
	}
};