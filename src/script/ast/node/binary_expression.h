#pragma once
#include "node.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct BinaryExpression : Node
		{
			std::unique_ptr<Node> left;
			std::unique_ptr<Node> right;
			int op;
		};
	}; // namespace ast
};	   // namespace compiler