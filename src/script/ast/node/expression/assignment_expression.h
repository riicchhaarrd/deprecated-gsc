#pragma once
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct AssignmentExpression : Expression
		{
			std::unique_ptr<Expression> lhs;
			std::unique_ptr<Expression> rhs;
			int op;
		};
	}; // namespace ast
};	   // namespace compiler