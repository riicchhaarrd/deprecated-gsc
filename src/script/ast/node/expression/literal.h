#pragma once
#include "../expression.h"
#include <string>

namespace compiler
{
	namespace ast
	{
		struct Literal : Expression
		{
			enum class Type
			{
				kString,
				kInteger,
				kNumber,
				kVector
			} type;
			std::string value;
		};
	}; // namespace ast
};	   // namespace compiler