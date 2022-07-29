#pragma once
#include "../expression.h"
#include "identifier.h"
#include <string>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct CallExpression : Expression
		{
			std::unique_ptr<Identifier> callee;
			std::vector<std::unique_ptr<Expression>> arguments;
			virtual void visit(VisitorFunction fn) override
			{
				fn(callee.get());
				for (auto& arg : arguments)
					fn(arg.get());
			}
			virtual void print(Printer& out) override
			{
				out.print("call expression:");
				out.indent();
				out.print("callee:");
				out.indent();
				callee->print(out);

				out.unindent();

				out.print("arguments:");
				out.indent();
				for (auto& arg : arguments)
					arg->print(out);
				out.unindent();
				out.unindent();
			}
		};
	}; // namespace ast
};	   // namespace compiler