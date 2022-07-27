#pragma once
#include "node.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct CallExpression : Node
		{
			std::unique_ptr<Node> callee;
			std::vector<std::unique_ptr<Node>> arguments;
		};
	}; // namespace ast
};	   // namespace compiler