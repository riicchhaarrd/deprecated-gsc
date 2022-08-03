#pragma once
#include <script/ast/node/expression.h>
#include <string>
#include <memory>

namespace script
{
	namespace ast
	{
		struct UnaryExpression : Expression
		{
			std::unique_ptr<Expression> argument;
			int op;
			bool prefix;
			AST_NODE(UnaryExpression)

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