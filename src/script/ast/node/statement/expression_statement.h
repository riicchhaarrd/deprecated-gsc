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
			virtual void print(Printer& out) override
			{
				out.print("expression statement:");
				out.indent();
				expression->print(out);
				out.unindent();
			}

			virtual void accept(ASTVisitor& visitor)
			{
				visitor.visit(*this);
			}
		};
	}; // namespace ast
};	   // namespace compiler