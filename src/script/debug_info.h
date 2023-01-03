#pragma once
#include <string>

namespace script
{
	struct DebugInfo
	{
		std::string file;
		std::string function;
		size_t line;
		std::string expression_string;
	};
};