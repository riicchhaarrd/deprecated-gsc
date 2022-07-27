#pragma once
#include "node.h"
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ReturnStatement : Node
		{
			std::unique_ptr<Node> argument;
		};
	}; // namespace ast
};	   // namespace compiler