#pragma once
#include "../expression.h"
#include <string>
#include <memory>
#include <vector>

namespace compiler
{
	namespace ast
	{
		struct ArrayExpression : Expression
		{
			std::vector<std::unique_ptr<Expression>> elements;
			virtual void print(Printer& out) override
			{
				out.print("array expression: %d elements", elements.size());
			}
		};
	}; // namespace ast
};	   // namespace compiler