#pragma once
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct UnaryExpression : Expression
		{
			std::unique_ptr<Expression> argument;
			int op;
			bool prefix;

			virtual void print(Printer &out) override
			{
				out.print("unary expression op: %d, prefix: %d", op, prefix);
				out.print("argument:");
				out.indent();
				argument->print(out);
				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler