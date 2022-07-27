#pragma once
#include "node.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct MemberExpression : Node
		{
			std::unique_ptr<Node> object;
			std::unique_ptr<Node> prop;
		};
	}; // namespace ast
};	   // namespace compiler