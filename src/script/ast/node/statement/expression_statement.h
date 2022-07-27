#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ExpressionStatement : Statement
		{
			std::unique_ptr<Expression> expression;
		};
	}; // namespace ast
};	   // namespace compiler