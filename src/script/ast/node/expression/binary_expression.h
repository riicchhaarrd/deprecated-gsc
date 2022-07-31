#pragma once
#include <script/ast/node/expression.h>
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct BinaryExpression : Expression
		{
			std::unique_ptr<Expression> left;
			std::unique_ptr<Expression> right;
			int op;

			AST_NODE(BinaryExpression)

			virtual void print(Printer& out) override
			{
				out.print("binary expression: %d", op);
				out.indent();
				out.print("left:");
				out.indent();
				left->print(out);
				out.unindent();
				out.print("right:");
				out.indent();
				right->print(out);
				out.unindent();
				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler