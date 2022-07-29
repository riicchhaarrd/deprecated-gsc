#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct WhileStatement : Statement
		{
			std::unique_ptr<Expression> test;
			std::unique_ptr<Statement> body;
			virtual void print(Printer& out) override
			{
				out.print("while:");
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
		};
	}; // namespace ast
};	   // namespace compiler