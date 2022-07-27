#pragma once
#include "node.h"
#include <string>

namespace compiler
{
	namespace ast
	{
		struct Identifier : Node
		{
			std::string name;
		};
	}; // namespace ast
};	   // namespace compiler