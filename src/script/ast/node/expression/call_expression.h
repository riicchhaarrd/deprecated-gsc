#pragma once
#include "../expression.h"
#include "identifier.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct CallExpression : Expression
		{
			std::unique_ptr<Identifier> callee;
			std::vector<std::unique_ptr<Expression>> arguments;
		};
	}; // namespace ast
};	   // namespace compiler