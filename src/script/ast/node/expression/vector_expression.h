#pragma once
#include "../expression.h"
#include <string>
#include <memory>
#include <vector>

namespace compiler
{
	namespace ast
	{
		struct VectorExpression : Expression
		{
			std::vector<std::unique_ptr<Expression>> elements;
			virtual void print(Printer& out) override
			{
				out.print("vector expression: %d elements", elements.size());
			}
		};
	}; // namespace ast
};	   // namespace compiler