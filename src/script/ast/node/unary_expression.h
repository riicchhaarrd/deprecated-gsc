#pragma once
#include "node.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct UnaryExpression : Node
		{
			std::unique_ptr<Node> argument;
			int op;
			int prefix;
		};
	}; // namespace ast
};	   // namespace compiler