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
			virtual void visit(VisitorFunction fn) override
			{
				fn(callee.get());
				for (auto& arg : arguments)
					fn(arg.get());
			}
		};
	}; // namespace ast
};	   // namespace compiler