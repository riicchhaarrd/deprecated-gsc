#pragma once
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct UnaryExpression : Expression
		{
			std::unique_ptr<Expression> argument;
			int op;
			int prefix;
		};
	}; // namespace ast
};	   // namespace compiler