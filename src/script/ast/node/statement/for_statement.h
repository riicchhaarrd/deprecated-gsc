#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ForStatement : Statement
		{
			std::unique_ptr<Statement> init; //kinda special statement, init-statement
			std::unique_ptr<Expression> test;
			std::unique_ptr<Statement> update;
			std::unique_ptr<Statement> body;
			virtual void print(Printer& out) override
			{
				out.print("for:");
				out.indent();

				out.print("init:");
				out.indent();
				init->print(out);
				out.unindent();

				out.print("test:");
				out.indent();
				test->print(out);
				out.unindent();

				out.print("update:");
				out.indent();
				update->print(out);
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