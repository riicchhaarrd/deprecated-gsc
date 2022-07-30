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
			std::unique_ptr<Expression> init; //kinda special statement, init-statement
			std::unique_ptr<Expression> test;
			std::unique_ptr<Expression> update;
			std::unique_ptr<Statement> body;
			virtual void print(Printer& out) override
			{
				out.print("for:");
				out.indent();
				if (init)
				{
					out.print("init:");
					out.indent();
					init->print(out);
					out.unindent();
				}
				if (test)
				{
					out.print("test:");
					out.indent();
					test->print(out);
					out.unindent();
				}
				if (update)
				{
					out.print("update:");
					out.indent();
					update->print(out);
					out.unindent();
				}
				if (body)
				{
					out.print("body:");
					out.indent();
					body->print(out);
					out.unindent();
				}

				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler