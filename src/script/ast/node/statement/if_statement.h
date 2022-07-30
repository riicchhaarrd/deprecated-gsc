#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct IfStatement : Statement
		{
			std::unique_ptr<Expression> test;
			std::unique_ptr<Statement> consequent;
			std::unique_ptr<Statement> alternative;
			virtual void print(Printer& out) override
			{
				out.print("if:");
				out.indent();
				out.print("test:");
				out.indent();
				test->print(out);
				out.unindent();
				
				out.print("consequent:");
				out.indent();
				consequent->print(out);
				out.unindent();

				if (alternative)
				{
					out.print("alternative:");
					out.indent();
					alternative->print(out);
					out.unindent();
				}

				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler