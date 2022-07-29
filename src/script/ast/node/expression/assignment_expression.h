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
			virtual void print(Printer& out) override
			{
				out.print("assignment expression: %d", op);
				out.indent();
				out.print("lhs:");
				lhs->print(out);
				out.print("rhs:");
				rhs->print(out);
				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler