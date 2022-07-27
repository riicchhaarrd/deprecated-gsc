#pragma once
#include "node.h"
#include <string>

namespace compiler
{
	namespace ast
	{
		struct Literal : Node
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