#pragma once
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct BinaryExpression : Expression
		{
			std::unique_ptr<Expression> left;
			std::unique_ptr<Expression> right;
			int op;
		};
	}; // namespace ast
};	   // namespace compiler