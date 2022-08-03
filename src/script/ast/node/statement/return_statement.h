#pragma once
#include "../statement.h"
#include "../expression.h"
#include <string>
#include <vector>
#include <memory>

namespace script
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

			AST_NODE(ReturnStatement)
		};
	}; // namespace ast
};	   // namespace compiler