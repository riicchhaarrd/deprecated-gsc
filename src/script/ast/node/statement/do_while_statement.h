#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace script
{
	namespace ast
	{
		struct DoWhileStatement : Statement
		{
			std::unique_ptr<Expression> test;
			std::unique_ptr<Statement> body;
			virtual void print(Printer& out) override
			{
				out.print("do while:");
				out.indent();

				out.print("test:");
				out.indent();
				test->print(out);
				out.unindent();

				out.print("body:");
				out.indent();
				body->print(out);
				out.unindent();

				out.unindent();
			}
			AST_NODE(DoWhileStatement)
		};
	}; // namespace ast
};	   // namespace compiler