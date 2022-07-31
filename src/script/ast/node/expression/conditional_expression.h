#pragma once
#include <script/ast/node/expression.h>
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ConditionalExpression : Expression //TernaryExpression
		{
			std::unique_ptr<Expression> condition;
			std::unique_ptr<Expression> consequent;
			std::unique_ptr<Expression> alternative;

			AST_NODE(ConditionalExpression)

			virtual void print(Printer& out) override
			{
				out.print("conditional expression:");

				out.indent();
				out.print("condition:");
				out.indent();
				condition->print(out);
				out.unindent();
				
				out.print("consequent:");
				out.indent();
				consequent->print(out);
				out.unindent();
				
				out.print("alternative:");
				out.indent();
				alternative->print(out);
				out.unindent();
				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler