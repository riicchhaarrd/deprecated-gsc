#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <vector>
#include <memory>

namespace compiler
{
	namespace ast
	{
		struct ReturnStatement : Statement
		{
			std::unique_ptr<Expression> argument;
			virtual void print(Printer& out) override
			{
				out.print("return:");
				if (argument)
				{
					out.indent();
					argument->print(out);
					out.unindent();
				}
			}

			virtual void accept(ASTVisitor& visitor)
			{
				visitor.visit(*this);
			}
		};
	}; // namespace ast
};	   // namespace compiler