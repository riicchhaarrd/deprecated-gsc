#pragma once
#include "../expression.h"
#include <string>

namespace compiler
{
	namespace ast
	{
		struct Identifier : Expression
		{
			std::string name;
			Identifier(const std::string s)
			{
				name = s;
			}
		};
	}; // namespace ast
};	   // namespace compiler