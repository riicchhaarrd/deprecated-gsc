#pragma once
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct MemberExpression : Expression
		{
			std::unique_ptr<Expression> object;
			std::unique_ptr<Expression> prop;
		};
	}; // namespace ast
};	   // namespace compiler