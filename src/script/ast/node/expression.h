#pragma once
#include <script/ast/node/node.h>
#include <string>

namespace compiler
{
	namespace ast
	{
		struct Expression : Node
		{
			virtual void accept(ASTVisitor& visitor) = 0;
		};
	}; // namespace ast
};	   // namespace compiler