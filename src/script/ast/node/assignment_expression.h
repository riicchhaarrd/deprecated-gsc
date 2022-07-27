#pragma once
#include "node.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct AssignmentExpression : Node
		{
			std::unique_ptr<Node> lhs;
			std::unique_ptr<Node> rhs;
			int op;
		};
	}; // namespace ast
};	   // namespace compiler