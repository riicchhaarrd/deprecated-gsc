#pragma once
#include "node.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ExpressionStatement : Node
		{
			std::unique_ptr<Node> expression;
		};
	}; // namespace ast
};	   // namespace compiler