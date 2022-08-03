#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace script
{
	namespace ast
	{
		struct ExpressionStatement : Statement
		{
			std::unique_ptr<Expression> expression;
			virtual void print(Printer& out) override
			{
				out.print("expression statement:");
				out.indent();
				expression->print(out);
				out.unindent();
			}

			AST_NODE(ExpressionStatement)
		};
	}; // namespace ast
};	   // namespace compiler