#pragma once
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ConditionalExpression : Expression //TernaryExpression
		{
			std::unique_ptr<Expression> condition;
			std::unique_ptr<Expression> consequent;
			std::unique_ptr<Expression> alternative;
		};
	}; // namespace ast
};	   // namespace compiler